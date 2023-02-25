/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2022 Gregg E. Berman
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
// Example 8: HomeKit Bridges and Bridge Accessories      //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_LED.h"     

void setup() {

  // If the only Service defined in the FIRST Accessory of a multi-Accessory device is the required Accessory Information Service,
  // the device is said to be configured as a "Bridge".  Historically there may have been a number of functional differences between bridge
  // devices and non-bridge devices, but since iOS 15, it's not obvious there are any differences in functionality, with two exceptions:
  
  //  1. Recall from Example 7 that the use of Characteristic::Name() to change the default name of an Accessory Tile
  //     does not work for the first Accessory defined.  The Home App always displays the default name of the first Accessory Tile
  //     as the name of the device specified in homeSpan.begin().  However, this is not an issue when implementing a device
  //     as a Bridge, since the first Accessory is nothing but the Bridge itself - having the default name match the name
  //     of the device in this case makes much more sense.  More importantly, you can now use Characteristic::Name() to change the 
  //     default name of BOTH the LED Accessory Tiles.

  //  2. Devices configured as a Bridge appear in the Home App under the main settings page that displays all Hubs and Bridges.

  // The sketch below is functionally identical to Example 7, except that instead of defining two Accessories (one for the Simple On/Off
  // LED and one for the Dimmable LED), we define three Accessories, where the first acts as the Bridge.
  
  // As usual, all previous comments have been deleted and only new changes from the previous example are shown.

  // NOTE: To see how this works in practice, you'll need to unpair your device and re-pair it once the new code is loaded.
  
  Serial.begin(115200);

  // Below we replace Category::Lighting with Category::Bridges. This changes the icon of the device shown when pairing
  // with the Home App, but does NOT change the icons of the Accessory Tiles.  You can choose any Category you like.
  // For instance, we could have continued to use Category::Lighting, even though we are configuring the device as a Bridge.

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");
  
  new SpanAccessory();                            // This first Accessory is the new "Bridge" Accessory.  It contains no functional Services, just the Accessory Information Service
    new Service::AccessoryInformation();
      new Characteristic::Identify();            
 
  new SpanAccessory();                            // This second Accessory is the same as the first Accessory in Example 7, with the exception that Characteristic::Name() now does something
    new Service::AccessoryInformation();
      new Characteristic::Identify();            
      new Characteristic::Name("Simple LED");     // Note that unlike in Example 7, this use of Name() is now utilized by the Home App since it is not the first Accessory (the Bridge above is the first)
    new DEV_LED(16);

  new SpanAccessory();                            // This third Accessory is the same as the second Accessory in Example 7
    new Service::AccessoryInformation();    
      new Characteristic::Identify();               
      new Characteristic::Name("Dimmable LED");  
    new DEV_DimmableLED(17);

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
