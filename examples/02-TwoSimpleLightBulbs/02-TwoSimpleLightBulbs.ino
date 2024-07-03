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

  homeSpan.begin(Category::Lighting,"HomeSpan LightBulb");   // initializes a HomeSpan device named "HomeSpan Lightbulb" with Category set to Lighting

  // Here we create the first LightBulb Accessory just as in Example 1

  new SpanAccessory();                              // Begin by creating a new Accessory using SpanAccessory(), no arguments needed
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service, with the required Identify Characteristic
      new Characteristic::Identify();                 // Create the required Identify  

    new Service::LightBulb();                       // Create the Light Bulb Service
      new Characteristic::On();                       // This Service requires the "On" Characterstic to turn the light on and off

  // Now we create a second Accessory, which is just a duplicate of the first Accessory

  new SpanAccessory();                              // Begin by creating a new Accessory using SpanAccessory(), no arguments needed
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service, with the required Identify Characteristic
      new Characteristic::Identify();                 // Create the required Identify  

    new Service::LightBulb();                       // Create the Light Bulb Service
      new Characteristic::On();                       // This Service requires the "On" Characterstic to turn the light on and off

  // That's it - our device now has two Accessories, each displayed up as a separate Tile in the Home App!

  // Note that for a device with multiple Accessories, the Home App generates a default name for each Accessory Tile from the Name
  // specified in homeSpan.begin().  In this case, the default name for the first Accessory Tile will be "HomeSpan Lightbulb",
  // just as it was in Example 1, and the default name for the second Accessory Tile will be "HomeSpan Lightbulb 2".

  // You can of course change the name of each Accessory Tile from these defaults when prompted by the Home App during pairing.  You
  // can also change the name of any Accessory Tile, even after pairing, directly from the Home App by opening the settings page
  // for any given Tile.

  // In Example 7 we will demonstrate how the default names can be changed from within a HomeSpan sketch.

  // IMPORTANT: You should NOT have to re-pair your device with HomeKit when moving from Example 1 to Example 2.  HomeSpan will note
  // that the Attribute Database has been updated, and will broadcast a new configuration number when the program restarts.  This should
  // cause all iOS and MacOS HomeKit Controllers to automatically update and reflect the new configuration above.

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();         // run HomeSpan!
  
} // end of loop()
