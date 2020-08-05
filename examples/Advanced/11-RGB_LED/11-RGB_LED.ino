
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
  // The configuration below should look familiar by now.  We've created a new derived Service,
  // call RgbLED to house all the required logic.  You'll find all the code in DEV_LED.h.
  // For completeness, this configuration also contains an on/off LED and a dimmable LED as shown
  // in prior examples.
  
  Serial.begin(115200);

  homeSpan.begin(Category::Lighting,"HomeSpan LEDs");


  new SpanAccessory();  
    new DEV_Identify("Bridge #1","HomeSpan","123-ABC","HS Bridge","0.9",3);
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");

  new SpanAccessory();                                                          
    new DEV_Identify("On/Off LED","HomeSpan","123-ABC","20mA LED","0.9",0);
    new DEV_LED(16);                                                                     // Create an On/Off LED attached to pin 16

  new SpanAccessory();                                                          
    new DEV_Identify("Dimmable LED","HomeSpan","123-ABC","20mA LED","0.9",0);
    new DEV_DimmableLED(0,17);                                                           // Create a Dimmable LED using PWM channel 0, attached to pin 17

  new SpanAccessory();                                                          
    new DEV_Identify("RGB LED","HomeSpan","123-ABC","20mA LED","0.9",0);
    new DEV_RgbLED(1,2,3,32,22,23);                                                      // Create an RGB LED using PWM channels 1,2,3, attached to pins 32,22,23 (for R, G, and B LED anodes)
      
} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
