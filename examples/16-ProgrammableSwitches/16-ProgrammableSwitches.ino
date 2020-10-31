
////////////////////////////////////////////////////////////
//                                                        //
//    HomeSpan: A HomeKit implementation for the ESP32    //
//    ------------------------------------------------    //
//                                                        //
// Example 16: Stateless Programmable Switches            //
//             * using linked-services                    //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_ProgButton.h"     
#include "DEV_Identify.h"       

void setup() {

  
  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");

  new SpanAccessory();  
    new DEV_Identify("Bridge #1","HomeSpan","123-ABC","HS Bridge","0.9",3);
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");

  new SpanAccessory();                                                          
    new DEV_Identify("PushButton Switch","HomeSpan","123-ABC","20mA LED","0.9",0);

    new DEV_ProgButton(23);
 
} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
