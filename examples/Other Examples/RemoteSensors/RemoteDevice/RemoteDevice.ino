
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
//     Demonstrates how to use SpanPoint() to implement   //
//     two remote temperature sensors on separate ESP32   //
//     devices.                                           //
//                                                        //
//     This sketch is for the REMOTE DEVICES.  They are   //
//     very simple and don't need any of the normal       //
//     HomeSpan logic (except for SpanPoint).             //
//                                                        //
//     Note this sketch only SIMULATES a temperature      //
//     sensor by slowly setting the temperature from      //
//     -30.0 to 35.0 C in steps of 0.5 C.  The sketch     //
//     does not contain logic for an actual physical      //
//     temperature sensor.                                //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h"

float temperature=-10.0;
SpanPoint *mainDevice;

void setup() {
  
  Serial.begin(115200);
  delay(1000);

  Serial.printf("\n\nThis is a REMOTE Device with MAC Address = %s\n",Network.macAddress().c_str());
  Serial.printf("NOTE: This MAC Address must be entered into the corresponding SpanPoint() call of the MAIN Device.\n\n");

  // In the line below, replace the MAC Address with that of your MAIN HOMESPAN DEVICE

  mainDevice=new SpanPoint("AC:67:B2:77:42:20",sizeof(float),0);    // create a SpanPoint with send size=sizeof(float) and receive size=0     

  homeSpan.setLogLevel(1);
}

void loop() {

  boolean success = mainDevice->send(&temperature);                 // this will show as success as long as the MAIN DEVICE is running
  Serial.printf("Send %s\n",success?"Succeded":"Failed");
  temperature+=0.5;
  if(temperature>35.0)
    temperature=-30.0;
   
  delay(20000);
}
