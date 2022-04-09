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
// Example 13: Target States and Current States           //
//             * implementing a Garage Door Opener        //
//             * implementing a motorized Window Shade    //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_DoorsWindows.h" 

void setup() {

  // In Example 12 we saw how to implement the loop() method for a Service to continuously monitor our device and periodically report
  // changes in one or more Characteristics back to HomeKit using setVal() and timeVal().  In that example we implemented passive sensors
  // that operated independently and required no input from the user, which meant we did not need to implement any update() methods.

  // In this Example 13 we demonstrate the simultaneous use of both the update() and loop() methods by implementing two new Services:
  // a Garage Door Opener and a motorized Window Shade.  Both examples showcase HomeKit's Target-State/Current-State framework.
  // For physical devices that take time to operate (such as closing a door), HomeKit Services typically use:
  
  //    * one Characteristic that HomeKit sets via update() requests to HomeSpan, and that represent a desired target state,
  //      such as opened, closed, or in some cases a percentage opened or closed; and
  
  //    * one read-only Characteristic that HomeSpan use to track the current state of the device in the loop() method, as well as
  //      report back changes to HomeKit using setVal().
  
  // Not all HomeKit Services utilize the same Characteristics to define target and current states.  Some Services use Characteristics
  // that are specific to that one Service, whereas others use more generic Characteristics.  The common theme seems to be that HomeKit
  // guesses the actions a device is taking, and updates it tile's icon accordingly, by comparing the value of the target state
  // Characteristic it sets, and the current state Characteristic it receives in the form of Event Notifications.  When they are the same,
  // HomeKit assumes the physical device has reached the required position.  When they differ, HomeKit assumes something will be opening,
  // closing, raising, lowering, etc.  The details of this process for each Service is outlined in the HAP documentation, but beware
  // the document is not always up to date with the lastest version of the HomeKit application.  Sometimes a little experimenting and a lot
  // of trial and error is required to fully understand how each Service responds to different combinations of Characteristic values.
  
  // As always, we won't be connecting our ESP32 to an actual garage door or window shade but will instead simulate their responses and
  // actions for illustrative purposes. In some ways the code is more complicated because of the need to simulate values - it might be
  // easier if we actually were connecting to a garage door or window shade!
  
  // Fully commented code for both of our derived Services can be found in DEV_DoorsWindows.h.  These examples do not introduce any new
  // HomeSpan functions or features.  Rather we are combining everything learned so far into two reasonably complex Services.  You may
  // want to reference the HAP documentation for these two parent Services to fully understand the meaning of the different value settings
  // for each of the Services' Characteristics.
  
  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");

  new SpanAccessory();  
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      
  new SpanAccessory();                                                          
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      new Characteristic::Name("Garage Door");
    new DEV_GarageDoor();                                                               // Create a Garage Door Opener (see DEV_DoorsWindows.h for definition)

  new SpanAccessory();                                                          
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      new Characteristic::Name("Window Shade");
    new DEV_WindowShade();                                                              // Create a motorized Window Shade (see DEV_DoorsWindows.h for definition)

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
