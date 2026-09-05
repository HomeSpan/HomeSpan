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
float temp=-10.0;         // this global variable represents our "simulated" temperature (in degrees C)

//////////////////////

void setup() {

  Serial.begin(115200);
  delay(1000); 

  Serial.printf("\n\nReady.\n\n");

  SpanPoint::configure(46,{.network=4,.password="HomeSpan2"});
    
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


  if(mainDevice->get(msgData))
    Serial.printf("Message Received = '%s'\n",msgData);


}
