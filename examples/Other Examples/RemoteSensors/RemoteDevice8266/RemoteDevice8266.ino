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

// *** THIS SKETCH IS FOR AN ESP8266, NOT AN ESP32 *** //

// This sketch is similar to HomeSpan's RemoteDevice.ino example (designed for an ESP32 running HomeSpan) in which we simulate
// a Remote Temperature Sensor using HomeSpan's SpanPoint class.  However, since neither HomeSpan nor SpanPoint is designed to
// run on an ESP8266, we will implement the BASIC communication functionality of SpanPoint by directly calling the equivalent
// ESP-NOW commands that are supported by the ESP8266.  This sketch does NOT seek to replicate all of SpanPoint's features, and
// does not include automatic channel calibration or queue management.

// Start by including the following ESP8266 libraries

#include <ESP8266WiFi.h>                 
#include <espnow.h>
#include <bearssl/bearssl_kdf.h>

class KeyGen {

  private:
  
  br_hkdf_context masterContext;

  public:

  KeyGen(const char *password, const char *salt){

	  br_hkdf_init(&masterContext, &br_sha256_vtable, salt, strlen(salt));
    br_hkdf_inject(&masterContext, password, strlen(password));
    br_hkdf_flip(&masterContext);
  }

  void extract(const char *keyInfo, uint8_t *keyOutput, size_t outLen){

    extract(keyInfo,strlen(keyInfo),keyOutput,outLen);   
  }

  void extract(const void *keyInfo, size_t inLen, uint8_t *keyOutput, size_t outLen){
  
    br_hkdf_context tempContext=masterContext;  
    br_hkdf_produce(&tempContext, keyInfo, inLen, keyOutput, outLen);

    for(int i=0;i<outLen;i++)
      Serial.printf("%02X%s",keyOutput[i],i%4==3?" ":"");
    Serial.printf("\n\n");    
  }  
};

float temp=-10.0;         // this global variable represents our "simulated" temperature (in degrees C)

struct SpAddress {

  union {
    struct {
      uint8_t firstByte=0xF2;
      uint8_t devID;
      uint16_t netID;
      uint16_t checkSum;
    };
    uint8_t mac[6];
  };

  SpAddress(uint8_t deviceID, uint16_t networkID){
    devID=deviceID;
    netID=networkID;
    mac[4]=mac[0]^mac[2];
    mac[5]=mac[1]^mac[3];
  }
};

SpAddress remoteAddress(18,4);
SpAddress localAddress(46,4);


// Next we create a simple, standard ESP-NOW callback function to report on the status of each data transmission

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.printf("Last Packet Send Status: %02X:%02X:%02X:%02X:%02X:%02X %s\n",mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5],sendStatus==0?"Success":"Fail");
}

void onDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {
  Serial.printf("Received %hhu bytes from: %02X:%02X:%02X:%02X:%02X:%02X\n",len,mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
}

struct SpConfig_t {
  uint16_t network=1;
  boolean channelSelector=true;
  String password="HomeSpan";
  boolean encrypt=true;
  uint16_t channelMask=0x3FFE;
} spConf;

//////////////////////

void setup() {

  Serial.begin(115200);
  delay(1000); 

  Serial.printf("\n\nREMOTE ADDRESS = %02X:%02X:%02X:%02X:%02X:%02X\n",remoteAddress.mac[0],remoteAddress.mac[1],remoteAddress.mac[2],remoteAddress.mac[3],remoteAddress.mac[4],remoteAddress.mac[5]);
  Serial.printf("REMOTE DEVID=%hhu  NETID=%hu\n",remoteAddress.devID,remoteAddress.netID);

  WiFi.mode(WIFI_AP);            // set the mode to Station
  wifi_set_channel(1);            // you also need to manually set the channel to match whatever channel is used by the ESP32 after it connects to your WiFi network

  wifi_set_macaddr(SOFTAP_IF, localAddress.mac);

  // Next, initialize ESP-NOW
  
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  KeyGen mKey("HomeSpan","SpanPoint");

  uint8_t pmk[16];
  mKey.extract("Key for PMK",pmk,16); 
  esp_now_set_kok(pmk,16);

  uint8_t lmk[16];
  char *lmkContext;
  asprintf(&lmkContext,"Key for LMK: NetID=%hu DevID1=%hhu DevID2=%hhu",remoteAddress.netID,
           remoteAddress.devID<(localAddress.devID)?remoteAddress.devID:localAddress.devID,
           remoteAddress.devID>(localAddress.devID)?remoteAddress.devID:localAddress.devID);

  Serial.printf("LMK Context = '%s'\n",lmkContext);

  mKey.extract(lmkContext,lmk,16);
  free(lmkContext);

  uint8_t authKey[32];
  mKey.extract(localAddress.mac,6,authKey,32);

  esp_now_register_send_cb(OnDataSent);                   // register the callback function we defined above
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);         // set the role of this device to be a controller (i.e. it sends data to the ESP32)
    
  // esp_now_add_peer(remoteAddress.mac, ESP_NOW_ROLE_COMBO, 0, hash, 16);    // now we add in the peer, set its role, and specify the LMK
  esp_now_add_peer(remoteAddress.mac, ESP_NOW_ROLE_SLAVE, 0, lmk, 16);    // now we add in the peer, set its role, and specify the LMK

  // Hint:  The third argument above is the WiFi Channel.  However, this is only a reference number stored by ESP-NOW.  ESP-NOW does NOT actually set the channel for you.
  // We already set the WiFi channel above.  To make things easier, ESP-NOW allows you to set the channel as zero, which means ESP-NOW should expect the channel to be whatever was
  // already set for the WiFi controller.  Recommend always setting this to zero to avoid having any mismatches if you instead specified a real channel.
}

//////////////////////

void loop() {

  Serial.printf("\nMAC Address: %s\n",WiFi.softAPmacAddress().c_str());         // enter this MAC address as the first argument of the matching SpanPoint object on the ESP32 running HomeSpan

  Serial.printf("Sending Temperature: %f\n",temp);  
  esp_now_send(remoteAddress.mac, (uint8_t *)&temp, sizeof(temp));     // Send the Data to the Main Device!

  temp+=0.5;       // increment the "temperature" by 0.5 C
  if(temp>35.0)
    temp=-10.0;

  delay(5000);    // wait 5 seconds before sending another update
}
