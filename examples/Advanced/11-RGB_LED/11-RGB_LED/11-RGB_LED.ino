
////////////////////////////////////////////////////////////
//                                                        //
//    HomeSpan: A HomeKit implementation for the ESP32    //
//    ------------------------------------------------    //
//                                                        //
// Example 9: Logging messages to the Serial Monitor      //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_LED.h"     
#include "DEV_Identify.h"       

void setup() {

  // Example 11 illustrates how to control an RGB LED to set any color and brightness.
  // The config below should look familiar by now.  We've created a new derived Service,
  // call RgbLED to house all the required logic.  You'll find all the code in DEV_LED.h.
  
  Serial.begin(115200);

  homeSpan.begin(Category::Lighting,"HomeSpan LEDs");


  new SpanAccessory();  
    new DEV_Identify("Bridge #1","HomeSpan","123-ABC","HS Bridge","0.9",3);
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");

  new SpanAccessory();                                                          
    new DEV_Identify("LED Blinker","HomeSpan","123-ABC","20mA LED","0.9",0);
    new DEV_RgbLED(0,1,2,32,22,23);                                                      // An RGB LED requires three PWM channels and three pins to be specified
      
} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
