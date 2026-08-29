/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2026 Gregg E. Berman
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

#ifndef ARDUINO_ARCH_ESP8266
#error ERROR: THIS SKETCH IS DESIGNED FOR ESP8266 MICROCONTROLLERS!
#endif

#include "SpanPoint.h"  
  
///////////////////////////////

SpanPoint *mainDevice;

SpanPoint::SpAddress remoteAddress(18,4);
SpanPoint::SpAddress localAddress(46,4);

float temp=-10.0;         // this global variable represents our "simulated" temperature (in degrees C)

boolean msgReceived=false;
uint8_t buffer[128];

void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {

  if(msgReceived)
    return;

  const SpanPoint::SpAddress *srcAddress = (SpanPoint::SpAddress *)mac_addr;

  Serial.printf("SpanPoint: ");

  if(!srcAddress->isValid()){
    Serial.printf("WARNING! Ignoring %d-byte message received from invalid SpanPoint MAC Address %02X:%02X:%02X:%02X:%02X:%02X.\n",len,
        mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
    return;
  }

  HMAC remoteHMAC(SpanPoint::mKey,mac_addr,6);
  if(!remoteHMAC.verify(incomingData, len)){
    Serial.printf("ERROR! Received unverifiable message of %d bytes from node %d.\n",len,srcAddress->devID);
    return;
  }

  len-=32;
  Serial.printf("Received %d verified bytes from node %d.\n",len,srcAddress->devID);
  memcpy(buffer,incomingData,len);
  msgReceived=true;
}

boolean getData(uint8_t *data){
  if(msgReceived){
    memcpy(data,buffer,61);
    msgReceived=false;
    return(true);
  }
  return(false);
}

//////////////////////

void setup() {

  Serial.begin(115200);
  delay(1000); 

  SpanPoint::configure(46,{.network=4,.password="HomeSpan2"});

  Serial.printf("\n\nREMOTE ADDRESS = %02X:%02X:%02X:%02X:%02X:%02X\n",remoteAddress.mac[0],remoteAddress.mac[1],remoteAddress.mac[2],remoteAddress.mac[3],remoteAddress.mac[4],remoteAddress.mac[5]);
  Serial.printf("REMOTE DEVID=%hhu  NETID=%hu\n",remoteAddress.devID,remoteAddress.netID);

  esp_now_register_recv_cb(OnDataRecv);                   // register the callback function we defined above
    
  mainDevice=new SpanPoint(18,sizeof(float),61);
}

//////////////////////

uint32_t aTime=0;
uint8_t msgData[61];

void loop() {

  if(millis()-aTime>5000){

    Serial.printf("Sending Temperature: %f\n",temp);

    mainDevice->send(&temp);

    temp+=0.5;       // increment the "temperature" by 0.5 C
    if(temp>35.0)
      temp=-10.0;

    aTime=millis();
  }

  if(getData(msgData))
    Serial.printf("Message Received = '%s'\n",msgData);

}
