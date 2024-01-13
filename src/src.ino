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

#define MAX_LIGHTS  2

void setup() {
 
  Serial.begin(115200);

  homeSpan.setLogLevel(1).setControlPin(21,SpanButton::TRIGGER_ON_HIGH).setStatusPin(13);
  homeSpan.enableWebLog(50,"pool.ntp.org","UTC");  

  homeSpan.begin(Category::Lighting,"HomeSpan Max");

   new SpanAccessory();
    new Service::AccessoryInformation();  
      new Characteristic::Identify();

  for(int i=0;i<MAX_LIGHTS;i++){
    new SpanAccessory();
      new Service::AccessoryInformation();
        new Characteristic::Identify();
        char c[100];
        sprintf(c,"Light-%d",i);
        new Characteristic::Name(c);
      new Service::LightBulb();
        new Characteristic::On(0,true);
        new Characteristic::Saturation(0,true);
     WEBLOG("Configuring %s\n",c);
  }

  new SpanUserCommand('w', " - get web log test",webLogTest);     // simulate getting an HTTPS request for weblog

//  homeSpan.autoPoll(8192+10000);
}

//////////////////////////////////////

void loop(){
 
  homeSpan.poll();
  
}

//////////////////////////////////////

void webLogTest(const char *dummy){
  Serial.printf("\n*** In Web Log Test.  Starting Custom Web Log Handler\n");     // here is where you would perform any HTTPS initializations   
  homeSpan.getWebLog(webLogHandler);
}

void webLogHandler(const char *buf){
  if(buf!=NULL)
    Serial.printf("%s",buf);            // here is where you would transmit data to the HTTPS connection
  else
    Serial.print("*** DONE!\n\n");      // here is where you would close the HTTPS connection
}
