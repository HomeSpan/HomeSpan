
////////////////////////////////////////////////////////////
//                                                        //
//    HomeSpan: A HomeKit implementation for the ESP32    //
//    ------------------------------------------------    //
//                                                        //
// Example 13: Target States and Current States           //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_Identify.h"      
#include "DEV_DoorsWindows.h" 

void setup() {

  
  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");


  new SpanAccessory();  
    new DEV_Identify("Bridge #1","HomeSpan","123-ABC","HS Bridge","0.9",3);
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");
      
  new SpanAccessory();                                                          
    new DEV_Identify("Garage Door","HomeSpan","123-ABC","Door","0.9",0);
    new DEV_GarageDoor();                                                               // Create a Garage Door Opener (see DEV_DoorsWindows.h for definition)

  new SpanAccessory();                                                          
    new DEV_Identify("Window Shade","HomeSpan","123-ABC","Shade","0.9",0);
    new DEV_WindowShade();                                                              // Create a motorized Window Shade (see DEV_DoorsWindows.h for definition)

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
