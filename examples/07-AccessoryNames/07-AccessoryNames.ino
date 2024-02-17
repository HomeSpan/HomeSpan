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
// Example 7: Changing an Accessory's default name        //
//            to distinguish On/Off from Dimmable LEDs    //
//                                                        //
////////////////////////////////////////////////////////////


#include "HomeSpan.h" 
#include "DEV_LED.h"          

void setup() {

  // As discusses in previous examples, the Home App automatically generates default names for each Accessory Tile
  // based on the Name provided in the second argument of homeSpan.begin().  And though you can change these names
  // both during, and anytime after, pairing, HAP also allows you to customize the default names themselves, so
  // something more intuitive is presented to the user when the device is first paired.

  // Changing the default name for an Accessory is done by adding an optional Name Characteristic to the
  // Accessory Information Service.  This causes the Home App to use the value of that Characteristic as the default name
  // for an Accessory Tile, instead of generating one from the name used in homeSpan.begin().
  
  // Howevever, there is one caveat:  The Name Characteristic has no affect when used in the first Accessory of a device.
  // Rather, the default name of the first Accessory Tile will always be shown by the Home App as the name specified in
  // homeSpan.begin() regardless of whether or not the Name Characteristic has been added to the Accessory Information Service.

  // Below is a replay of Example 6 showing how the Name Characteristic can be used to change the default names of the second
  // and third, but not the first, Accessory Tile.
  
  Serial.begin(115200);

  homeSpan.begin(Category::Lighting,"HomeSpan LED");    // Note this results in the default name of "HomeSpan LED", "HomeSpan LED 2", etc. for each Accessory Tile
  
  new SpanAccessory(); 
  
    new Service::AccessoryInformation();
      new Characteristic::Identify();            
      new Characteristic::Name("Simple LED");     // This use of Name() will be ignored by the Home App.  The default name for the Accessory will continue to be shown as "HomeSpan LED" 

    new DEV_LED(16);

  new SpanAccessory(); 
  
    new Service::AccessoryInformation();    
      new Characteristic::Identify();               
      new Characteristic::Name("Dimmable LED");   // This DOES change the default name for the Accessory from "HomeSpan LED 2" to "Dimmable LED"      
  
    new DEV_DimmableLED(17);

  new SpanAccessory(); 
  
    new Service::AccessoryInformation();    
      new Characteristic::Identify();               
      new Characteristic::Name(u8"Special chars ÄÖÜß");  // Use UTF-8 coded string for non-ASCII characters    
  
    new DEV_DimmableLED(18);

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
