
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
 
  Serial.begin(115200);      // Start a serial connection - this is needed for you to type in your WiFi credentials

  homeSpan.begin(Category::Lighting,"HomeSpan LightBulbs");  // Begin a HomeSpan Session - note the name is now "HomeSpan LightBulbs"

  // Here we create the first LightBulb Accessory just as in Example 1

  new SpanAccessory();                            // Begin by creating a new Accessory using SpanAccessory(), which takes no arguments 
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service, which has 6 required Characteristics
      new Characteristic::Name("My Table Lamp");      // Name of the Accessory, which shows up on the HomeKit "tiles", and should be unique across Accessories
      new Characteristic::Manufacturer("HomeSpan");   // Manufacturer of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::SerialNumber("123-ABC");    // Serial Number of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::Model("120-Volt Lamp");     // Model of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::FirmwareRevision("0.9");    // Firmware of the Accessory (arbitrary text string, and can be the same for every Accessory) 
      new Characteristic::Identify();                 // Create the required Identify  
      
    new Service::HAPProtocolInformation();          // Create the HAP Protcol Information Service  
      new Characteristic::Version("1.1.0");           // Set the Version Characteristicto "1.1.0" as required by HAP

    new Service::LightBulb();                       // Create the Light Bulb Service
      new Characteristic::On();                       // This Service requires the "On" Characterstic to turn the light on and off

  // Now we create a second Accessory, which is just a duplicate of Accessory 1 with the exception of changing the Name from "My Table Lamp" to "My Floor Lamp"

  new SpanAccessory();                            // Begin by creating a new Accessory using SpanAccessory(), which takes no arguments 
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service, which has 6 required Characteristics
      new Characteristic::Name("My Floor Lamp");      // Name of the Accessory, which shows up on the HomeKit "tiles", and should be unique across Accessories
      new Characteristic::Manufacturer("HomeSpan");   // Manufacturer of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::SerialNumber("123-ABC");    // Serial Number of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::Model("120-Volt Lamp");     // Model of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::FirmwareRevision("0.9");    // Firmware of the Accessory (arbitrary text string, and can be the same for every Accessory) 
      new Characteristic::Identify();                 // Create the required Identify  
      
    new Service::HAPProtocolInformation();          // Create the HAP Protcol Information Service  
      new Characteristic::Version("1.1.0");           // Set the Version Characteristicto "1.1.0" as required by HAP

    new Service::LightBulb();                       // Create the Light Bulb Service
      new Characteristic::On();                       // This Service requires the "On" Characterstic to turn the light on and off

  // That's it - our device now has two Accessories!

  // IMPORTANT: You should NOT have to re-pair your device with HomeKit when moving from Example 1 to Example 2.  HomeSpan will note
  // that the Attribute Database has been updated, and will broadcast a new configuration number when the program restarts.  This should
  // cause all iOS and MacOS HomeKit Controllers to automatically update and reflect the new configuration above.

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();         // run HomeSpan!
  
} // end of loop()
