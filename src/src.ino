/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2024 Gregg E. Berman
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


CUSTOM_CHAR(TestChar,3F4F,PR+PW,UINT8,20,0,100,false)
CUSTOM_CHAR_STRING(TestString,3F45,PR+EV,"Hello");
CUSTOM_CHAR_TLV8(TestTLV,45674F457,PW+PR);
CUSTOM_CHAR_DATA(TestData,303,PW+PW);

void setup() {

  Serial.begin(115200);
 
  homeSpan.setLogLevel(2);
  homeSpan.enableWebLog();

  homeSpan.begin(Category::Lighting,"HomeSpan LightBulb");

  new SpanUserCommand('D', " - disconnect WiFi", [](const char *buf){WiFi.disconnect();});  
  
  new SpanAccessory();   
    new Service::AccessoryInformation(); 
      new Characteristic::Identify();  
    new Service::LightBulb();
      new Characteristic::On();
      new Characteristic::TestChar(30);
      new Characteristic::TestString();
      new Characteristic::TestString("MyName");
      new Characteristic::TestTLV();
      Characteristic::TestData *testData = new Characteristic::TestData();

      TLV8 myTLV;

      myTLV.add(5,0x20);
      myTLV.add(5,0x30);
      myTLV.add(1);
      myTLV.add(5,255);

      Characteristic::TestTLV *testTLV = new Characteristic::TestTLV(myTLV);

      size_t n=testTLV->getData(NULL,0);
      uint8_t buf[n];
      testTLV->getData(buf,n);

      Serial.printf("\n");
      for(int i=0;i<n;i++)
      Serial.printf("%d %0X\n",i,buf[i]);
      Serial.printf("\n");

      testData->setData(buf,8);
}   


//////////////////////////////////////

void loop(){
 
  homeSpan.poll();  
}

//////////////////////////////////////
