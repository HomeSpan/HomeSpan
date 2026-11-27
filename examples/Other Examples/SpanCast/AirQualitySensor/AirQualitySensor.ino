/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2026 Gregg E. Berman
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

///////////

void setup(){

  struct SensorData {
    uint8_t quality;
    float o3;
    float no2;
    float so2;
    float pm25;
    float pm10;
    float voc;
  } sensorData;

  SpanCast::configure(2, {.channelMask=SpanCast::range(1,11)} );
  SpanCast station(1,sizeof(sensorData),0);

  sensorData.quality=random(1,6);
  sensorData.o3=random(0,100);
  sensorData.no2=random(100,300);
  sensorData.so2=random(0,500);
  sensorData.pm25=random(200,800);
  sensorData.pm10=random(50,250);
  sensorData.voc=random(300,600);

  station.send(&sensorData);

  esp_deep_sleep(30*1000*1000);     // enter deep sleep mode for 5 seconds -- reboot after resuming

}

///////////

void loop(){}

