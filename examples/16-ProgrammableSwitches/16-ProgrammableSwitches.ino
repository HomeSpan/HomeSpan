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
// Example 16: Stateless Programmable Switches            //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_ProgButton.h"     

void setup() {

  // Example 16 does not introduce any new HomeSpan functionality, but instead showcases a unique feature of HomeKit that you can readily access with HomeSpan.
  // In all prior examples we used the ESP32 to control a local appliance - something connected directly to the ESP32 device.  We've then seen how you can control
  // the device via HomeKit's iOS or MacOS Home App, or by the addition of local pushbuttons connected directly to the ESP32 device.

  // In this example we do the opposite, and use buttons on the ESP32 to control OTHER HomeKit devices.
  
  // To do so, we use HomeKit's Stateless Programmable Switch Service.  Similar to other read-only Services, such as the Temperature and Air Quality Sensors 
  // fully explored in Example 12, the Stateless Programmable Switch Service only listens for event notifications coming from HomeSpan and does not try to control
  // or update anything on the HomeSpan Device.  More specifically, the Stateless Programmable Switch Service listens for notifications of a SINGLE, DOUBLE,
  // or LONG button press coming from HomeSpan.
  
  // What these button presses mean is outside the control of HomeSpan.  Instead, you program their actions directly in the Home App.  In this fashion, HomeSpan
  // becomes a platform for generic buttons that you can program to control any other HomeKit accessory or even trigger HomeKit scenes.

  // Upon running this configuration and pairing to HomeKit, your Home App should reveal a new tile labeled "PushButton Switches."  Clicking that tile will open up
  // a new page where you can program the actions of each of the buttons.  These actions can be changed at any time without any need to modify the HomeSpan code,
  // or even reboot the device. 

  // The code for this is quite simple, and as usual we've encapsulated all the functionality in a standalone file: DEV_ProgButton.h.  Below we create two generic
  // buttons, one connected to pin 23, and one connected to pin 5.  See DEV_ProgButton.h for complete details. 
  
  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");

  new SpanAccessory();  
    new Service::AccessoryInformation();
      new Characteristic::Identify();

  new SpanAccessory();                                                          
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      new Characteristic::Name("PushButton Switches");

    // We've written DEV_ProgButton to take two arguments.  The first is a pin number that DEV_ProgButton.h uses to create a SpanButton.  The second is an index number
    // that HomeKit uses as a label when you program the actions of each button in the Home App.  The numbers do not have to be sequential, nor start with 1.  They just need
    // to be unique so HomeKit can distinguish them.  Note that HomeKit does not require index numbers if you only have one Stateless Programmable Switch Service within any
    // given Accessory.  Since we have two, we must specify two unique index numbers.
    
    new DEV_ProgButton(23,1);       // create Stateless Programmable Switch Service on pin 23 with index=1
    new DEV_ProgButton(5,2);        // create Stateless Programmable Switch Service on pin 5 with index=2
 
} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
