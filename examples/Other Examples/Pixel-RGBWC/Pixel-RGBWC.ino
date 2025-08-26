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
 
// HomeSpan Addressable RGBCW LED Example.  Demonstrates use of two separate Accessories to control a SINGLE
// NeoPixel LED Strip.  The first controls the RGB LEDs and the second controls the Warm-White and Cool-White LEDs.
// Both sets can be on at the same time. 
//
// Note use of Pixel::CCT() method to convert color temperature into values for the White LEDs.
//
// Also note that HomeKit uses the Mirek scale for color temperture instead of the Kelvin scale.  To convert from
// one to the other, divide into 1 million:  Mirek=1.0e6/Kelvin and Kelvin=1.0e6/Mirek.
//

#ifndef HS_FEATHER_PINS
  #define NEOPIXEL_PIN    23        // CHANGE THIS PIN TO MATCH YOUR BOARD
#else
  #define NEOPIXEL_PIN    F23       // FEATHER PIN MAPPING - FOR HOMESPAN DEVELOPER USE ONLY
#endif
 
#define NPIXELS         100       // number of controllable pixels in strand (which may be less than actual number of LEDs)
#define WARM_LED_TEMP   3000      // temperature (in Kelvin) of Warm-White LED
#define COOL_LED_TEMP   6500      // temperature (in Kelvin) of Cool-White LED
 
#include "HomeSpan.h"

Pixel pixel(NEOPIXEL_PIN,"GRBWC");     // create a global instance of a Pixel strand supporting RGB LEDs plus warm AND cool white LED
Pixel::Color colorRGB;                 // create a global instance of a Pixel color to be used to store the RGB color
Pixel::Color colorWC;                  // create a global instance of a Pixel color to be used to store the WC color

///////////////////////////////

struct NeoPixel_RGB : Service::LightBulb {      // define an RGB Lightbulb to control the RGB LEDs on the NeoPixel Light Strip
 
  Characteristic::On power{0,true};
  Characteristic::Hue H{0,true};
  Characteristic::Saturation S{0,true};
  Characteristic::Brightness V{100,true};
  
  NeoPixel_RGB() : Service::LightBulb(){
    
    V.setRange(5,100,1);
    update();                                 // manually call update() to set pixel with restored initial values
  }

  boolean update() override {

    colorRGB=pixel.HSV(H.getNewVal(), S.getNewVal(), V.getNewVal()*power.getNewVal());
    pixel.set(colorRGB+colorWC,NPIXELS);          
    return(true);  
  }
};

///////////////////////////////

struct NeoPixel_WC : Service::LightBulb {      // define WC Lightbulb to control the Warm White and Cool White LEDs on the NeoPixel Light Strip
 
  Characteristic::On power{0,true};
  Characteristic::Brightness V{100,true};
  Characteristic::ColorTemperature T{(uint32_t)1.0e6/((COOL_LED_TEMP+WARM_LED_TEMP)/2),true};     // set initial value to average of Warm and Cool Temp
  
  NeoPixel_WC() : Service::LightBulb(){

    V.setRange(5,100,1);
    T.setRange(1.0e6/COOL_LED_TEMP,1.0e6/WARM_LED_TEMP);      // set range of control to match range of Warm-White and Cool-White LEDs
    
    update();                                 // manually call update() to set pixel with restored initial values
  }

  boolean update() override {

    colorWC=pixel.CCT(1.0e6/T.getNewVal(), V.getNewVal()*power.getNewVal());   // convert HomeKit temperature (in Mirek) to Kelvin
    pixel.set(colorRGB+colorWC,NPIXELS);          
    return(true);  
  }
};

///////////////////////////////

void setup() {
  
  Serial.begin(115200);

  pixel.setTemperatures(WARM_LED_TEMP,COOL_LED_TEMP);        // set color temperatures of Warm-White and Cool-White LEDs
 
  homeSpan.begin(Category::Lighting,"RGBWC Light");

  SPAN_ACCESSORY();

  SPAN_ACCESSORY("Neo RGB");
    new NeoPixel_RGB();

  SPAN_ACCESSORY("Neo WC");
    new NeoPixel_WC();
}

///////////////////////////////

void loop() {
  homeSpan.poll();
}

///////////////////////////////
