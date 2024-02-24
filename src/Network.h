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

#include <WiFi.h>
#include "Settings.h"

const int MAX_SSID=32;                              // max number of characters in WiFi SSID
const int MAX_PWD=64;                               // max number of characters in WiFi Password

///////////////////////////////

struct Network {

  const int MAX_HTTP=4095;                            // max number of bytes in HTTP message

  const char *apSSID=DEFAULT_AP_SSID;                 // Access Point SSID
  const char *apPassword=DEFAULT_AP_PASSWORD;         // Access Point password (does not need to be secret - only used to ensure encrypted WiFi connection)
  unsigned long lifetime=DEFAULT_AP_TIMEOUT*1000;     // length of time (in milliseconds) to keep Access Point alive before shutting down and restarting
  
  char **ssidList=NULL;
  int numSSID;

  WiFiClient client;                      // client used for HTTP calls
  int waitTime;                           // time to wait between HTTP refreshed when checking for WiFi connection
  unsigned long alarmTimeOut;             // alarm time after which access point is shut down and HomeSpan is re-started
  int apStatus;                           // tracks access point status (0=timed-out, -1=cancel, 1=save)

  struct {
    char ssid[MAX_SSID+1]="";
    char pwd[MAX_PWD+1]="";
  } wifiData;
  
  char setupCode[8+1];  

  void scan();                                                              // scan for WiFi networks and save only those with unique SSIDs
  void serialConfigure();                                                   // configure homeSpan WiFi from serial monitor
  boolean allowedCode(char *s);                                             // checks if Setup Code is allowed (HAP defines a list of disallowed codes)
  void apConfigure();                                                       // configure homeSpan WiFi and Setup Code using temporary Captive Access Point; only returns if sucessful, else ESP restarts
  void processRequest(char *body, char *formData);                          // process the HTTP request
  int getFormValue(char *formData, const char *tag, char *value, int maxSize);    // search for 'tag' in 'formData' and copy result into 'value' up to 'maxSize' characters; returns number of characters, else -1 if 'tag' not found
  int badRequestError();                                                    // return 400 error
  
};
