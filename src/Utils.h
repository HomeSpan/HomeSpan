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
 
#pragma once

#include <Arduino.h>

#include "PSRAM.h"

namespace Utils {

char *readSerial(char *c, int max);   // read serial port into 'c' until <newline>, but storing only first 'max' characters (the rest are discarded)
String mask(char *c, int n);          // simply utility that creates a String from 'c' with all except the first and last 'n' characters replaced by '*'
  
}

/////////////////////////////////////////////////
// Creates a temporary buffer that is freed after
// going out of scope

template <class bufType>
class TempBuffer {

  private:
  
  bufType *buf=NULL;
  size_t nElements;

  public:
  
  TempBuffer(size_t _nElements=1) : nElements(_nElements) {
    buf=(bufType *)HS_MALLOC(nElements*sizeof(bufType));
    if(buf==NULL){
      Serial.printf("\n\n*** FATAL ERROR: Requested allocation of %d bytes failed.  Program Halting.\n\n",nElements*sizeof(bufType));
      while(1);
    }
   }

  TempBuffer(bufType *addBuf...) : nElements(0) {
    va_list args;
    va_start(args,addBuf);
    while(addBuf!=NULL){
      size_t addElements=va_arg(args,size_t);    
      buf=(bufType *)HS_REALLOC(buf,(nElements+addElements)*sizeof(bufType));
      if(buf==NULL){
        Serial.printf("\n\n*** FATAL ERROR: Requested allocation of %d bytes failed.  Program Halting.\n\n",nElements*sizeof(bufType));
        while(1);
      }
      memcpy(buf+nElements,addBuf,addElements*sizeof(bufType));
      nElements+=addElements;
      addBuf=va_arg(args,bufType *);
    }
    va_end(args);   
   }
   
  ~TempBuffer(){
    free(buf);
  }

  int len(){
    return(nElements*sizeof(bufType));
  }

  int size(){
    return(nElements);
  }

  bufType *get(){
    return(buf);
  }

  operator bufType*() const{
    return(buf);
  }
  
};

////////////////////////////////
//         PushButton         //
////////////////////////////////

class PushButton{
  
  int status;
  int toggleStatus;
  boolean doubleCheck;
  uint32_t singleAlarm;
  uint32_t doubleAlarm;
  uint32_t longAlarm;

#if SOC_TOUCH_VERSION_2
  typedef uint32_t touch_value_t;
#else
  typedef uint16_t touch_value_t;
#endif  
  
  static touch_value_t threshold;
  static const int calibCount=20;

  public:

  typedef boolean (*triggerType_t)(int pin);
  
  protected:

  int pressType;
  int pin;
  triggerType_t triggerType;

  public:

  enum {
    SINGLE=0,       // applicable only for push button
    DOUBLE=1,       // applicable only for push button
    LONG=2,         // applicable only for push button
    CLOSED=3,       // applicable only for toggle switch
    OPEN=4          // applicable only for toggle switch
  };

  static boolean TRIGGER_ON_LOW(int pin){return(!digitalRead(pin));}
  static boolean TRIGGER_ON_HIGH(int pin){return(digitalRead(pin));}

#if SOC_TOUCH_SENSOR_NUM > 0
#if SOC_TOUCH_VERSION_2
  static boolean TRIGGER_ON_TOUCH(int pin){return(touchRead(pin)>threshold);}
#else
  static boolean TRIGGER_ON_TOUCH(int pin){return(touchRead(pin)<threshold);}
#endif
#endif

  PushButton(int pin, triggerType_t triggerType=TRIGGER_ON_LOW);

//  Creates a push-button/toggle-switch of specified type on specified pin
//
//  pin:          pin number to which the button is connected
//  triggerType:  a function of of the form 'boolean f(int)' that is passed
//                the parameter *pin* and returns TRUE if the button associated
//                with *pin* is pressed/on, or FALSE if not.  Can choose from 3 pre-specifed
//                triggerType_t functions (TRIGGER_ON_LOW, TRIGGER_ON_HIGH, and TRIGGER_ON_TOUCH),
//                or write your own custom handler

  void reset();

//  Resets state of PushButton.  Should be called once before any loops that will
//  repeatedly check the button for a trigger() event.

  boolean triggered(uint16_t singleTime, uint16_t longTime, uint16_t doubleTime=0);

//  Returns true if button has been triggered by an press event based on the following parameters:

//    singleTime:   the minimum time required for the button to be pressed to trigger a Single Press
//    doubleTime:   the maximum time allowed between button presses to qualify as a Double Press
//    longTime:     the minimum time required for the button to be pressed and held to trigger a Long Press

//  All times are in milliseconds (ms). Trigger Rules:

//  * If button is pressed and continuously held, a Long Press will be triggered every longTime ms until the
//    button is released.
//  * If button is pressed for more than singleTime ms but less than longTime ms and then released, a Single Press
//    will be triggered, UNLESS
//  * The button is pressed a second time within doubleTime ms AND held again for at least singleTime ms, in which case
//    a DoublePress will be triggered.  No further events will occur until the button is released.
//  * If singleTime>longTime, only Long Press triggers can occur.
//  * If doubleTime=0, Double Presses cannot occur.
//  * Once triggered() returns true, if will subsequently return false until there is a new trigger event.

  boolean primed();

//  Returns true if button has been pressed and held for greater than singleTime, but has not yet been released.
//  After returning true, subsequent calls will always return false until the button has been released and reset.

  int type();

//  Returns the press type based on the whether triggered() or toggled() is called:

//  * For a push button, returns the last trigger event: 0=Single Press, 1=Double Press, 2=Long Press
//  * For a toggle switch, returns the current state of the switch: 4=ON, 5=OFF

  void wait();

//  Waits for button to be released.  Use after Long Press if button release confirmation is desired

  boolean toggled(uint16_t toggleTime);

//  Returns true if switch has been toggled, where

//    toggleTime:   the minimum time (in milliseconds) a switch needs to be ON to register a toggle event

//  Once toggled() returns true, if will subsequently return false until the switch is toggled again.  

  int getPin(){return(pin);}

//  Returns pin number

#if SOC_TOUCH_SENSOR_NUM > 0

  static void setTouchCycles(uint16_t measureTime, uint16_t sleepTime){touchSetCycles(measureTime,sleepTime);}
  
//  Sets the measure time and sleep time touch cycles , and lower threshold that triggers a touch - used only when triggerType=PushButton::TRIGGER_ON_TOUCH

//    measureTime:  duration of measurement time of all touch sensors in number of clock cycles
//    sleepTime:    duration of sleep time (between measurements) of all touch sensors number of clock cycles

  static void setTouchThreshold(touch_value_t thresh){threshold=thresh;}

//  Sets the threshold that triggers a touch - used only when triggerType=TRIGGER_ON_TOUCH

//    thresh:  the read value of touch sensors, beyond which which sensors are considered touched (i.e. "pressed").
//             This is a class-level value applied to all touch sensor buttons.

#endif

};
