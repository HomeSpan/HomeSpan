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
// Example 1: A non-functioning on/off light control      //
//            constructed from basic HomeSpan components  //
//                                                        //
////////////////////////////////////////////////////////////


  // WELCOME TO HOMESPAN!
  
  // This first example introduces the HomeSpan library and demonstrates how to implement a simple on/off light control
  // using a combination of HomeSpan Accessory, Service, and Characteristic objects.  Once this sketch has been uploaded
  // to your HomeSpan device and the device is paired to your home, a new "lightbulb" tile will appear in the Home App of your iPhone,
  // iPad, or Mac.
  
  // Though the tile will be fully operational (i.e. you can change the status of the lightbulb from "on" or "off"), we won't yet connect
  // an actual light or LED to the HomeSpan device, so nothing real will light up.  Instead, in this and the next few examples, we'll focus
  // on learning about the different ways HomeKit controls can be configured.  Starting in Example 5, we'll connect an LED to the device
  // and introduce the methods that actually turn the LED on and off from your Home App.

  // NOTE: All HomeSpan examples are best understood when reviewed in conjunction with the documentation provided on the HomeSpan GitHub page.
  // See https://github.com/HomeSpan/HomeSpan for details and references.  In particular, you may want to review the HomeSpan API Overview
  // page before proceeding.
  
  // These examples also make frequent reference to  Apple's HomeKit Accessory Protocol Specification, known as HAP.  You can download this
  // directly from Apple at https://developer.apple.com/support/homekit-accessory-protocol.

  // LET'S GET STARTED...

#include "HomeSpan.h"         // HomeSpan sketches always begin by including the HomeSpan library

void setup() {                // Your HomeSpan code should be placed within the standard Arduino setup() function
 
  Serial.begin(115200);       // Start a serial connection so you can receive HomeSpan diagnostics and control the device using HomeSpan's Command-Line Interface (CLI)

  // The HomeSpan library creates a global object named "homeSpan" that encapsulates all HomeSpan functionality.
  // The begin() method is used to initialize HomeSpan and start all HomeSpan processes.
  
  // The first two parameters are Category and Name, which are used by HomeKit to configure the icon and name of the device shown in your Home App
  // when initially pairing your device.

  homeSpan.begin(Category::Lighting,"HomeSpan LightBulb");   // initializes a HomeSpan device named "HomeSpan Lightbulb" with Category set to Lighting

  // Next, we construct a simple HAP Accessory Database with a single Accessory containing 3 Services,
  // each with their own required Characteristics.
  
  new SpanAccessory();                              // Begin by creating a new Accessory using SpanAccessory(), which takes no arguments
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service, which has 6 required Characteristics:
      new Characteristic::Name("My Table Lamp");      // Name of the Accessory, which shows up on the HomeKit "tiles", and should be unique across Accessories
      
  // The next 4 Characteristics serve no function except for being displayed in HomeKit's setting panel for each Accessory.  They are nevertheless required by HAP:
                                                      
      new Characteristic::Manufacturer("HomeSpan");   // Manufacturer of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::SerialNumber("123-ABC");    // Serial Number of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::Model("120-Volt Lamp");     // Model of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::FirmwareRevision("0.9");    // Firmware of the Accessory (arbitrary text string, and can be the same for every Accessory)

  // The last required Characteristic for the Accessory Information Service is the special Identify Characteristic.  We'll learn more about this
  // Characteristic in later examples.  For now, you can just instantiate it without any arguments.
  
      new Characteristic::Identify();                 // Create the required Identify

  // *NOTE* HAP requires that the AccessoryInformation Service always be instantiated BEFORE any other Services, which is why we created it first.

  // HAP also requires every Accessory (with the exception of those in Bridges, as we will see later) to implement the HAP Protocol Information Service.
  // This Service supports a single required Characteristic that defines the version number of HAP used by the device.
  // HAP Release R2 requires this version to be set to "1.1.0" 
  
    new Service::HAPProtocolInformation();          // Create the HAP Protcol Information Service  
      new Characteristic::Version("1.1.0");           // Set the Version Characteristicto "1.1.0" as required by HAP

  // Now that the required "informational" Services have been defined, we can finally create our Light Bulb Service

    new Service::LightBulb();                       // Create the Light Bulb Service
      new Characteristic::On();                       // This Service requires the "On" Characterstic to turn the light on and off

  // That's all that's needed to define a database from scratch, including all required HAP elements, to control a single lightbulb.
  // Of course this sketch does not yet contain any code to implement the actual operation of the light - there is nothing to
  // turn on and off.  But you'll still see a Light Bulb tile show up in your Home App with an ability to toggle it on and off.

} // end of setup()

//////////////////////////////////////

void loop(){

  // The code in setup above implements the Accessory Attribute Database, but performs no operations.  HomeSpan itself must be
  // continuously polled to look for requests from Controllers, such as the Home App on your iPhone.  The poll() method below is all that
  // is needed to perform this continuously in each iteration of loop()
  
  homeSpan.poll();         // run HomeSpan!
  
} // end of loop()

// Congratulations!  You've created your first HomeSpan sketch, ready to be uploaded to your ESP32 board and paired with HomeKit.
//
//
