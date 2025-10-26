/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2025 Gregg E. Berman
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

//////////////////////////////////////////////////////
//              HomeSpan Version                    //
 
#define     HS_MAJOR  2
#define     HS_MINOR  1
#define     HS_PATCH  6
#define     HS_EXTENSION  ""

//////////////////////////////////////////////////////

#ifndef ARDUINO_ARCH_ESP32
  #error ERROR: HOMESPAN IS ONLY AVAILABLE FOR ESP32 MICROCONTROLLERS!
  #include <FATAL_ERROR>
#endif

#if !(defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C6) || defined(CONFIG_IDF_TARGET_ESP32C5))
  #error ERROR: SELECTED MICROCONTROLLER NOT SUPPORTED. HOMESPAN SUPPORTS THE FOLLOWING CHIPS: ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C5, AND ESP32-C6
  #include <FATAL_ERROR>
#endif

#include <esp_arduino_version.h>

#if ESP_ARDUINO_VERSION < ESP_ARDUINO_VERSION_VAL(3, 1, 0)
  #error ERROR: THIS VERSION OF HOMESPAN REQUIRES VERSION 3.1.0 OR GREATER OF THE ARDUINO-ESP32 BOARD MANAGER 
  #include <FATAL_ERROR>
#endif

#define     STRINGIFY(x) _STR(x)
#define     _STR(x) #x

#define     HOMESPAN_VERSION    STRINGIFY(HS_MAJOR) "." STRINGIFY(HS_MINOR) "." STRINGIFY(HS_PATCH) HS_EXTENSION

#define     VERSION(major,minor,patch) major*10000+minor*100+patch

#ifndef REQUIRED
  #define REQUIRED 0
#endif

#if (REQUIRED>VERSION(HS_MAJOR,HS_MINOR,HS_PATCH))
  #error ERROR: THIS SKETCH REQUIRES A LATER VERSION OF THE HOMESPAN LIBRARY
  #include <FATAL_ERROR>
#endif

#define     ARDUINO_ESP_VERSION  STRINGIFY(ESP_ARDUINO_VERSION_MAJOR) "." STRINGIFY(ESP_ARDUINO_VERSION_MINOR) "." STRINGIFY(ESP_ARDUINO_VERSION_PATCH)
