/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2023 Gregg E. Berman
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

void setup() {
 
  Serial.begin(115200);

  homeSpan.setLogLevel(2)
           .enableWebLog(500,"pool.ntp.org","UTC")
           .setWifiCallback(wifiEstablished)
           ;

  homeSpan.begin(Category::Lighting,"HomeSpan Max");

  ps_new(SpanAccessory)();
    ps_new(Service::AccessoryInformation)();  
      ps_new(Characteristic::Identify)();

  for(int i=0;i<100;i++){
    ps_new(SpanAccessory)();
      ps_new(Service::AccessoryInformation)();  
        ps_new(Characteristic::Identify)();
        char c[30];
        sprintf(c,"Light-%d",i);
        ps_new(Characteristic::Name)(c);
      ps_new(Service::LightBulb)();
        ps_new(Characteristic::On)();
        ps_new(Characteristic::Brightness)(50,false);
  }

}

//////////////////////////////////////

void loop(){
 
  homeSpan.poll();
  
}

//////////////////////////////////////

void wifiEstablished(){

  for(int i=0;i<600;i++){
  WEBLOG("Here is a lot of log file text that should take up a lot of space: %d",i);
  delay(30);
}

}
