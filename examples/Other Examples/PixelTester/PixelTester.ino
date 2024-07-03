/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2024 Gregg E. Berman
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

/////////////////////// PIXEL TESTER //////////////////////////

// This sketch is designed to help identify the proper settings to use for a NeoPixel, NeoPixel Strip,
// or any device containing one or more single-wire addressable RGB or RGBW LEDs (the "Pixel Device").

// Before compiling, set PIXEL_PIN to the ESP32 pin that is connected to your Pixel Device, and set NPIXELS to
// the numnber of Pixels in the Pixel Device.  Note that for some strips a single chip controls more than one LED,
// in which case NPIXELS should be set to the number of controlling chips, NOT the number of LEDs.

// To start, the second argument of the Pixel constructor for the testPixel object below should remain
// set to PixelType::RGBW

// When run, the sketch will repeatedly cycle colors by setting ALL pixels in the device first to RED, then GREEN,
// followed by BLUE, and then finally WHITE.  After a short pause, the cycle repeats.

// For each color the brightness will increase from 0 through MAX_BRIGHTNESS, and then back to 0. You can change
// MAX_BRIGHTNESS to something lower than 255 if you want to limit how bright the pixels get.

// For Pixel Devices with more than one pixel, diagnostics are as follows:
//
// * If all 4 colors repeatedly flash in the order expected, this means the base setting of PixelType::RGBW is correct!
//
// * If instead of each pixel being set to the same color, the pixels in the strip each light up with a different color
//   (or no color at all), this means you have an RGB LED, not an RGBW LED.  Change the second parameter of the constructor
//   to PixelType::RGB and re-run the sketch.
//
// * If all of the pixels are being set to the same color, but the sequence is NOT in the order RED, GREEN, BLUE, change
//   the second parameter of the constructor so that the order of the PixelType colors match the sequence of the colors
//   that appear on the Pixel Device.  For example, if your RGBW Pixel Device flashes GREEN, RED, BLUE, and than WHITE, use
//   PixelType::GRBW.

// For Pixel Devices with only a single pixel, diagnostics are as follows:

// * If all 4 colors repeatedly flash in the order expected, this means the base setting of PixelType::RGBW is correct!
//
// * If the pixel does not light at all when set to WHITE this means you have an RGB LED, not an RGBW LED.  Change the
//   second parameter of the constructor to PixelType::RGB and re-run the sketch.
//
// * If all of the pixels are being set to the same color, but the sequence is NOT in the order RED, GREEN, BLUE, change
//   the second parameter of the constructor so that the order of the PixelType colors match the sequence of the colors
//   that appear on the Pixel Device.  For example, if your RGB Pixel Device flashes GREEN, RED, and then BLUE, use
//   PixelType::GRB.

//////////////////////////////////////

#include "HomeSpan.h"

//////////////////////////////////////

#define MAX_BRIGHTNESS  255           // maximum brightness when flashing RGBW [0-255]

#define PIXEL_PIN 26                  // set this to whatever pin you are using - note pin cannot be "input only"
#define NPIXELS   8                   // set to number of pixels in strip

Pixel testPixel(PIXEL_PIN, PixelType::RGBW);      // change the second argument until device operates with correct colors

//////////////////////////////////////

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

  if(testPixel.isRGBW()){
    Serial.printf("White...");
    flashColor(0,0,0,1);
  }

  Serial.printf("Pausing.\n");
  delay(1000);
}

//////////////////////////////////////
