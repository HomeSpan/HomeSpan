/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2022 Gregg E. Berman
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
 
// HomeSpan Addressable RGB LED Examples.  Demonstrates use of:
//
//  * HomeSpan Pixel Class that provides for control of single-wire addressable RGB and RGBW LEDs, such as the WS2812 and SK6812
//  * HomeSpan Dot Class that provides for control of two-wire addressable RGB LEDs, such as the APA102 and SK9822
//
// IMPORTANT:  YOU LIKELY WILL NEED TO CHANGE THE PIN NUMBERS BELOW TO MATCH YOUR SPECIFIC ESP32/S2/C3 BOARD
//

#define NEOPIXEL_PIN            23           // only one pin needed for NeoPixels

#define DOTSTAR_DATA_PIN        32           // two pins are needed to DotStars
#define DOTSTAR_CLOCK_PIN       5
  
#include "HomeSpan.h"
#include "extras/Pixel.h"                       // include the HomeSpan Pixel class

///////////////////////////////

struct Pixel_Light : Service::LightBulb {      // Addressable single-wire RGB LED Strand (e.g. NeoPixel)
 
  Characteristic::On power{0,true};
  Characteristic::Hue H{0,true};
  Characteristic::Saturation S{0,true};
  Characteristic::Brightness V{100,true};
  Pixel *pixel;
  uint8_t nPixels;
  
  Pixel_Light(uint8_t pin, uint8_t nPixels) : Service::LightBulb(){

    V.setRange(5,100,1);                      // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%
    pixel=new Pixel(pin);                     // creates Pixel LED on specified pin using default timing parameters suitable for most SK68xx LEDs
    this->nPixels=nPixels;                    // save number of Pixels in this LED Strand
    update();                                 // manually call update() to set pixel with restored initial values
  }

  boolean update() override {

    int p=power.getNewVal();
    
    float h=H.getNewVal<float>();       // range = [0,360]
    float s=S.getNewVal<float>();       // range = [0,100]
    float v=V.getNewVal<float>();       // range = [0,100]

    pixel->set(Pixel::HSV(h*p, s*p, v*p),nPixels);       // sets all nPixels to the same HSV color
          
    return(true);  
  }
};

///////////////////////////////

struct Dot_Light : Service::LightBulb {      // Addressable two-wire RGB LED Strand (e.g. DotStar) - Chasing Light Effect
 
  Characteristic::On power{0,true};
  Characteristic::Hue H{0,true};
  Characteristic::Saturation S{0,true};
  Characteristic::Brightness V{100,true};
  Dot *dot; 
  uint8_t nPixels;                                 
  Dot::Color *colors;
  int phase=0;
  int dir=1;
  uint32_t alarmTime=0;
  
  Dot_Light(uint8_t dataPin, uint8_t clockPin, uint8_t nPixels) : Service::LightBulb(){

    dot=new Dot(dataPin,clockPin);            // creates Dot LED on specified pins - suitable for APA102 or SK9822
    this->nPixels=nPixels;                    // save number of Pixels in this LED Strand

    colors=(Dot::Color *)calloc(2*nPixels-1,sizeof(Dot::Color));   // storage for dynamic pixel pattern
    update();                                                      // manually call update() to set pixel with restored initial values
  }

  boolean update() override {

    if(!power.getNewVal())
      dot->set(Dot::RGB(0,0,0),nPixels);        // turn off all LEDs in Strand
    
    return(true);  
  }

  void loop() override {

    if(millis()>alarmTime && power.getVal()){
      alarmTime=millis()+40;
      colors[phase]=Dot::HSV(0,0,0);
      if(phase==nPixels-1)
        dir=-1;
      else if(phase==0)
        dir=1;
      phase+=dir;

      float h=H.getNewVal<float>();       // range = [0,360]
      float s=S.getNewVal<float>();       // range = [0,100]
      float v=V.getNewVal<float>();       // range = [0,100]
      
      colors[phase]=Dot::HSV(h,s,100,v);

      dot->set(colors,nPixels);           // update Strand with new colors
    }
          
  }

};

///////////////////////////////

void setup() {
  
  Serial.begin(115200);
 
  homeSpan.begin(Category::Lighting,"RGB Pixels");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Name("NeoPixel");
      new Characteristic::Manufacturer("HomeSpan");
      new Characteristic::SerialNumber("123-ABC");
      new Characteristic::Model("8-LED Strand");
      new Characteristic::FirmwareRevision("1.0");
      new Characteristic::Identify();

  new Service::HAPProtocolInformation();
    new Characteristic::Version("1.1.0");

  new Pixel_Light(NEOPIXEL_PIN,8);                       // create 8-LED NeoPixel Strand
      
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Name("DotStar");
      new Characteristic::Manufacturer("HomeSpan");
      new Characteristic::SerialNumber("123-ABC");
      new Characteristic::Model("30-LED Strand");
      new Characteristic::FirmwareRevision("1.0");
      new Characteristic::Identify();

  new Service::HAPProtocolInformation();
    new Characteristic::Version("1.1.0");

  new Dot_Light(DOTSTAR_DATA_PIN,DOTSTAR_CLOCK_PIN,30);     // create 30-LED DotStar Strand with Chasing-Light effect

}

///////////////////////////////

void loop() {
  homeSpan.poll();
}

///////////////////////////////
