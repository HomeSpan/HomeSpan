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

#include "SpanCast.h"  
  
///////////////////////////////

SpanCast *mainDevice;     // pointer to SpanCast connection to Main (HomeSpan) Device
float temp=-10.0;         // variable to store our "simulated" temperature (in degrees C)

#define THIS_DEVICE_NAME    "Indoor Temperature Sensor"

#define THIS_DEVICE_ID      46    // SpanCast Device ID configured for THIS Device
#define MAIN_DEVICE_ID      18    // SpanCast Device ID of the MAIN Device to which this device sends (and optionally receives) messages

//////////////////////

void setup() {

  Serial.begin(115200);
  delay(1000); 

  Serial.printf("\n\n%s Ready.\n\n",THIS_DEVICE_NAME);

  SpanCast::configure(THIS_DEVICE_ID,{.channelMask=SpanCast::range(1,11)});
    
  mainDevice=new SpanCast(MAIN_DEVICE_ID,sizeof(float),48);
}

//////////////////////

uint32_t aTime=0;
uint8_t msgData[48];

void loop() {

  if(millis()-aTime>5000){

    Serial.printf("Sending Temperature: %0.1f °F\n",temp*9/5+32);

    if(mainDevice->send(&temp))
      Serial.printf("Send Succeeded\n");
    else
      Serial.printf("Send Failed\n");

    temp+=0.5;       // increment the "temperature" by 0.5 C
    if(temp>35.0)
      temp=-10.0;

    aTime=millis();
  }

  if(mainDevice->get(msgData))
    Serial.printf("Message Received = '%s'\n",msgData);
}
