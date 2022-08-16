/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2022 Gregg E. Berman
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Contains various generic utility functions and classes:
//
//  Utils::readSerial       - reads all characters from Serial port and saves only up to max specified
//  Utils::mask             - masks a string with asterisks (good for displaying passwords)
//
//  class PushButton        - tracks Single, Double, and Long Presses of a pushbutton that connects a specified pin to ground
//  class Blinker           - creates customized blinking patterns on an LED connected to a specified pin
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char *Utils::readSerial(char *c, int max){
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
    Serial.printf("Touch Sensor at pin=%d used for calibration.  Triggers when sensor reading < %d.\n",pin,threshold);
#elif SOC_TOUCH_VERSION_2
    threshold*=2;
    Serial.printf("Touch Sensor at pin=%d used for calibration.  Triggers when sensor reading > %d.\n",pin,threshold);
#endif
  }
#endif
  
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

#if SOC_TOUCH_SENSOR_NUM > 0
  touch_value_t PushButton::threshold=0;
#endif

////////////////////////////////
//         Blinker            //
////////////////////////////////

Blinker::Blinker(){
}

//////////////////////////////////////

Blinker::Blinker(int pin, int timerNum, uint16_t autoOffDuration){
  init(pin, timerNum, autoOffDuration);
}

//////////////////////////////////////

void Blinker::init(int pin, int timerNum, uint16_t autoOffDuration){

  this->pin=pin;
  if(pin<0)
    return;
  
  pinMode(pin,OUTPUT);
  digitalWrite(pin,0);

  pauseDuration=autoOffDuration*1000;

#if SOC_TIMER_GROUP_TIMERS_PER_GROUP>1                        // ESP32 and ESP32-S2 contains two timers per timer group
  group=((timerNum/2)%2==0)?TIMER_GROUP_0:TIMER_GROUP_1;
  idx=(timerNum%2==0)?TIMER_0:TIMER_1;                        // ESP32-C3 only contains one timer per timer group
#else
  group=(timerNum%2==0)?TIMER_GROUP_0:TIMER_GROUP_1;
  idx=TIMER_0;
#endif

  timer_config_t conf;
  conf.alarm_en=TIMER_ALARM_EN;
  conf.counter_en=TIMER_PAUSE;
  conf.intr_type=TIMER_INTR_LEVEL;
  conf.counter_dir=TIMER_COUNT_UP;
  conf.auto_reload=TIMER_AUTORELOAD_EN;
  conf.divider=getApbFrequency()/10000;                      // set divider to yield 10 kHz clock (0.1 ms pulses)

#ifdef SOC_TIMER_GROUP_SUPPORT_XTAL                          // set clock to APB (default is XTAL!) if clk_src is defined in conf structure
  conf.clk_src=TIMER_SRC_CLK_APB;
#endif

  timer_init(group,idx,&conf);
  timer_isr_register(group,idx,Blinker::isrTimer,(void *)this,0,NULL);
  timer_enable_intr(group,idx);

}

//////////////////////////////////////

void Blinker::isrTimer(void *arg){

  Blinker *b=(Blinker *)arg;

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0)               // use new method that is generic to ESP32, S2, and C3
  timer_group_clr_intr_status_in_isr(b->group,b->idx);
#else                                                             // use older method that is only for ESP32
  if(b->group){
    if(b->idx)
      TIMERG1.int_clr_timers.t1=1;
    else
      TIMERG1.int_clr_timers.t0=1;
  } else {
    if(b->idx)
      TIMERG0.int_clr_timers.t1=1;
    else
      TIMERG0.int_clr_timers.t0=1;    
  }
#endif
  
  if(!digitalRead(b->pin)){
    digitalWrite(b->pin,1);
    timer_set_alarm_value(b->group,b->idx,b->onTime);
    b->count--;
  } else {
    digitalWrite(b->pin,0);    
    if(b->count){
      timer_set_alarm_value(b->group,b->idx,b->offTime);
    } else {
      timer_set_alarm_value(b->group,b->idx,b->delayTime);
      b->count=b->nBlinks;      
    }
  }
  
  timer_set_alarm(b->group,b->idx,TIMER_ALARM_EN);

}

//////////////////////////////////////

void Blinker::start(int period, float dutyCycle){

  start(period, dutyCycle, 1, 0);
}

//////////////////////////////////////

void Blinker::start(int period, float dutyCycle, int nBlinks, int delayTime){

  if(pin<0)
    return;

  pauseTime=millis();
  isPaused=false;
  gpio_set_direction((gpio_num_t)pin, GPIO_MODE_INPUT_OUTPUT);      // needed to ensure digitalRead() functions correctly on ESP32-C3; also needed to re-enable after pause()

  period*=10;
  onTime=dutyCycle*period;
  offTime=period-onTime;
  this->delayTime=delayTime*10+offTime;
  this->nBlinks=nBlinks;
  count=nBlinks;
  timer_set_counter_value(group,idx,0);
  timer_set_alarm_value(group,idx,0);  
  timer_start(group,idx);
}

//////////////////////////////////////

void Blinker::stop(){

  if(pin<0)
    return;
  
  timer_pause(group,idx);  
}

//////////////////////////////////////

void Blinker::on(){

  if(pin<0)
    return;

  pauseTime=millis();
  isPaused=false;
  gpio_set_direction((gpio_num_t)pin, GPIO_MODE_INPUT_OUTPUT);

  stop();
  digitalWrite(pin,1);
}

//////////////////////////////////////

void Blinker::off(){

  if(pin<0)
    return;

  pauseTime=millis();
  isPaused=false;
  gpio_set_direction((gpio_num_t)pin, GPIO_MODE_INPUT_OUTPUT);

  stop();
  digitalWrite(pin,0);
}

//////////////////////////////////////

void Blinker::check(){

  if(pin<0)
    return;

  if(pauseDuration==0 || isPaused || (millis()-pauseTime)<pauseDuration)
    return;

  Serial.print("Pausing Status LED\n");
  isPaused=true;
  gpio_set_direction((gpio_num_t)pin, GPIO_MODE_DISABLE);
}
