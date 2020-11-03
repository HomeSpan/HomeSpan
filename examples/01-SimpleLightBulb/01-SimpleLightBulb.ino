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
// Example 1: A non-functioning on/off light bulb         //
//            constructed from basic HomeSpan components  //
//                                                        //
////////////////////////////////////////////////////////////


#include "HomeSpan.h"         // Always start by including the HomeSpan library

void setup() {
 
  Serial.begin(115200);      // Start a serial connection - this is needed for you to type in your WiFi credentials

  // Begin a HomeSpan Session.  Required parameters are Category and Name.
  // These are used by HomeKit to configure the icon and name of the device shown when initially pairing, as well
  // as set the icon (but not the name) of defined Accessories in some cases.
  // There are no other effects.  You can even specify a "Lighting" Category for a Faucet with no ill effects (except the icons)
  // A complete list of Categories can be found in Settings.h, which is based on Section 13 of Apple's
  // HomeKit Accessory Protocol (HAP) Specifications Document.

  homeSpan.begin(Category::Lighting,"HomeSpan LightBulb");                             

  // Every HomeKit device consists of one or more Accessories.  Each Accessory contains one or more Services.
  // Every Service contains one or more Characteristics.  HAP defines all allowable Services and Characteristics,
  // including those that are required and those that are optional.  An Accessory is typically a complete appliance,
  // such as a table lamp or ceiling fan.  Services are the main components of the appliance - a ceiling fan Accessory will
  // typically have a fan Service and a light bulb Service.  Characteristics define how each Service operates.
  
  // Some Characteristics are read-only and describe the name or properties of a Service.  Other Characteristics
  // can be both written and read by HomeKit - these are the interesting ones since they enable actions to occur,
  // such as turning on or off a light, or setting its brightness.
  
  // HAP also requires various informational Services that describe the overall Accessory. 
  
  // HAP calls the entirety of all Accessories, Services, and Characteristics the "Accessory Attributes Database."
  // A complete list of HAP Services and Characteristics implemented in HomeSpan can be found in Services.h, which is
  // based on HAP Section 8 (Services) and HAP Section 9 (Characteristics).
  
  // Users construct the Accessories database in HomeSpan by using a combination of new SpanAccessory, new Services (which point to underlying
  // SpanServices), and new Characteristics (which point to underlying SpanCharacteristics).  The database is assembled in the
  // order in which components are defined.  A new Service will be implemented in the last new Accessory defined, and a new
  // Characteristic will be implemented in the last new Service defined.  Indention helps convey this structure though is
  // of course not required.

  /////////////////////////////////

  // For this example, our Database will comprise a single Accessory containing 3 Services, each with their own required Characteristics

  new SpanAccessory();                            // Begin by creating a new Accessory using SpanAccessory(), which takes no arguments
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service, which has 6 required Characteristics
      new Characteristic::Name("My Table Lamp");      // Name of the Accessory, which shows up on the HomeKit "tiles", and should be unique across Accessories
      
  // The next 4 Characteristics serve no function except for being displayed in HomeKit's setting panel for each Accessory.  They are nevertheless required by HAP:
                                                      
      new Characteristic::Manufacturer("HomeSpan");   // Manufacturer of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::SerialNumber("123-ABC");    // Serial Number of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::Model("120-Volt Lamp");     // Model of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::FirmwareRevision("0.9");    // Firmware of the Accessory (arbitrary text string, and can be the same for every Accessory)

  // The last required Characteristic for the Accessory Information Service allows the user to identify the Characteristic and requires
  // some implementation code (such as blinking an LED, or flashing the light).  HomeSpan defaults to take no action if there is no
  // implementation code, so we can simply create the Identify Characteristic for now and let HomeSpan default to no action.
  
      new Characteristic::Identify();                 // Create the required Identify

  // HAP requires every Accessory (with the exception of those in Bridges) to implement the HAP Protocol Information Service.
  // This Serrvice supports a single required Characteristic that defined the version number of HAP used by the device.
  // HAP Release R2 requires this version to be set to "1.1.0" 
  
    new Service::HAPProtocolInformation();          // Create the HAP Protcol Information Service  
      new Characteristic::Version("1.1.0");           // Set the Version Characteristicto "1.1.0" as required by HAP

  // Now that the required "informational" Services have been defined, we can finally create the Light Bulb Service
  // NOTE: The order of the Services is not important - we could have created the LightBulb first.

    new Service::LightBulb();                       // Create the Light Bulb Service
      new Characteristic::On();                       // This Service requires the "On" Characterstic to turn the light on and off

  // That's all that's needed to define a database from scratch, including all required HAP elements, to control a single lightbulb.
  // Of course the database itself does not contain any code to implement the actual operation of the light - there is nothing to
  // turn on and off.  But you'll still see a Light Bulb tile show up in HomeKit with an ability to toggle it on and off.  In the next
  // example we will add the code that turns on and off an LED.  For now, upload this sketch to your ESP32, pair with HomeKit, and
  // verify everything works.

} // end of setup()

//////////////////////////////////////

void loop(){

  // The code in setup above implements the Accessory Attribute Database, but performs no operations.  HomeSpan itself must be
  // continuously polled to look for requests from Controllers, such as an iOS or MacOS device.  The poll() method below is all that
  // is needed to perform this continuously in each iteration of loop()
  
  homeSpan.poll();         // run HomeSpan!
  
} // end of loop()
