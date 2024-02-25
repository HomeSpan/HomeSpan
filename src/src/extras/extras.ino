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

#define PIXEL_PIN 26    // set this to whatever pin you are using - note pin cannot be "input only"

Pixel testPixel(PIXEL_PIN);

void setup() {
 
  Serial.begin(115200);
  delay(1000);

  Serial.printf("Pixel Test on pin %d\n",PIXEL_PIN); 
}

//////////////////////////////////////

void loop(){
  Serial.print("Red\n");
  testPixel.set(Pixel::RGB(100,0,0));
  delay(2000);  

  Serial.print("Green\n");
  testPixel.set(Pixel::RGB(0,100,0));
  delay(2000);  
  
  Serial.print("Blue\n");
  testPixel.set(Pixel::RGB(0,0,100));
  delay(2000);  
}
