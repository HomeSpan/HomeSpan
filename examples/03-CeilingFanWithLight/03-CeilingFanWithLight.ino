/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020 Gregg E. Berman
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
 
////////////////////////////////////////////////////////////
//                                                        //
//    HomeSpan: A HomeKit implementation for the ESP32    //
//    ------------------------------------------------    //
//                                                        //
// Example 3: A simple on/off ceiling fan with an         //
//            on/off ceiling light                        //
//                                                        //
////////////////////////////////////////////////////////////


#include "HomeSpan.h"         // Always start by including the HomeSpan library

void setup() {

  // Example 3 shows how adding multiple Services to a single Accessory allows us to create a multi-featured Accessory, such as a ceiling fan wih a ceiling light
 
  Serial.begin(115200);      // Start a serial connection - this is needed for you to type in your WiFi credentials

  homeSpan.begin(Category::Fans,"HomeSpan Ceiling Fan");  // Begin a HomeSpan Session - note the Category has been set to "Fans"

  // We begin by creating a Light Bulb Accessory just as in Examples 1 and 2, but with Name now set to "My Ceiling Fan"

  new SpanAccessory();                            // Begin by creating a new Accessory using SpanAccessory(), which takes no arguments 
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service, which has 6 required Characteristics
      new Characteristic::Name("My Ceiling Fan");     // Name of the Accessory, which shows up on the HomeKit "tiles", and should be unique across Accessories
      new Characteristic::Manufacturer("HomeSpan");   // Manufacturer of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::SerialNumber("123-ABC");    // Serial Number of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::Model("120-Volt Lamp");     // Model of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::FirmwareRevision("0.9");    // Firmware of the Accessory (arbitrary text string, and can be the same for every Accessory) 
      new Characteristic::Identify();                 // Create the required Identify  
      
    new Service::HAPProtocolInformation();          // Create the HAP Protcol Information Service  
      new Characteristic::Version("1.1.0");           // Set the Version Characteristicto "1.1.0" as required by HAP

    new Service::LightBulb();                       // Create the Light Bulb Service
      new Characteristic::On();                       // This Service requires the "On" Characterstic to turn the light on and off

  // Now we create the a Fan Service within this same Accessory

    new Service::Fan();                             // Create the Fan Service
      new Characteristic::Active();                   // This Service requires the "Active" Characterstic to turn the fan on and off

  // IMPORTANT:  HomeKit Controllers often cache a lot of information.  If your Controller does not update to match the above configuration,
  // simply select the Accessory in your Controller and under setting, select "Remove Accessory" and then re-pair.

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();         // run HomeSpan!
  
} // end of loop()
