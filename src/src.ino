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

// This is a placeholder .ino file that allows you to easily edit the contents of this library using the Arduino IDE,
// as well as compile and test from this point.  This file is ignored when the library is included in other sketches.

#include "HomeSpan.h"

CUSTOM_CHAR(CharFloat, 00000001-0001-0001-0001-46637266EA00, PR+PW+EV, FLOAT, 0, 0, 100, false);
CUSTOM_CHAR(CharUInt8, 00000009-0001-0001-0001-46637266EA00, PR+PW+EV, UINT8, 0, 0, 100, false);
CUSTOM_CHAR(CharUInt16, 00000016-0001-0001-0001-46637266EA00, PR+PW+EV, UINT16, 0, 0, 100, false);
CUSTOM_CHAR(CharUInt32, 00000032-0001-0001-0001-46637266EA00, PR+PW+EV, UINT32, 0, 0, 100, false);
CUSTOM_CHAR(CharInt, 00000002-0001-0001-0001-46637266EA00, PR+PW+EV, INT, 0, 0, 100, false);

//////////////////////////////////////
  
void setup() {
  
  Serial.begin(115200);

  homeSpan.setLogLevel(1);

  homeSpan.begin(Category::Other,"HomeSpan Test");

  new SpanAccessory();  
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
    new Service::LightBulb();
      new Characteristic::On();

      (new Characteristic::CharFloat())->setValidValues(5,0,1,2,6,7,8);
      (new Characteristic::CharUInt8())->setValidValues(5,0,1,2,6,7,8);
      (new Characteristic::CharUInt16())->setValidValues(5,0,1<<8,1<<16,0xFFFFFFFF,-1);
      (new Characteristic::CharUInt32())->setValidValues(5,0,1<<8,1<<16,0xFFFFFFFF,-1);
      (new Characteristic::CharInt())->setValidValues(5,0,255,2000000000,-2000000000,-1)->setValidValues(1,2);
  
} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();

} // end of loop()

//////////////////////////////////////
