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

void setup() {
 
  Serial.begin(115200);

  homeSpan.setLogLevel(2);

  homeSpan.begin(Category::Lighting,"HomeSpan Test");
  
   new SpanAccessory();                       // start with Bridge Accessory
    new Service::AccessoryInformation();  
      new Characteristic::Identify(); 

// First dual-light/dual-switch Accessory

   new SpanAccessory();
    new Service::AccessoryInformation();  
      new Characteristic::Identify();  
      new Characteristic::Name("Access-1");                 // the AccessoryInformation Service seems to require Name(), and will ignore ConfiguredName()

    new Service::LightBulb();
      new Characteristic::On(0);
      new Characteristic::ConfiguredName("Main Light");     // here we use ConfiguredName() instead of Name().  This is properly reflected in Home App during pairing

    new Service::LightBulb();
      new Characteristic::On(0);
      new Characteristic::ConfiguredName("Night Light");
      
    new Service::Switch();
      new Characteristic::On(0);
      new Characteristic::ConfiguredName("Switch-A");

    new Service::Switch();
      new Characteristic::On(0);
      new Characteristic::ConfiguredName("Switch-B");

// Second dual-light/dual-switch Accessory

   new SpanAccessory();
    new Service::AccessoryInformation();  
      new Characteristic::Identify();  
      new Characteristic::Name("Access-2");               // note as above we use Name() instead of ConfiguredName() for the AccessoryInformation Service

    new Service::LightBulb();
      new Characteristic::On(0);
      new Characteristic::ConfiguredName("Aux Light");

    new Service::LightBulb();
      new Characteristic::On(0);
      new Characteristic::ConfiguredName("Night Light");
      
    new Service::Switch();
      new Characteristic::On(0);
      new Characteristic::ConfiguredName("Switch-A");

    new Service::Switch();
      new Characteristic::On(0);
      new Characteristic::ConfiguredName("Switch-B");

    Characteristic::AirQuality *air = new Characteristic::AirQuality;
    air->setVal(Characteristic::AirQuality::GOOD);
    air->setVal(air->GOOD);
      

}

//////////////////////////////////////

void loop(){
 
  homeSpan.poll();
  
}
