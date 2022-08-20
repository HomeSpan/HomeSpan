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
 
#pragma once

#include <Arduino.h>
#include <driver/timer.h>

namespace Utils {

char *readSerial(char *c, int max);   // read serial port into 'c' until <newline>, but storing only first 'max' characters (the rest are discarded)
String mask(char *c, int n);          // simply utility that creates a String from 'c' with all except the first and last 'n' characters replaced by '*'
  
}

/////////////////////////////////////////////////
// Creates a temporary buffer that is freed after
// going out of scope

template <class bufType>
struct TempBuffer {
  bufType *buf;
  int nBytes;
  
  TempBuffer(size_t len){
    nBytes=len*sizeof(bufType);
    buf=(bufType *)heap_caps_malloc(nBytes,MALLOC_CAP_8BIT);
    if(buf==NULL){
      Serial.print("\n\n*** FATAL ERROR: Requested allocation of ");
      Serial.print(nBytes);
      Serial.print(" bytes failed.  Program Halting.\n\n");
      while(1);
    }
   }

  ~TempBuffer(){
    heap_caps_free(buf);
  }

  int len(){
    return(nBytes);
  }
  
};

////////////////////////////////
//         PushButton         //
////////////////////////////////

class PushButton{
  
  int status;
  boolean doubleCheck;
  uint32_t singleAlarm;
  uint32_t doubleAlarm;
  uint32_t longAlarm;
  int pressType;

#if SOC_TOUCH_SENSOR_NUM > 0
  static touch_value_t threshold;
  static const int calibCount=20;
#endif
  
  protected:

  typedef boolean (*triggerType_t)(int pin);

  int pin;
  triggerType_t triggerType;

  public:

  enum {
    SINGLE=0,
    DOUBLE=1,
    LONG=2
  };

  static boolean TRIGGER_ON_LOW(int pin){return(!digitalRead(pin));}
  static boolean TRIGGER_ON_HIGH(int pin){return(digitalRead(pin));}

#if SOC_TOUCH_VERSION_1 // ESP32
  static boolean TRIGGER_ON_TOUCH(int pin){return(touchRead(pin)<threshold);}
#elif SOC_TOUCH_VERSION_2 // ESP32S2 ESP32S3
  static boolean TRIGGER_ON_TOUCH(int pin){return(touchRead(pin)>threshold);}
#endif

  PushButton(int pin, triggerType_t triggerType=TRIGGER_ON_LOW);

//  Creates pushbutton of specified type on specified pin
//
//  pin:          pin number to which the button is connected
//  triggerType:  a function of of the form 'boolean f(int)' that is passed
//                the parameter *pin* and returns TRUE if the button associated
//                with *pin* is pressed, or FALSE if not.  Can choose from 3 pre-specifed
//                triggerType_t functions (TRIGGER_ON_LOW, TRIGGER_ON_HIGH, and TRIGGER_ON_TOUCH), or write your
//                own custom handler

  void reset();

//  Resets state of PushButton.  Should be called once before any loops that will
//  repeatedly check the button for a trigger event.

  boolean triggered(uint16_t singleTime, uint16_t longTime, uint16_t doubleTime=0);

//  Returns true if button has been triggered by an press event based on the following parameters:

//  singleTime:   the minimum time required for the button to be pressed to trigger a Single Press
//  doubleTime:   the maximum time allowed between button presses to qualify as a Double Press
//  longTime:     the minimum time required for the button to be pressed and held to trigger a Long Press

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

//  Returns 0=Single Press, 1=Double Press, or 2=Long Press 

  void wait();

//  Waits for button to be released.  Use after Long Press if button release confirmation is desired

  int getPin(){return(pin);}

//  Returns pin number

#if SOC_TOUCH_SENSOR_NUM > 0

  static void setTouchCycles(uint16_t measureTime, uint16_t sleepTime){touchSetCycles(measureTime,sleepTime);}
  
//  Sets the measure time and sleep time touch cycles , and lower threshold that triggers a touch - used only when triggerType=PushButton::TRIGGER_ON_TOUCH

//  measureTime:      duration of measurement time of all touch sensors in number of clock cycles
//  sleepTime:        duration of sleep time (between measurements) of all touch sensors number of clock cycles

  static void setTouchThreshold(touch_value_t thresh){threshold=thresh;}

//  Sets the threshold that triggers a touch - used only when triggerType=TRIGGER_ON_TOUCH

//  thresh:  the read value of touch sensors, beyond which which sensors are considered touched (i.e. "pressed").
//           This is a class-level value applied to all touch sensor buttons.

#endif

};

////////////////////////////////
//         Blinker            //
////////////////////////////////

class Blinker {
  
  timer_group_t group;
  timer_idx_t idx;
  int pin;

  int nBlinks;
  int onTime;
  int offTime;
  int delayTime;
  int count;
  
  unsigned long pauseDuration;
  unsigned long pauseTime;
  boolean isPaused=false;

  static void isrTimer(void *arg); 

  public:

  Blinker();
  Blinker(int pin, int timerNum=0, uint16_t autoOffDuration=0);

//  Creates a generic blinking LED on specified pin controlled
//  in background via interrupts generated by an ESP32 Alarm Timer.
//
//  In the first form, a Blinker is instantiated without specifying
//  the pin.  In this case the pin must be specified in a subsequent call 
//  to init() before the Blinker can be used.
//
//  In the second form, a Blinker is instantiated and initialized with
//  the specified pin, obviating the need for a separate call to init().
//
//  pin:              Pin mumber to control.  Blinker will set pinMode to OUTPUT automatically 
//
//  timerNum:         ESP32 Alarm Timer to use.
//                    For ESP32 and ESP32-S2:  0=Group0/Timer0, 1=Group0/Timer1, 2=Group1/Timer0, 3=Group1/Timer1
//                    For ESP32-C3:  0=Group0/Timer0, 1=Group1/Timer0
//
//  autoOffDuration:  If greater than zero, Blinker will automatically turn off after autoOffDuration (in seconds) has elapsed
//                    Blinker will resume normal operation upon next call to start(), on(), or off()
//                    Program must periodically call check() for auto-off functionality to work
    
  void init(int pin, int timerNum=0, uint16_t autoOffDuration=0);

//  Initializes Blinker, if not configured during instantiation.
//
//  pin:              Pin mumber to control.  Blinker will set pinMode to OUTPUT automatically 
//
//  timerNum:         ESP32 Alarm Timer to use.
//                    For ESP32 and ESP32-S2:  0=Group0/Timer0, 1=Group0/Timer1, 2=Group1/Timer0, 3=Group1/Timer1
//                    For ESP32-C3:  0=Group0/Timer0, 1=Group1/Timer0
//
//  autoOffDuration:  If greater than zero, Blinker will automatically turn off after autoOffDuration (in seconds) has elapsed
//                    Blinker will resume normal operation upon next call to start(), on(), or off()
//                    Program must periodically call check() for auto-off functionality to work

  void start(int period, float dutyCycle=0.5);
    
//  Starts simple ON/OFF blinking.
//
//  period:      ON/OFF blinking period, in milliseconds
//  dutyCycle:   Fraction of period that LED is ON (default=50%)

  void start(int period, float dutyCycle, int nBlinks, int delayTime);

//  Starts ON/OFF blinking pattern.
//
//  period:      ON/OFF blinking period, in milliseconds, used for blinking portion of pattern
//  dutyCycle:   Fraction of period that LED is ON (default=50%)
//  nBlinks:     Number of blinks in blinking portion of pattern
//  delayTime:   delay, in milliseconds, during which LED is off before restarting blinking pattern

  void stop();

//  Stops current blinking pattern.

  void on();

//  Stops current blinking pattern and turns on LED

  void off();

//  Stops current blinking pattern and turns off LED

  void check();

//  Optional check to see if LED output should be paused (check is bypassed if pauseDuration=0)


};
