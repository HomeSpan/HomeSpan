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
// Example 5: Two working on/off LEDs based on the        //
//            LightBulb Service                           //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_LED.h"          // NEW! Include this new file, DEV_LED.h, which will be fully explained below

void setup() {

  // First light! Control an LED from HomeKit!
  
  // Example 5 expands on Example 2 by adding in the code needed to actually control LEDs connected to the ESP32 from HomeKit.
  // In Example 2 we built out all the functionality to create a "Tile" Acessories inside HomeKit that displayed an on/off light, but
  // these control did not actually operate anything on the ESP32.  To operate actual devices HomeSpan needs to be programmed to
  // respond to "update" requests from HomeKit by performing some form of operation.
  
  // Though HomeKit itself sends "update" requests to individual Characteristics, this is not intuitive and leads to complex coding requirements
  // when a Service has more than one Characteristic, such as both "On" and "Brightness."  To make this MUCH easier for the user, HomeSpan
  // uses a framework in which Services are updated instead of individual Characteristics.  It does so by calling the update() method of
  // each Service with flags indicating all the Characteristics in that Service that HomeKit requested to update.  The user simply
  // implements code to perform the actual operation, and returns either true or false if the update was successful.  HomeSpan takes care of all
  // the underlying nuts and bolts.

  // Every Service defined in HomeKit, such as Service:LightBulb and Service:Fan (and even Service::AccessoryInformation) implements an update()
  // method that, as a default, does nothing but returns a value of true.  To actually operate real devices you need to over-ride this default update()
  // method with your own code.  The easiest way to do this is by creating a DERIVED class based on one of the built-in HomeSpan Services.
  // Within this derived class you can perform initial set-up routines (if needed), over-ride the update() method with your own code, and even create
  // any other methods or class-specific variables you need to fully operate complex devices.  Most importantly, the derived class can take arguments
  // so that you can make them more generic, re-use them multiple times (as will be seen below), and convert them to standalone modules (also shown below).

  // All of the HomeKit Services implemented by HomeSpan can be found in the Services.h file.  Any can be used as the parent for a derived Service.

  // We begin by repeating nearly the same code from Example 2, but with a few key changes. For ease of reading, all prior comments have been removed
  // from lines that simply repeat Example 2, and new comments have been added to explictly show the new code.

  Serial.begin(115200);

  homeSpan.begin(Category::Lighting,"HomeSpan LED");
  
  new SpanAccessory(); 
  
    new Service::AccessoryInformation(); 
      new Characteristic::Identify();                

  //  In Example 2 we instantiated a LightBulb Service and its "On" Characteristic here.  We are now going to replace these two lines (by commenting them out)...

  //  new Service::LightBulb();                   
  //    new Characteristic::On();                 

  // ...with a single new line instantiating a new class we will call DEV_LED():

    new DEV_LED(16);        // this instantiates a new LED Service.  Where is this defined?  What happpened to Characteristic::On?  Keep reading...

  // The full definition and code for DEV_LED is implemented in a separate file called "DEV_LED.h" that is specified using the #include at the top of this program.
  // The prefix DEV_ is not required but it's a helpful convention when naming all your device-specific Services. Note that DEV_LED will include all the required
  // Characterictics of the Service, so you DO NOT have to separately instantiate Characteristic::On --- everything HomeSpan needs for DEV_LED should be implemented
  // in DEV_LED itself (though it's not all that much).  Finally, note that we created DEV_LED to take a single integer argument.  If you guessed this is
  // the number of the Pin to which you have attached an LED, you'd be right.  See DEV_LED.h for a complete explanation of how it works.

  new SpanAccessory(); 
  
    new Service::AccessoryInformation();    
      new Characteristic::Identify();                       

  //  new Service::LightBulb();                       // Same as above, this line is deleted...
  //    new Characteristic::On();                     // This line is also deleted...
  
    new DEV_LED(17);                                  // ...and replaced with a single line that instantiates a second DEV_LED Service on Pin 17

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
