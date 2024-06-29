/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2024 Gregg E. Berman
 *  
 *  https://github.com/HomeSpan/HomeSpan
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *  
 ********************************************************************************/
 
#include "Utils.h"
#include "HomeSpan.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Contains various generic utility functions and classes:
//
//  Utils::readSerial       - reads all characters from Serial port and saves only up to max specified
//  Utils::mask             - masks a string with asterisks (good for displaying passwords)
//
//  class PushButton        - tracks Single, Double, and Long Presses of a pushbutton that connects a specified pin to ground
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char *Utils::readSerial(char *c, int max){

  if(homeSpan.getSerialInputDisable()){
    c[0]='\0';
    return(c);
  }
  
  int i=0;
  char buf;

  while(1){

    while(!Serial.available());       // wait until there is a new character
    
    buf=Serial.read();
    
    if(buf=='\n'){         // exit upon newline
      if(i>0)              // characters have been typed
        c[i]='\0';            // replace newline with string terminator
      return(c);           // return updated string
    }

    if(buf!='\r'){         // save any character except carriage return
      c[i]=buf;               // store new character    
      if(i<max)               // do not store more than max characters (excluding string terminator)
        i++;
    }
  
  } // while(1)
  
} // readSerial

//////////////////////////////////////

String Utils::mask(char *c, int n){
  String s="";
  int len=strlen(c);
  
  for(int i=0;i<len;i++){
    if(i<n || i>=len-n)
      s+=c[i];
    else
      s+='*';
  }
  
  return(s);  
} // mask

////////////////////////////////
//         PushButton         //
////////////////////////////////

PushButton::PushButton(int pin, triggerType_t triggerType){

  this->pin=pin;
  this->triggerType=triggerType;
  
  status=0;
  doubleCheck=false;

  if(triggerType==TRIGGER_ON_LOW)
    pinMode(pin, INPUT_PULLUP);
  else if(triggerType==TRIGGER_ON_HIGH)
    pinMode(pin, INPUT_PULLDOWN);
  
#if SOC_TOUCH_SENSOR_NUM > 0
  else if (triggerType==TRIGGER_ON_TOUCH && threshold==0){    
    for(int i=0;i<calibCount;i++)
      threshold+=touchRead(pin);
    threshold/=calibCount;
#if SOC_TOUCH_VERSION_1
    threshold/=2;
    LOG0("Touch Sensor at pin=%d used for calibration.  Triggers when sensor reading < %d.\n",pin,threshold);
#elif SOC_TOUCH_VERSION_2
    threshold*=2;
    LOG0("Touch Sensor at pin=%d used for calibration.  Triggers when sensor reading > %d.\n",pin,threshold);
#endif
  }
#endif

  if(triggerType(pin)){
    pressType=CLOSED;
    toggleStatus=2;
  } else {
    pressType=OPEN;
    toggleStatus=0;
  }
  
}

//////////////////////////////////////

boolean PushButton::triggered(uint16_t singleTime, uint16_t longTime, uint16_t doubleTime){

  unsigned long cTime=millis();

  switch(status){
    
    case 0:
      if(doubleCheck && cTime>doubleAlarm){
        doubleCheck=false;
        pressType=SINGLE;
        return(true);
      }
      
      if(triggerType(pin)){         // button is "pressed"
        singleAlarm=cTime+singleTime;
        if(!doubleCheck){
          status=1;
          doubleAlarm=singleAlarm+doubleTime;
          longAlarm=cTime+longTime;
        } else {
          status=4;
        }
      }
    break;  
  
    case 1:
    case 2:
      if(!triggerType(pin)){         // button is released          
        status=0;
        if(cTime>singleAlarm){
          doubleCheck=true;
        }
      } else
      
      if(cTime>longAlarm){          // button is long-pressed
        longAlarm=cTime+longTime;
        status=3;
        pressType=LONG;
        return(true);
      }
    break;

    case 3:
      if(!triggerType(pin))          // button has been released after a long press
        status=0;
      else if(cTime>longAlarm){
        longAlarm=cTime+longTime;
        pressType=LONG;
        return(true);        
      }
    break;

    case 4:    
      if(!triggerType(pin)){         // button is released          
        status=0;
      } else
      
      if(cTime>singleAlarm){         // button is still pressed
        status=5;
        pressType=DOUBLE;
        doubleCheck=false;
        return(true);
      }
    break;

    case 5:
      if(!triggerType(pin))          // button has been released after double-click
        status=0;
     break;

  }

  return(false);
}

//////////////////////////////////////

boolean PushButton::toggled(uint16_t toggleTime){

  unsigned long cTime=millis();

  switch(toggleStatus){
    
    case 0:      
      if(triggerType(pin)){         // switch is toggled CLOSED
        singleAlarm=cTime+toggleTime;
        toggleStatus=1;
        }
    break;  
  
    case 1:
      if(!triggerType(pin)){       // switch is toggled back OPEN too soon
        toggleStatus=0;
      }
      
      else if(cTime>singleAlarm){  // switch has been in CLOSED state for sufficient time
        toggleStatus=2;
        pressType=CLOSED;
        return(true);
      }
    break;

    case 2:
      if(!triggerType(pin)){       // switch is toggled OPEN after being in CLOSED state
        toggleStatus=0;
        pressType=OPEN;
        return(true);        
      }
    break;
    
  } // switch

  return(false);
}

//////////////////////////////////////

boolean PushButton::primed(){
 
  if(millis()>singleAlarm && status==1){
    status=2;
    return(true);
  }
  
  return(false);
}

//////////////////////////////////////

int PushButton::type(){
  return(pressType);
}

//////////////////////////////////////

void PushButton::wait(){  
  while(triggerType(pin));
}

//////////////////////////////////////

void PushButton::reset(){
  status=0;
}

//////////////////////////////////////

PushButton::touch_value_t PushButton::threshold=0;
