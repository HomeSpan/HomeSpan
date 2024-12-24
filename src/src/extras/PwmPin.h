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
 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                ----- PWM Pin Control -----
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Wrappers around the ESP-IDF ledc library to control PWM-based devices:
//
// LedPin(pin) - controls a Dimmable LED on specified pin with frequency=5000 Hz
//             - use set(level) to set brightness from 0-100%
//
// ServoPin(pin) - controls a Servo Motor on specified pin with frequency=50 Hz
//               - use set(degrees) to set position to degrees
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Arduino.h>
#include <driver/ledc.h>
#include "Blinker.h"

[[maybe_unused]] static const char* PWM_TAG = "PwmPin";

#define DEFAULT_PWM_FREQ     5000

/////////////////////////////////////

class LedC {

  protected:
    static ledc_channel_config_t *channelList[LEDC_CHANNEL_MAX][LEDC_SPEED_MODE_MAX];
    static ledc_timer_config_t *timerList[LEDC_TIMER_MAX][LEDC_SPEED_MODE_MAX];
    
    ledc_channel_config_t *channel=NULL;
    ledc_timer_config_t *timer;
    
    LedC(uint8_t pin, uint16_t freq, boolean invert=false);

  public:
    int getPin(){return(channel?channel->gpio_num:-1);}               // returns the pin number

    operator bool(){         // override boolean operator to return true/false if creation succeeded/failed
      return(channel);    
    }    
    
};
  
/////////////////////////////////////

class LedPin : public LedC {

  public:
    enum {
      NOT_FADING,
      COMPLETED,
      FADING
    };

    enum {
      ABSOLUTE,
      PROPORTIONAL
    };

  private:
    int fadeState=NOT_FADING;
    static bool fadeCallback(const ledc_cb_param_t *param, void *arg);
    static boolean fadeInitialized;

  public:
    LedPin(uint8_t pin, float level=0, uint16_t freq=DEFAULT_PWM_FREQ, boolean invert=false);   // assigns LED pin
    void set(float level);                                                                      // sets the PWM duty to level (0-100)
    int fade(float level, uint32_t fadeTime, int fadeType=ABSOLUTE);                            // sets the PWM duty to level (0-100) within fadeTime in milliseconds, returns success (0) or fail (1)
    int fadeStatus();                                                                           // returns fading state
    
    static void HSVtoRGB(float h, float s, float v, float *r, float *g, float *b );       // converts Hue/Saturation/Brightness to R/G/B
};



////////////////////////////////
//         ServoPin           //
////////////////////////////////

class ServoPin : public LedC {
  uint16_t minMicros;
  uint16_t maxMicros;
  double minDegrees;
  double microsPerDegree;

  public:
    ServoPin(uint8_t pin, double initDegrees, uint16_t minMicros, uint16_t maxMicros, double minDegrees, double maxDegrees);
    ServoPin(uint8_t pin, double initDegrees=0) : ServoPin(pin,initDegrees,1000,2000,-90,90) {};
    
    void set(double degrees);                               // sets the Servo to degrees, where degrees is bounded by [minDegrees,maxDegrees]  
};
