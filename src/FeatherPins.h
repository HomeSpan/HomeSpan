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

// For developer use and testing only - provides a common set of pin numbers mapped to the Adafruit Feather Board
// Facilitates the testing of identical code on an ESP32, ESP32-S2, and ESP32-C3 using a common jig without rewiring

#pragma once

#if defined(CONFIG_IDF_TARGET_ESP32)
  enum {F13=13,F12=12,F27=27,F33=33,F15=15,F32=32,F14=14,F22=22,F23=23,F26=26,F25=25,F34=34,F39=39,F36=36,F4=4,F5=5,F18=18,F19=19,F16=16,F17=17,F21=21};
  #define DEVICE_SUFFIX ""

#elif defined(CONFIG_IDF_TARGET_ESP32S2)
  enum {F13=11,F12=10,F27=7,F33=3,F15=1,F32=38,F14=33,F22=9,F23=8,F26=17,F25=18,F34=14,F39=12,F36=6,F4=5,F5=36,F18=35,F19=37,F16=44,F17=43};
  #define DEVICE_SUFFIX "-S2"

#elif defined(CONFIG_IDF_TARGET_ESP32C3)
  enum {F27=2,F33=7,F32=3,F14=10,F22=9,F23=8,F26=0,F25=1,F4=18,F5=4,F18=6,F19=5,F16=20,F17=21,F21=19};
  #define DEVICE_SUFFIX "-C3"

#endif
