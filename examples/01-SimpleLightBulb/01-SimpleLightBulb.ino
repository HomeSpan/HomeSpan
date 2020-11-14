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
  
  // This first example introduces the HomeSpan library and demonstrates how to implement a simple HomeKit on/off light control
  // using a combination of HomeSpan Accessory, Service, and Characteristic objects.  Once this sketch has been uploaded
  // to your HomeSpan device and the device is paired to your home, a new "lightbulb" tile will appear in the Home App of your iPhone,
  // iPad, or Mac.
  
  // Though the tile will be fully operational (i.e. you can change the status of the lightbulb from "on" or "off"), we won't yet connect
  // an actual light or LED to the HomeSpan device, so nothing real will light up.  Instead, in this and the next few examples, we'll focus
  // on learning about the different ways HomeKit controls can be configured.  Starting in Example 5, we'll connect an LED to the device
  // and introduce the methods you'll need to implement to actually turn the LED on and off from your Home App.

  // These examples are best understood when reviewed in conjunction with the documentation provided on the HomeSpan GitHub page.
  // See  https://github.com/HomeSpan/HomeSpan for details and references.  The examples also make frequent reference to
  // Apple's HomeKit Accessory Protocol Specification, known as HAP.  You can download this directly from Apple
  // at https://developer.apple.com/support/homekit-accessory-protocol.

  // LET'S GET STARTED...

#include "HomeSpan.h"         // HomeSpan sketches always begin by including the HomeSpan library

void setup() {                // Your HomeSpan code should be placed within the standard Arduino setup() function
 
  Serial.begin(115200);       // Start a serial connection so you can receive HomeSpan diagnostics and control the device using HomeSpan's Command-Line Interface (CLI)

  // The HomeSpan library creates a global object named "homeSpan" that encapsulates all HomeSpan functionality.
  // The begin() method is used to initialize HomeSpan and start all HomeSpan processes.
  
  // Required parameters are Category and Name, which are used by HomeKit to configure the icon and name of the device shown in your Home App
  // when initially pairing your device.  A list of all defined categories can be found at https://github.com/HomeSpan/HomeSpan/docs/Categories.md
  // and match those specified by Apple in Section 13 of the HAP guide (which of course you have downloaded as recommended above!).

  // HomeSpan's category names are defined in a C++ namespace appropriately called Category, so you'll need to use the prefix Category:: when
  // specifying categories.

  homeSpan.begin(Category::Lighting,"HomeSpan LightBulb");   // initializes a HomeSpan device named "HomeSpan Lightbulb" with Category set to Lighting

  // Next, some general information about Apple HomeKit before we proceed...
  
  // Every HomeKit device consists of one or more Accessories.  Each Accessory contains one or more Services, and
  // every Service contains one or more Characteristics.  HAP defines all allowable Services and specifies which Characteristics
  // are required or optional for each Service.
  
  // An Accessory is typically a complete appliance, such as a table lamp or ceiling fan.  Services are the main components of the
  // appliance - a ceiling fan Accessory will typically have a fan Service and a light bulb Service.  Characteristics define
  // how each Service operates.
  
  // Some Characteristics are read-only and describe the name or properties of a Service.  Other Characteristics
  // can be both written and read by HomeKit - these are the interesting ones since they enable actions to occur,
  // such as turning on or off a light, or setting its brightness.
  
  // HAP also requires various informational Services that describe the overall Accessory. 
  
  // HAP calls the entirety of all Accessories, Services, and Characteristics used in a device the "Accessory Attributes Database" of
  // that device.  A complete list of HomeKit Services can be found in Section 8 of the HAP guide.  The subset of those Services that
  // have been implemented in HomeSpan can be found at https://github.com/HomeSpan/HomeSpan/docs/ServiceList.md. and are defined in the
  // Services namespace.  A complete list of all HomeKit Characteristics used by these Services can be found in Section 9 of the HAP guide,
  // and are defined by HomeSpan in the Characteristics namespace.
  
  // Users construct a HomeKit device's Accessory Attribute Database by instantiating one or more Accessories, each with their own
  // HAP Services and HAP Characteristics.  To make this as easy as possible, HomeSpan self-registers each object and assembles the database
  // in the order in which you instantiate the objects.  You do not need to create variables for any of the objects nor know anything about
  // their underlying HAP codes.  HomeSpan takes care of all of this for you.

  // For this example, our Database will comprise a single Accessory containing 3 Services, each with their own required Characteristics

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

  // HAP also requires every Accessory (with the exception of those in Bridges, as we will see later) to implement the HAP Protocol Information Service.
  // This Serrvice supports a single required Characteristic that defines the version number of HAP used by the device.
  // HAP Release R2 requires this version to be set to "1.1.0" 
  
    new Service::HAPProtocolInformation();          // Create the HAP Protcol Information Service  
      new Characteristic::Version("1.1.0");           // Set the Version Characteristicto "1.1.0" as required by HAP

  // Now that the required "informational" Services have been defined, we can finally create our Light Bulb Service
  // NOTE: The order of the Services is not important - we could have created the LightBulb first.

    new Service::LightBulb();                       // Create the Light Bulb Service
      new Characteristic::On();                       // This Service requires the "On" Characterstic to turn the light on and off

  // That's all that's needed to define a database from scratch, including all required HAP elements, to control a single lightbulb.
  // Of course this sketch does not yet contain any code to implement the actual operation of the light - there is nothing to
  // turn on and off.  But you'll still see a Light Bulb tile show up in your Home App with an ability to toggle it on and off.

} // end of setup()

//////////////////////////////////////

void loop(){

  // The code in setup above implements the Accessory Attribute Database, but performs no operations.  HomeSpan itself must be
  // continuously polled to look for requests from Controllers, such as an iOS or MacOS device.  The poll() method below is all that
  // is needed to perform this continuously in each iteration of loop()
  
  homeSpan.poll();         // run HomeSpan!
  
} // end of loop()

// Congratulations!  You've created your first HomeSpan sketch, ready to be uploaded to your ESP32 board and paired with HomeKit.
//
//
