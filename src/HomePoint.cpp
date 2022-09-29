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

#include "HomePoint.h"
#include <WiFi.h>
#include <mbedtls/sha256.h>

SpanPoint::SpanPoint(const char *macAddress, int qLength, int nItems){

  if(sscanf(macAddress,"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",peerInfo.peer_addr,peerInfo.peer_addr+1,peerInfo.peer_addr+2,peerInfo.peer_addr+3,peerInfo.peer_addr+4,peerInfo.peer_addr+5)!=6){
    Serial.printf("\nFATAL ERROR!  Can't create new SpanPoint(\"%s\") - Invalid MAC Address ***\n",macAddress);
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }

  init();                             // initialize SpanPoint
  peerInfo.channel=0;                 // 0 = matches current WiFi channel
  peerInfo.ifidx=WIFI_IF_STA;         // must specify interface
  peerInfo.encrypt=true;              // turn on encryption for this peer
  memcpy(peerInfo.lmk, lmk, 16);      // set local key
  esp_now_add_peer(&peerInfo);        // add peer to ESP-NOW

  this->qLength=qLength;
  dataQueue = xQueueCreate(nItems,qLength);  

  SpanPoints.push_back(this);             
}

///////////////////////////////

void SpanPoint::init(const char *password){

  if(initialized)
    return;

  if(WiFi.getMode()!=WIFI_AP_STA)
    WiFi.mode(WIFI_AP_STA);                 // set mode to mixed AP/STA.  This does not start any servers, just configures the WiFi radio to ensure it does not sleep (required for ESP-NOW)
  
  uint8_t hash[32];
  mbedtls_sha256_ret((const unsigned char *)password,strlen(password),hash,0);      // produce 256-bit bit hash from password

  esp_now_init();                           // initialize ESP-NOW
  memcpy(lmk, hash, 16);                    // store first 16 bytes of hash for later use as local key
  esp_now_set_pmk(hash+16);                 // set hash for primary key using last 16 bytes of hash
  esp_now_register_recv_cb(dataReceived);   // set callback for receiving data

  initialized=true;
}

///////////////////////////////

boolean SpanPoint::get(void *dataBuf){

  return(xQueueReceive(dataQueue, dataBuf, 0));
}

///////////////////////////////

void SpanPoint::dataReceived(const uint8_t *mac, const uint8_t *incomingData, int len){
  
  auto it=SpanPoints.begin();
  for(;it!=SpanPoints.end() && memcmp((*it)->peerInfo.peer_addr,mac,6)!=0; it++);
  
  if(it==SpanPoints.end())
    return;

  if(len!=(*it)->qLength){
    Serial.printf("SpanPoint Warning! %d bytes received from %02X:%02X:%02X:%02X:%02X:%02X does not match %d-byte queue size\n",len,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],(*it)->qLength);
    return;
  }

  xQueueSend((*it)->dataQueue, incomingData, pdMS_TO_TICKS(1000));  
}

///////////////////////////////

uint8_t SpanPoint::lmk[16];
boolean SpanPoint::initialized=false;
vector<SpanPoint *> SpanPoint::SpanPoints;
