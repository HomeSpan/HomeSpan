
////////////////////////////////////////////////////////////
//                                                        //
//    HomeSpan: A HomeKit implementation for the ESP32    //
//    ------------------------------------------------    //
//                                                        //
// Example 4: A variable-speed ceiling fan with           //
//            dimmable ceiling light                      //
//                                                        //
////////////////////////////////////////////////////////////


#include "HomeSpan.h"         // Always start by including the HomeSpan library

void setup() {

  // Example 4 expands on Example 3 by adding Characteristics to set FAN SPEED, FAN DIRECTION, and LIGHT BRIGHTNESS.
  // For ease of reading, all prior comments have been removed and new comments added to show explicit changes from the previous example.
 
  Serial.begin(115200); 

  homeSpan.begin(Category::Fans,"HomeSpan Ceiling Fan");  

  new SpanAccessory();                            
  
    new Service::AccessoryInformation();            
      new Characteristic::Name("My Ceiling Fan");     
      new Characteristic::Manufacturer("HomeSpan");   
      new Characteristic::SerialNumber("123-ABC");    
      new Characteristic::Model("120-Volt Lamp");     
      new Characteristic::FirmwareRevision("0.9");    
      new Characteristic::Identify();                 
      
    new Service::HAPProtocolInformation();         
      new Characteristic::Version("1.1.0");        

    new Service::LightBulb();                      
      new Characteristic::On(true);             // NEW: Providing an argument sets its initial value.  In this case it means the LightBulb will be turned on at start-up
      new Characteristic::Brightness(50);       // NEW: This allows control of the Brightness of the LightBulb, with an initial value of 50% upon start-up (Note 1)
        new SpanRange(20,100,5);                  // NEW: This sets the range of the Brightness to be from a min of 20%, to a max of 100%, in steps of 5% (Note 2)

    new Service::Fan();                             
      new Characteristic::Active();             
      new Characteristic::RotationDirection();  // NEW: This allows control of the Rotation Direction of the Fan
      new Characteristic::RotationSpeed(25);    // NEW: This allows control of the Rotation Speed of the Fan, with an initial value of 25% upon start-up (Note 1)       
        new SpanRange(0,100,25);                 // NEW: This sets the range of the Rotation Speed to be from a min of 0%, to a max of 100%, in steps of 25%


  // NOTE 1: Setting the initial value of the Brightness Characteristic to 50% does not by itself cause HomeKit to turn the light on to 50% upon start-up.
  // Rather, this is governed by the initial value of the On Characteristic, which in this case happens to be set to true.  If it were set to false,
  // or left unspecified (default is false) then the LightBulb will be off at start-up.  However, it will jump to 50% brightness as soon as turned on
  // for the first time.  This same logic applies to the Active and RotationSpeed Characteristics for a Fan.

  // NOTE 2: The default range for Characteristics that support a range of values is specified in the HAP Section 9.  For Brightness, the range defaults
  // to min=0%, max=100%, step=1%.  SpanRange(min,max,step) can be used to over-ride this default.  SpanRange is generic and can be used wih other Characteristics
  // that support a range of values, such as RotationSpeed.  Whenever a new SpanRange is defined it is applied to the most recently defined Characteristic.
  // It only has an effect if that Characteristic utilizes ranges.

  // RECOMMENDATION:  Using SpanRange to change the minimum Brightness from 0% (the default) to 20% (or any non-zero value) provides for a better
  // HomeKit experience.  This is because the LightBulb power is controlled by the On Characteristic, and allowing Brightness to be as low as 0%
  // sometimes results in HomeKit turning on the LightBulb but with Brightness=0%, which is not very intuitive.  This can occur when asking Siri
  // to lower the Brightness all the way, and turnign on the LightBulb.  By setting a minumum value of 20%, HomeKit always ensures that there is
  // some Brightness value whenever the LightBulb is turned on.

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();         // run HomeSpan!
  
} // end of loop()
