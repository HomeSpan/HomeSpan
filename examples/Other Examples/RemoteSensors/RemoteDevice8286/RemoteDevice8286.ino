/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2023 Gregg E. Berman
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
#include <Crypto.h>       // this library is needed to implement the hash-code process SpanPoint uses to generate ESP-NOW encryption keys

float temp=-10.0;         // this global variable represents our "simulated" temperature (in degrees C)

// Below we encode the MAC Address of the Main ESP32 Device running HomeSpan to which this ESP8266 device will connect

// IMPORTANT:  ESP32 devices have TWO MAC Addresses.  One is used when the ESP32 is operating in Station (STA) mode. It is the address returned
// by the WiFi.macAddress() function.  The other is used when the ESP32 is operating in Access Point (AP) mode.  This address is returned by the 
// WiFi.softAPmacAddress() function.  HomeSpan normally operates the ESP32 with both modes (STA+AP), so both MAC Addresses are active.

// On ESP32 devices, ESP-NOW seems to work fine when each device sends data to other devices via their STA MAC Address.  The same is true for ESP8266
// devices sending data to an ESP32 device via ESP-NOW with one critical exception: Once the ESP32 connects (via STA mode) to a WiFi network, which it must 
// do to run HomeSpan, for some reason ESP8266 devices can no longer send data via ESP-NOW to the ESP32 using its STA MAC Address.

// The solution is to instead have the ESP8266 send data via ESP-NOW to the ESP32's AP MAC Address.  This seems to work regardless of whether or not
// the ESP32 is connected to a central WiFi newtork.  To support such use on the ESP32, the SpanPoint constructor includes a fifth, optional parameter
// called "useAPaddress".  When creating SpanPoint links of the ESP32 using HomeSpan, set useAPaddress to TRUE if the Remote Device SpanPoint is connecting
// to is an ESP8266.  Set "useAPaddress" to FALSE (or leave unspecified, since FALSE is the default) if the Remote Device is an ESP32.

// When HomeSpan first starts (and whenever you type 'i' into the CLI), the Serial Monitor will display the details of each SpanPoint object you instantiated
// in your ESP32 sketch.  This  output includes the MAC Address at which SpanPoint will be listening for incoming data from Remote Devices.  The MAC Address
// shown for the instance of SpanPoint corresponding to this Remote Deivce (i.e. this sketch) is the MAC Address you should use below.
 
uint8_t main_mac[6]={0x84,0xCC,0xA8,0x11,0xB4,0x85};        // this is the **AP MAC Address** of the Main Device running HomeSpan on an ESP32 as reported in the HomeSpan Serial Monitor

// Next we create a simple, standard ESP-NOW callback function to report on the status of each data transmission

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.printf("Last Packet Send Status: %s\n",sendStatus==0?"Success":"Fail");
}

//////////////////////

void setup() {

  Serial.begin(115200);
  delay(1000); 
  Serial.printf("\nMAC Address: %s\n",WiFi.macAddress().c_str());         // enter this MAC address as the first argument of the matching SpanPoint object on the ESP32 running HomeSpan
  
  WiFi.mode(WIFI_STA);            // set the mode to Station
  wifi_set_channel(6);            // you also need to manually set the channel to match whatever channel is used by the ESP32 after it connects to your WiFi network

  // Hint: As an alterntive, you can add code to this sketch to connect to the same WiFi network that HomeSpan uses.  Though this sketch won't make any use of that WiFi network,
  // by establishing the connection the ESP8266 automatically configures the channel, which will now match the ESP32.

  // Next, initialize ESP-NOW
  
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // SpanPoint uses ESP-NOW encryption for all communication.  This encrpytion is based on two 16-byte keys: a local master key (LMK) and a primary master key (PMK).  To generate
  // these keys, SpanPoint takes a text-based password (the default is the word "HomeSpan"), creates a 32 byte (256 bit) hash of the text (using the SHA256 method), and uses
  // the first 16 bytes as the LMK and the last 16 bytes as the PMK.  This is easily replicated as follows:
  
  uint8_t hash[32];                 // create space to store as 32-byte hash code
  char password[]="HomeSpan";       // specify the password
  
  experimental::crypto::SHA256::hash(password,strlen(password),hash);     // create the hash code to be used further below

  esp_now_register_send_cb(OnDataSent);                   // register the callback function we defined above
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);         // set the role of this device to be a controller (i.e. it sends data to the ESP32)

  esp_now_set_kok(hash+16,16);        // next we set the PMK.  For some reason this is called KOK on the ESP8266.  Note you must set the PMK BEFORE adding any peers
        
  esp_now_add_peer(main_mac, ESP_NOW_ROLE_COMBO, 0, hash, 16);    // now we add in the peer, set its role, and specify the LMK

  // Hint:  The third argument above is the WiFi Channel.  However, this is only a reference number stored by ESP-NOW.  ESP-NOW does NOT actually set the channel for you.
  // We already set the WiFi channel above.  To make things easier, ESP-NOW allows you to set the channel as zero, which means ESP-NOW should expect the channel to be whatever was
  // already set for the WiFi controller.  Recommend always setting this to zero to avoid having any mismatches if you instead specified a real channel.
}

//////////////////////

void loop() {

  Serial.printf("Sending Temperature: %f\n",temp);  
  esp_now_send(main_mac, (uint8_t *)&temp, sizeof(temp));     // Send the Data to the Main Device!

  temp+=0.5;       // increment the "temperature" by 0.5 C
  if(temp>35.0)
    temp=-10.0;

  delay(5000);    // wait 5 seconds before sending another update
}
