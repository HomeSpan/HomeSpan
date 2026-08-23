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

SpanPoint::SpAddress remoteAddress(18,4);
SpanPoint::SpAddress localAddress(46,4);

float temp=-10.0;         // this global variable represents our "simulated" temperature (in degrees C)

boolean msgReceived=false;
uint8_t buffer[128];

// Next we create a simple, standard ESP-NOW callback function to report on the status of each data transmission

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.printf("Last Packet Send Status: %02X:%02X:%02X:%02X:%02X:%02X %s\n",mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5],sendStatus==0?"Success":"Fail");
}

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

  // WiFi.mode(WIFI_AP);            // set the mode to Station
  // wifi_set_channel(1);            // you also need to manually set the channel to match whatever channel is used by the ESP32 after it connects to your WiFi network

  // wifi_set_macaddr(SOFTAP_IF, localAddress.mac);

  // // Next, initialize ESP-NOW
  
  // if (esp_now_init() != 0) {
  //   Serial.println("Error initializing ESP-NOW");
  //   return;
  // }

//  mKey = new MasterKey("HomeSpan2","SpanPoint");

  // uint8_t pmk[16];
  // mKey->create("Key for PMK",pmk,16); 
  // esp_now_set_kok(pmk,16);

  uint8_t lmk[16];
  char *lmkContext;
  asprintf(&lmkContext,"Key for LMK: NetID=%hu DevID1=%hhu DevID2=%hhu",remoteAddress.netID,
           remoteAddress.devID<(localAddress.devID)?remoteAddress.devID:localAddress.devID,
           remoteAddress.devID>(localAddress.devID)?remoteAddress.devID:localAddress.devID);

  Serial.printf("LMK Context = '%s'\n",lmkContext);

  SpanPoint::mKey->create(lmkContext,lmk,16);
  free(lmkContext);

  esp_now_register_send_cb(OnDataSent);                   // register the callback function we defined above
  esp_now_register_recv_cb(OnDataRecv);                   // register the callback function we defined above
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);         // set the role of this device to be a controller (i.e. it sends data to the ESP32)
    
  // esp_now_add_peer(remoteAddress.mac, ESP_NOW_ROLE_COMBO, 0, hash, 16);    // now we add in the peer, set its role, and specify the LMK
  esp_now_add_peer(remoteAddress.mac, ESP_NOW_ROLE_SLAVE, 0, lmk, 16);    // now we add in the peer, set its role, and specify the LMK

  // Hint:  The third argument above is the WiFi Channel.  However, this is only a reference number stored by ESP-NOW.  ESP-NOW does NOT actually set the channel for you.
  // We already set the WiFi channel above.  To make things easier, ESP-NOW allows you to set the channel as zero, which means ESP-NOW should expect the channel to be whatever was
  // already set for the WiFi controller.  Recommend always setting this to zero to avoid having any mismatches if you instead specified a real channel.
}

//////////////////////

uint32_t aTime=0;
uint8_t msgData[61];

void loop() {

 // Serial.printf("\nMAC Address: %s\n",WiFi.softAPmacAddress().c_str());         // enter this MAC address as the first argument of the matching SpanPoint object on the ESP32 running HomeSpan

  if(millis()-aTime>5000){

    Serial.printf("Sending Temperature: %f\n",temp);

    uint8_t msg[36];
    memcpy(msg,&temp,4);

    SpanPoint::localHMAC->create(msg,4,msg+4);

  //  esp_now_send(remoteAddress.mac, (uint8_t *)&temp, sizeof(temp));     // Send the Data to the Main Device!
    esp_now_send(remoteAddress.mac, msg, 36);     // Send the Data to the Main Device!

    temp+=0.5;       // increment the "temperature" by 0.5 C
    if(temp>35.0)
      temp=-10.0;

    aTime=millis();
  }

  if(getData(msgData))
    Serial.printf("Message Received = '%s'\n",msgData);

//  delay(5000);    // wait 5 seconds before sending another update
}
