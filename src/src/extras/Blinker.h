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
#include <driver/timer.h>

[[maybe_unused]] static const char* BLINKER_TAG = "Blinker";

////////////////////////////////
//     Blinkable Interface    //
////////////////////////////////

class Blinkable {
  public:
  
  virtual void on()=0;
  virtual void off()=0;
  virtual int getPin()=0;
};

////////////////////////////////
//         Blinker            //
////////////////////////////////

class Blinker {

  enum STATUS {OFF, BLINKING, ON};
  
  TaskHandle_t blinkHandle = NULL;
  Blinkable *led;

  int nBlinks;
  int onTime;
  int offTime;
  int delayTime;
  STATUS status=STATUS::OFF;
  
  unsigned long pauseDuration;
  unsigned long pauseTime;
  boolean isPaused=false;

  static void blinkTask(void *arg);

  public:

  Blinker(Blinkable *led, uint16_t autoOffDuration=0);

//  Creates a generic blinking LED in a separate task thread
//
//  led:              An initialized LED device that implements the Blinkable Interface
////
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

  void refresh(){if(status==STATUS::ON)on();}

//  Refreshes LED color by turning device ON if status=ON (if status=BLINKING, new color is automatically used at next blink)

  void check();

//  Optional check to see if LED output should be paused (check is bypassed if pauseDuration=0)

  int getPin();

//  Returns pin number of connected LED 

};

////////////////////////////////
//        GenericLED          //
////////////////////////////////

class GenericLED : public Blinkable {
  int pin;

  public:
   
  GenericLED(int pin) : pin{pin} {pinMode(pin,OUTPUT);digitalWrite(pin,0);}
  void on() {digitalWrite(pin,HIGH);}
  void off() {digitalWrite(pin,LOW);}
  int getPin() {return(pin);}
};
