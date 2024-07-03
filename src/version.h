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

//////////////////////////////////////////////////////
//              HomeSpan Version                    //
 
#define     HS_MAJOR  1
#define     HS_MINOR  9
#define     HS_PATCH  1

//////////////////////////////////////////////////////

#ifndef ARDUINO_ARCH_ESP32
  #error ERROR: HOMESPAN IS ONLY AVAILABLE FOR ESP32 MICROCONTROLLERS!
  #include <FATAL_ERROR>
#endif

#include <esp_arduino_version.h>

#if ESP_ARDUINO_VERSION_MAJOR!=2
  #error ERROR: HOMESPAN REQUIRES VERSION 2 OF THE ARDUINO ESP32 LIBRARY.  HOMESPAN IS NOT COMPATIBLE WITH VERSION 1 OR VERSION 3
  #include <FATAL_ERROR>
#endif

#define     STRINGIFY(x) _STR(x)
#define     _STR(x) #x

#define     HOMESPAN_VERSION    STRINGIFY(HS_MAJOR) "." STRINGIFY(HS_MINOR) "." STRINGIFY(HS_PATCH)

#define     VERSION(major,minor,patch) major*10000+minor*100+patch

#ifndef REQUIRED
  #define REQUIRED 0
#endif

#if (REQUIRED>VERSION(HS_MAJOR,HS_MINOR,HS_PATCH))
  #error ERROR: THIS SKETCH REQUIRES A LATER VERSION OF THE HOMESPAN LIBRARY
  #include <FATAL_ERROR>
#endif

#define     ARDUINO_ESP_VERSION  STRINGIFY(ESP_ARDUINO_VERSION_MAJOR) "." STRINGIFY(ESP_ARDUINO_VERSION_MINOR) "." STRINGIFY(ESP_ARDUINO_VERSION_PATCH)
