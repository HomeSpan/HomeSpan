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

#include <Arduino.h>
#include <esp_now.h>
#include <vector>

using std::vector;

class SpanPoint {

  int qLength;                                // length of 1 queue item (in bytes)
  esp_now_peer_info_t peerInfo;               // structure for all ESP-NOW peer data
  QueueHandle_t dataQueue;                    // queue to store data after it is received
  
  static uint8_t lmk[16];
  static boolean initialized;
  static vector<SpanPoint *> SpanPoints;
  
  static void dataReceived(const uint8_t *mac, const uint8_t *incomingData, int len);
  static void init(const char *password="HomeSpan");

  public:

  SpanPoint(const char *macAddress, int qLength, int nItems=1);
  static void setPassword(const char *pwd){init(pwd);};      
  boolean get(void *dataBuf);
};

///////////////////////////////
