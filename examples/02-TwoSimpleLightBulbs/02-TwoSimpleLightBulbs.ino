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
// Example 2: Two non-functioning on/off light bulbs      //
//            constructed from basic HomeSpan components  //
//                                                        //
////////////////////////////////////////////////////////////


#include "HomeSpan.h"         // Always start by including the HomeSpan library

void setup() {

  // Example 2 expands on Example 1 by implementing two LightBulbs, each as their own Accessory
 
  Serial.begin(115200);

  homeSpan.begin(Category::Lighting,"HomeSpan LightBulbs");  // initialize HomeSpan - note the name is now "HomeSpan LightBulbs"

  // Here we create the first LightBulb Accessory just as in Example 1

  new SpanAccessory();                            // Begin by creating a new Accessory using SpanAccessory(), which takes no arguments 
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service, which has 6 required Characteristics
      new Characteristic::Name("My Table Lamp");      // Name of the Accessory, which shows up on the HomeKit "tiles", and should be unique across Accessories
      new Characteristic::Manufacturer("HomeSpan");   // Manufacturer of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::SerialNumber("123-ABC");    // Serial Number of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::Model("120-Volt Lamp");     // Model of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::FirmwareRevision("0.9");    // Firmware of the Accessory (arbitrary text string, and can be the same for every Accessory) 
      new Characteristic::Identify();                 // Create the required Identify  
      
    new Service::HAPProtocolInformation();          // Create the HAP Protcol Information Service  
      new Characteristic::Version("1.1.0");           // Set the Version Characteristicto "1.1.0" as required by HAP

    new Service::LightBulb();                       // Create the Light Bulb Service
      new Characteristic::On();                       // This Service requires the "On" Characterstic to turn the light on and off

  // Now we create a second Accessory, which is just a duplicate of Accessory 1 with the exception of changing the Name from "My Table Lamp" to "My Floor Lamp"

  new SpanAccessory();                            // Begin by creating a new Accessory using SpanAccessory(), which takes no arguments 
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service, which has 6 required Characteristics
      new Characteristic::Name("My Floor Lamp");      // Name of the Accessory, which shows up on the HomeKit "tiles", and should be unique across Accessories
      new Characteristic::Manufacturer("HomeSpan");   // Manufacturer of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::SerialNumber("123-ABC");    // Serial Number of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::Model("120-Volt Lamp");     // Model of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::FirmwareRevision("0.9");    // Firmware of the Accessory (arbitrary text string, and can be the same for every Accessory) 
      new Characteristic::Identify();                 // Create the required Identify  
      
    new Service::HAPProtocolInformation();          // Create the HAP Protcol Information Service  
      new Characteristic::Version("1.1.0");           // Set the Version Characteristicto "1.1.0" as required by HAP

    new Service::LightBulb();                       // Create the Light Bulb Service
      new Characteristic::On();                       // This Service requires the "On" Characterstic to turn the light on and off

  // That's it - our device now has two Accessories!

  // IMPORTANT: You should NOT have to re-pair your device with HomeKit when moving from Example 1 to Example 2.  HomeSpan will note
  // that the Attribute Database has been updated, and will broadcast a new configuration number when the program restarts.  This should
  // cause all iOS and MacOS HomeKit Controllers to automatically update and reflect the new configuration above.

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();         // run HomeSpan!
  
} // end of loop()
