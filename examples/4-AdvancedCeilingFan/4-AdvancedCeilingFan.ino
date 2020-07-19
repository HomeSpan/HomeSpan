
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

  // Example 4 expands on Example 3 by adding Characteristics to set fan speed, fan direction, and light brightness.
  // For ease of reading, all prior comments have been removed and new comments added to show explicit changes from previous example.
 
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
      new Characteristic::On();                    
      new Characteristic::Name("Light Control");      //  Adding a Name Characteristic allows us to name this specific Service (see below)

    new Service::Fan();                             
      new Characteristic::Active();             
      new Characteristic::Name("Fan Control");        //  Adding a Name Characteristic allows us to name this specific Service (see below)

   // A Note about Names: HomeKit generally uses the name of the primary Service (first one defined) as the name to display on a Tile.  However, you can
   // set HomeKit (from within the Controller) to display each Service as its own tile.  In that case the names of the Light and Fan Tiles with both display
   // as "My Ceiling Fan."  By setting individual names for each Service you can create your own display names when Tiles are shown separately.

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();         // run HomeSpan!
  
} // end of loop()
