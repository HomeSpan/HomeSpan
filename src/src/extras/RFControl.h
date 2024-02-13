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
 
////////////////////////////////////
//       RF Control Module        //
////////////////////////////////////

#pragma once

#include <Arduino.h>
#include <soc/rmt_reg.h>
#include "driver/rmt.h"
#include <vector>

[[maybe_unused]] static const char* RFControl_TAG = "RFControl";

using std::vector;

class RFControl {
  friend class Pixel;
  
  private:
    rmt_config_t *config=NULL;
    vector<uint32_t> data;
    boolean lowWord=true;
    boolean refClock;
    static uint8_t nChannels;

    RFControl(uint8_t pin, boolean refClock, boolean installDriver);        // private constructor (only used by Pixel class)

  public:    
    RFControl(uint8_t pin, boolean refClock=true):RFControl(pin,refClock,true){};     // public constructor to create transmitter on pin, using 1-MHz Ref Tick clock or 80-MHz APB clock
    
    void start(uint32_t *data, int nData, uint8_t nCycles=1, uint8_t tickTime=1);     // starts transmission of pulses from specified data pointer, repeated for numCycles, where each tick in pulse is tickTime microseconds long
    void start(uint8_t nCycles=1, uint8_t tickTime=1);                                // starts transmission of pulses from internal data structure, repeated for numCycles, where each tick in pulse is tickTime microseconds long    
    
    void clear();                                         // clears transmitter memory
    void add(uint32_t onTime, uint32_t offTime);          // adds pulse of onTime ticks HIGH followed by offTime ticks LOW
    void phase(uint32_t nTicks, uint8_t phase);           // adds either a HIGH phase or LOW phase lasting numTicks ticks
    void enableCarrier(uint32_t freq, float duty=0.5);    // enables carrier wave if freq>0, else disables carrier wave; duty is a fraction from 0-1
    void disableCarrier(){enableCarrier(0);}              // disables carrier wave

    int getPin(){return(config?config->gpio_num:-1);}                             // returns the pin number, or -1 if no channel defined
    rmt_channel_t getChannel(){return(config?config->channel:RMT_CHANNEL_0);}     // returns channel, or channel_0 is no channel defined

    operator bool(){                                      // override boolean operator to return true/false if creation succeeded/failed
      return(config);    
    }
};

// Helper macro for creating your own storage of uint32_t data array elements - used with first variation of start() above

#define RF_PULSE(highTicks,lowTicks) (1 << 15 | uint32_t(highTicks) | uint32_t(lowTicks) << 16)
   
