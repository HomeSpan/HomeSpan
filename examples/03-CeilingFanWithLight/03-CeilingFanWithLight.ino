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
// Example 3: A simple on/off ceiling fan with an         //
//            on/off ceiling light                        //
//                                                        //
////////////////////////////////////////////////////////////


#include "HomeSpan.h"         // Always start by including the HomeSpan library

void setup() {

  // Example 3 shows how adding multiple Services to a single Accessory allows us to create a multi-featured Accessory, such as a ceiling fan wih a ceiling light
 
  Serial.begin(115200);      // Start a serial connection - this is needed for you to type in your WiFi credentials

  homeSpan.begin(Category::Fans,"HomeSpan Ceiling Fan");  // Initialize HomeSpan - note the Category has been set to "Fans"

  // We begin by creating a Light Bulb Accessory just as in Examples 1 and 2

  new SpanAccessory();                              // Begin by creating a new Accessory using SpanAccessory(), no arguments needed
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service, with the required Identify Characteristic
      new Characteristic::Identify();                 // Create the required Identify  

    new Service::LightBulb();                       // Create the Light Bulb Service
      new Characteristic::On();                       // This Service requires the "On" Characterstic to turn the light on and off

  // Now we add a Fan Service within this same Accessory

    new Service::Fan();                             // Create the Fan Service
      new Characteristic::Active();                   // This Service requires the "Active" Characterstic to turn the fan on and off

 // Similar to Example 2, we will also implement a LightBulb as a second Accessory

  new SpanAccessory();                              // Begin by creating a new Accessory using SpanAccessory(), no arguments needed
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service, with the required Identify Characteristic
      new Characteristic::Identify();                 // Create the required Identify  

    new Service::LightBulb();                       // Create the Light Bulb Service
      new Characteristic::On();                       // This Service requires the "On" Characterstic to turn the light on and off      

  // If everything worked correctly you should now see two Tiles in the Home App:
  // 
  //  * a Tile named "HomeSpan Ceiling Fan" with an icon of a Fan.  Clicking this Tile should open the
  //    control page showing a Fan control on the left, and a Light control on the right
  //
  //  * a Tile named "HomeSpan Ceiling Fan 2" with an icon of a LightBulb.  Clicking this Tile should
  //    toggle the Light On/Off
  
  // The reason for including the second LightBulb Accessories in this example is to illustrate the impact of the device's Category
  // on various icons.  Setting Category to Fan in homeSpan.begin() serves two purposes.  First, it sets the icon for the device itself,
  // as shown by the Home App during initial pairing, to a Fan.  Second, it helps the Home App to determine which icon to use for an
  // Accessory Tile when there is ambiguity.  The second Accessory contains nothing but a LightBulb Service, so the Home App sensibly
  // uses a LightBulb icon for the Tile.  But what icon should the Home App use for the first Accessory containing both a Fan Service
  // and a LightBulb Service?  Either a Fan or LightBulb icon would make sense.  Setting the Category of the device to Fan causes
  // the Home App to choose a Fan icon for the first Accessory.
  
  // As a test of this, unpair the device; change the Category to Lighting (as in Example 2); re-load the sketch; and re-pair the device.
  // You should now see the icon for the "HomeSpan Ceiling Fan" Tile is a LightBulb, and the control screen for the Accessory should
  // show the Light control on the left and the Fan control on the right.

  // IMPORTANT:  HomeKit Controllers often cache a lot of information.  If your Controller does not update to match the above configuration,
  // simply select the Accessory in your Controller and under settings, select "Remove Accessory", but BEFORE re-pairing the device, type
  // 'H' into the HomeSpan CLI.  This forces HomeSpan to reboot and generate a new device ID so that it will look "brand new" to the Home App
  // when you re-pair.

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();         // run HomeSpan!
  
} // end of loop()
