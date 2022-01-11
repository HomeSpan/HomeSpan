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
 
 // HomeSpan Addressable RGB LED Example

// Demonstrates use of HomeSpan Pixel Class that provides for control of single-wire
// addressable RGB LEDs, such as the SK68xx or WS28xx models found in many devices,
// including the Espressif ESP32, ESP32-S2, and ESP32-C3 development boards.

#include "HomeSpan.h"
#include "extras/Pixel.h"                       // include the HomeSpan Pixel class

CUSTOM_CHAR(Selector, 00000001-0001-0001-0001-46637266EA00, PR+PW+EV, UINT8, 1, 1, 5, false);      // create Custom Characteristic to "select" special effects via Eve App

///////////////////////////////

struct Pixel_Light : Service::LightBulb {      // Addressable RGB Pixel
 
  Characteristic::On power{0,true};
  Characteristic::Hue H{0,true};
  Characteristic::Saturation S{0,true};
  Characteristic::Brightness V{100,true};
  Pixel *pixel; 
  
  Pixel_Light(int pin) : Service::LightBulb(){

    V.setRange(5,100,1);                      // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%
    pixel=new Pixel(pin);                     // creates pixel LED on specified pin using default timing parameters suitable for most SK68xx LEDs
    update();                                 // manually call update() to set pixel with restored initial values
  }

  boolean update() override {

    int p=power.getNewVal();
    
    float h=H.getNewVal<float>();       // range = [0,360]
    float s=S.getNewVal<float>();       // range = [0,100]
    float v=V.getNewVal<float>();       // range = [0,100]

    pixel->setHSV(h*p, s*p, v*p);       // sets pixel to HSV colors
          
    return(true);  
  }
};

///////////////////////////////

struct Pixel_Strand : Service::LightBulb {      // Addressable RGB Pixel Strand of nPixel Pixels - allows for special effects controlled by custom characreristic
 
  Characteristic::On power{0,true};
  Characteristic::Hue H{0,true};
  Characteristic::Saturation S{0,true};
  Characteristic::Brightness V{100,true};
  Pixel *pixel; 
  int nPixels;                                 // number of Pixels in Strand (default=1)
  
  Pixel_Strand(int pin, int nPixels=1) : Service::LightBulb(){

    V.setRange(5,100,1);                      // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%
    pixel=new Pixel(pin);                     // creates pixel LED on specified pin using default timing parameters suitable for most SK68xx LEDs
    this->nPixels=nPixels;                    // store number of Pixels in Strand
    update();                                 // manually call update() to set pixel with restored initial values
  }

  boolean update() override {

    int p=power.getNewVal();
    
    float h=H.getNewVal<float>();                // range = [0,360]
    float s=S.getNewVal<float>();                // range = [0,100]
    float v=V.getNewVal<float>();                // range = [0,100]

    pixel->setHSV(h*p, s*p, v*p, nPixels);       // sets all nPixels to HSV colors
          
    return(true);  
  }
};

///////////////////////////////

void setup() {
  
  Serial.begin(115200);
 
  homeSpan.begin(Category::Lighting,"RGB Pixels");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Name("RGB Pixel");
      new Characteristic::Manufacturer("HomeSpan");
      new Characteristic::SerialNumber("123-ABC");
      new Characteristic::Model("SK68 LED");
      new Characteristic::FirmwareRevision("1.0");
      new Characteristic::Identify();

  new Service::HAPProtocolInformation();
    new Characteristic::Version("1.1.0");

  new Pixel_Light(8);                         // create single Pixel attached to pin 8
      
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Name("Pixel Strand");
      new Characteristic::Manufacturer("HomeSpan");
      new Characteristic::SerialNumber("123-ABC");
      new Characteristic::Model("8-LED NeoPixel");
      new Characteristic::FirmwareRevision("1.0");
      new Characteristic::Identify();

  new Service::HAPProtocolInformation();
    new Characteristic::Version("1.1.0");

  new Pixel_Strand(7,8);                         // create single Pixel attached to pin 8
  
  (new Characteristic::Selector())->setUnit("")->setDescription("Color Effect")->setRange(1,5,1);
  

}

///////////////////////////////

void loop() {
  homeSpan.poll();
}

///////////////////////////////
