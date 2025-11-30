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

#include "Pixel.h"

Dot p(33, 27);
Dot::Color empty[8]={};
Dot::Color colors[8]={Dot::RGB(0, 50, 0),  Dot::RGB(50, 50, 0), Dot::RGB(50, 0, 0) ,Dot::RGB(0, 0, 0), Dot::RGB(0, 0, 255), Dot::RGB(0,0,0), Dot::RGB(0,0,255,15),Dot::RGB(255,0,0,2)};

void setup() {

  Serial.begin(115200);
  delay(2000);
  Serial.printf("\n\nReady\n");
  p.set(Dot::RGB(0,255,0,5),9);
  delay(5000);
}

void loop() {
  p.set(Dot::RGB(255,0,0,2),8);
  delay(1000);
  p.set(Dot::RGB(0,0,255,2),8);
  delay(1000);    
}
