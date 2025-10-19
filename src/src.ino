/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2025 Gregg E. Berman
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
  delay(2000);

  homeSpan.setLogLevel(2);
  homeSpan.enableOTA();
  homeSpan.enableWebLog(50).setWebLogFavicon();
  homeSpan.setCompileTime();

 homeSpan.setConnectionTimes(30,60,5);

  homeSpan.addBssidName("34:98:B5:DB:3E:C0","Great Room")
          .addBssidName("3A:98:B5:db:53:5e","Upstairs Hallway")
          .addBssidName("3A:98:B5:EF:BF:69","Kitchen")
          .addBssidName("3A:98:B5:DB:54:86","Basement");

  // WiFi.STA.begin();
  // WiFi.setBandMode(WIFI_BAND_MODE_5G_ONLY);        
             
  homeSpan.begin(Category::Lighting,"HomeSpan Basic");


  new SpanAccessory();
    new Service::AccessoryInformation();  
      new Characteristic::Identify();
    new Service::LightBulb();
      new Characteristic::On();

  homeSpan.autoPoll();
}
