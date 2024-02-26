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

#define MAX_BRIGHTNESS  255           // maximum brightness when flashing RGB [0-255]

#define PIXEL_PIN 26                  // set this to whatever pin you are using - note pin cannot be "input only"
#define NPIXELS   8                  // set to number of pixels in strand

Pixel testPixel(PIXEL_PIN, PixelType::RGBW);

void setup() {
 
  Serial.begin(115200);
  delay(1000);

  Serial.printf("\n\nPixel Test on pin %d with %d pixels\n\n",PIXEL_PIN,NPIXELS);
}

//////////////////////////////////////

void flashColor(boolean r, boolean g, boolean b, boolean w){
  
  for(int i=0;i<MAX_BRIGHTNESS;i++){
    testPixel.set(Pixel::RGB(i*r,i*g,i*b,i*w),NPIXELS);
    delay(4);
  }
    
  for(int i=MAX_BRIGHTNESS;i>=0;i--){
    testPixel.set(Pixel::RGB(i*r,i*g,i*b,i*w),NPIXELS);
    delay(4);
  }
}

//////////////////////////////////////

void loop(){

  Serial.printf("Red...");
  flashColor(1,0,0,0);

  Serial.printf("Green...");
  flashColor(0,1,0,0);

  Serial.printf("Blue...");
  flashColor(0,0,1,0);

  Serial.printf("White...");
  flashColor(0,0,0,1);

  Serial.printf("Pausing.\n");
  delay(1000);
}
