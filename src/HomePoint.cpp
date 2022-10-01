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

#include "HomeSpan.h"
#include <mbedtls/sha256.h>
#include <esp_wifi.h>

SpanPoint::SpanPoint(const char *macAddress, int sendSize, int receiveSize, int queueDepth){

  if(sscanf(macAddress,"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",peerInfo.peer_addr,peerInfo.peer_addr+1,peerInfo.peer_addr+2,peerInfo.peer_addr+3,peerInfo.peer_addr+4,peerInfo.peer_addr+5)!=6){
    Serial.printf("\nFATAL ERROR!  Can't create new SpanPoint(\"%s\") - Invalid MAC Address ***\n",macAddress);
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }

  if(sendSize<0 || sendSize>200 || receiveSize<0 || receiveSize>200 || queueDepth<1 || (sendSize==0 && receiveSize==0)){
    Serial.printf("\nFATAL ERROR!  Can't create new SpanPoint(\"%s\",%d,%d,%d) - one or more invalid parameters ***\n",macAddress,sendSize,receiveSize,queueDepth);
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }

  this->sendSize=sendSize;
  this->receiveSize=receiveSize;

  Serial.printf("SpanPoint: Created link to device with MAC Address %02X:%02X:%02X:%02X:%02X:%02X.  Send size=%d bytes, Receive size=%d bytes with queue depth=%d.\n",
    peerInfo.peer_addr[0],peerInfo.peer_addr[1],peerInfo.peer_addr[2],peerInfo.peer_addr[3],peerInfo.peer_addr[4],peerInfo.peer_addr[5],sendSize,receiveSize,queueDepth);
  
  init();                             // initialize SpanPoint
  peerInfo.channel=0;                 // 0 = matches current WiFi channel
  peerInfo.ifidx=WIFI_IF_STA;         // must specify interface
  peerInfo.encrypt=true;              // turn on encryption for this peer
  memcpy(peerInfo.lmk, lmk, 16);      // set local key
  esp_now_add_peer(&peerInfo);        // add peer to ESP-NOW

  if(receiveSize>0)
    receiveQueue = xQueueCreate(queueDepth,receiveSize);  

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
  esp_now_register_send_cb(dataSent);       // set callback for sending data
  
  statusQueue = xQueueCreate(1,sizeof(esp_now_send_status_t));    // create statusQueue even if not needed
  setChannelMask(0x3FFE);                                         // default channel mask uses channels 1-13  

  initialized=true;
}

///////////////////////////////

void SpanPoint::setChannelMask(uint16_t mask){
  channelMask = mask & 0x3FFE;

  channel=0;

  for(int i=1;i<=13 && channel==0;i++)
    channel=(channelMask & (1<<i))?i:0;

  if(channel==0){
    Serial.printf("\nFATAL ERROR!  SpanPoint::setChannelMask(0x%04X) - one or more invalid parameters ***\n",mask);
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }

  if(!isHub)                                                      // if this is NOT the main HomeSpan device
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);         // set the WiFi channel 

  Serial.printf("ChannelMask=0x%04X => channel=%d\n",channelMask,channel);
}

///////////////////////////////

boolean SpanPoint::get(void *dataBuf){

  if(receiveSize==0)
    return(false);

  return(xQueueReceive(receiveQueue, dataBuf, 0));
}

///////////////////////////////

boolean SpanPoint::send(void *data){

  if(sendSize==0)
    return(false);
  
  esp_now_send_status_t status = ESP_NOW_SEND_FAIL;

  for(int c=0;c<13;c++){
    if((1<<channel) & channelMask){
      for(int i=1;i<=3;i++){
        Serial.printf("Sending on channel %d, attempt #%d\n",channel,i);
        esp_now_send(peerInfo.peer_addr, (uint8_t *) data, sendSize);
        xQueueReceive(statusQueue, &status, pdMS_TO_TICKS(2000));
        if(status==ESP_NOW_SEND_SUCCESS)
          return(true);
        delay(10);
      }
    }
    channel++;
    if(channel==14)
      channel=1;
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  }
  return(false);
} 

///////////////////////////////

void SpanPoint::setAsHub(){
  
  if(SpanPoints.size()>0){
    Serial.printf("\nFATAL ERROR!  SpanPoint objects created in main hub device must be instantiated AFTER calling homeSpan.begin() ***\n");
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }
  
  isHub=true;
}

///////////////////////////////

void SpanPoint::dataReceived(const uint8_t *mac, const uint8_t *incomingData, int len){
  
  auto it=SpanPoints.begin();
  for(;it!=SpanPoints.end() && memcmp((*it)->peerInfo.peer_addr,mac,6)!=0; it++);
  
  if(it==SpanPoints.end())
    return;

  if((*it)->receiveSize==0)
    return;

  if(len!=(*it)->receiveSize){
    Serial.printf("SpanPoint Warning! %d bytes received from %02X:%02X:%02X:%02X:%02X:%02X does not match %d-byte queue size\n",len,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],(*it)->receiveSize);
    return;
  }

  xQueueSend((*it)->receiveQueue, incomingData, 0);        // send to queue - do not wait if queue is full and instead fail immediately since we need to return from this function ASAP
}

///////////////////////////////

uint8_t SpanPoint::lmk[16];
boolean SpanPoint::initialized=false;
boolean SpanPoint::isHub=false;
vector<SpanPoint *> SpanPoint::SpanPoints;
int SpanPoint::channel;
uint16_t SpanPoint::channelMask;
QueueHandle_t SpanPoint::statusQueue;
