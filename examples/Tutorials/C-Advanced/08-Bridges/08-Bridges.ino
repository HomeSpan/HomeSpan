
////////////////////////////////////////////////////////////
//                                                        //
//    HomeSpan: A HomeKit implementation for the ESP32    //
//    ------------------------------------------------    //
//                                                        //
// Example 8: HomeKit Bridges and Bridge Accessories      //
//            ** the preferred method for HomeSpan **     //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_LED.h"     
#include "DEV_Identify.h"       

void setup() {

  // Though we've seen in prior examples that one device can support multiple Accessories, HomeKit provides a more
  // general multi-Accessory framework that is somewhat more robust and easier to use: HomeKit Bridges.
  // A Bridge is a device that includes multiple Accessories, except that the FIRST defined Accessory contains
  // nothing but the AccessoryInformation Service and the HAPProtcolInformation Service.  When such a device is paired
  // to HomeKit, it is automatically recognized as a Bridge.  All of the other Accessories defined in the device are
  // associated with this Bridge.  If you unpair the Bridge from HomeKit, all associated Accessories are automatically
  // removed.
  //
  // Adding, editing, and deleting the other Accessories occurs in the same manner as before, but because the device
  // is paired as a Bridge, changes to the other Accessories is less likely to require you to un-pair and re-pair
  // the device.  HomeKit seems to be able to better process changes when they are done within a Bridge framework.
  //
  // One added bonus is that the HAPProtcolInformation Service only needs to be defined for the Bridge Accessory, and
  // does not need to be repeated for other Accessories.
  //
  // Example 8 is functionally identical to Example 7, except that instead of defined two Accessories (one for the on/off
  // LED and one for the dimmable LED), we define three Accessories, where the first acts as the Bridge.
  
  // As usual, all previous comments have been deleted and only new changes from the previous example are shown.

  // NOTE: To see how this works in practice, you'll need to unpair your device and re-pair it once the new code is loaded.
  
  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");       // CHANGED!  Note that we replaced Category::Lighting with Bridges (this changes the icon when pairing)

  // We begin by creating a Bridge Accessory, which look just like any other Accessory,
  // except that is only contains DEV_Identify (which is derived from AccessoryInformation)
  // and HAPProtcolInformation (required).  Note that HomeKit will still call the identify
  // update() routine upon pairing, so we specify the number of blinks to be 3.
  
  new SpanAccessory();  
    new DEV_Identify("Bridge #1","HomeSpan","123-ABC","HS Bridge","0.9",3);
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");

  // Now we simply repeat the definitions of the previous LED Accessories, as per Example 7, with two exceptions:
  // 1) We no longer need to include the HAPProtocolInformation Service.
  // 2) We will set the number of blinks to zero, so that only the bridge accessory will cause the Built-In
  //    LED to blink. This becomes especially important if you had 20 Accessories defined and needed to wait a
  //    minute or more for all the blinking to finish while pairing.
  
  new SpanAccessory(); 
    new DEV_Identify("On/Off LED","HomeSpan","123-ABC","20mA LED","0.9",0);    // CHANGED! The number of blinks is now set to zero

  //  new Service::HAPProtocolInformation();      - DELETED - NO LONGER NEEDED
  //    new Characteristic::Version("1.1.0");     - DELETED - NO LONGER NEEDED

    new DEV_LED(16);                // create an on/off LED attached to pin 16

  new SpanAccessory(); 

    new DEV_Identify("Dimmable LED","HomeSpan","123-ABC","20mA LED","0.9",0);    // CHANGED! The number of blinks is now set to zero
      
  //  new Service::HAPProtocolInformation();      - DELETED - NO LONGER NEEDED
  //    new Characteristic::Version("1.1.0");     - DELETED - NO LONGER NEEDED
  
    new DEV_DimmableLED(0,17);        // create a dimmable LED attached to pin 17 using PWM channel 0

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
