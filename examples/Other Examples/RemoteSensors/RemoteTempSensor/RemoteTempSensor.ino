
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
 
////////////////////////////////////////////////////////////
//                                                        //
//    HomeSpan: A HomeKit implementation for the ESP32    //
//    ------------------------------------------------    //
//                                                        //
//     This sketch is for a Remote Temperature Sensor to  //
//     be used in conjunction with the "MainDevice.ino"   //
//     sketch running on a separate ESP32                 //
//                                                        //
//     The purpose of these sketches is to demonstrate    //
//     how to use SpanPoint() to communication between    //
//     a remote ESP32 device that takes measurements from //
//     a sensor, and a separate "main" ESP32 device that  //
//     is running the full HomeSpan code, and thus        //
//     connects to HomeKit.                               //
//                                                        //
//     This sketch implements an Adafruit ADT7410 I2C     //
//     Temperature Sensor.  If you don't have such a      //
//     device, please use the sketch "RemoteDevice.ino"   //
//     instead.  That sketch SIMULATES a temperature      //
//     sensor and therefore allows you to learn how       //
//     SpanPoint() works even though the temperature data //
//     itself isn't real.                                 //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h"
#include <Wire.h>           // include the I2C library

#define DIAGNOSTIC_MODE

#define SAMPLE_TIME   30000     // Time between temperature samples (in milliseconds)
#define I2C_ADD       0x48      // I2C Address to use for the Adafruit ADT7410

SpanPoint *mainDevice;

void setup() {

  setCpuFrequencyMhz(80);       // reduce CPU frequency to save battery power  

#if defined(DIAGNOSTIC_MODE)  
  homeSpan.setLogLevel(1);
  Serial.begin(115200);
  delay(1000);
  Serial.printf("Starting Remote Temperature Sensor.  MAC Address of this device = %s\n",WiFi.macAddress().c_str());
#endif

  // In the line below, replace the MAC Address with that of your MAIN HOMESPAN DEVICE

  mainDevice=new SpanPoint("7C:DF:A1:61:E4:A8",sizeof(float),0);    // create a SpanPoint with send size=sizeof(float) and receive size=0     

  Wire.begin();                         // start I2C in Controller Mode
  
#if defined(DIAGNOSTIC_MODE)  
  Wire.beginTransmission(I2C_ADD);      // setup transmission
  Wire.write(0x0B);                     // ADT7410 Identification Register
  Wire.endTransmission(0);              // transmit and leave in restart mode to allow reading
  Wire.requestFrom(I2C_ADD,1);          // request read of single byte
  uint8_t id = Wire.read();             // receive a byte
  LOG1("Configuring Temperature Sensor ADT7410 version 0x%02X with address 0x%02X.\n",id,I2C_ADD);           // initialization message
#endif

  Wire.beginTransmission(I2C_ADD);      // setup transmission
  Wire.write(0x03);                     // ADT740 Configuration Register
  Wire.write(0xC0);                     // set 16-bit temperature resolution, 1 sample per second
  Wire.endTransmission();               // transmit
          
  Wire.beginTransmission(I2C_ADD);      // setup transmission
  Wire.write(0x00);                     // ADT7410 2-byte Temperature
  Wire.endTransmission(0);              // transmit and leave in restart mode to allow reading
  Wire.requestFrom(I2C_ADD,2);          // request read of two bytes

  int16_t iTemp = ((int16_t)Wire.read()<<8)+Wire.read();    
  float temperature = iTemp/128.0;
  
  boolean success = mainDevice->send(&temperature);                 // send temperature to main device
  
  LOG1("Send temp update of %0.2f F: %s\n",temperature*9/5+32,success?"Succeded":"Failed");  

  esp_deep_sleep(SAMPLE_TIME*1000);     // enter deep sleep mode -- reboot after resuming
}
