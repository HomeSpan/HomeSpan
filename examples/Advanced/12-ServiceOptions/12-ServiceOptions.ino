
////////////////////////////////////////////////////////////
//                                                        //
//    HomeSpan: A HomeKit implementation for the ESP32    //
//    ------------------------------------------------    //
//                                                        //
// Example 12: Service Options:                           //
//             * setting the Primary Service              //
//             * setting Service Names                    //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_LED.h"     
#include "DEV_Identify.h"       

void setup() {

  // Example 12...
  
  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");

  new SpanAccessory();  
    new DEV_Identify("Bridge #1","HomeSpan","123-ABC","HS Bridge","0.9",3);
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");

  new SpanAccessory();                                                          
    new DEV_Identify("Ceiling Fan #1","HomeSpan","123-ABC","20mA LED","0.9",0);
    new DEV_DimmableLED(0,17,ServiceType::Primary);    
    new Service::Fan();                             
      new Characteristic::Active();             
      new Characteristic::RotationDirection();
      new Characteristic::RotationSpeed(0);

  new SpanAccessory();                                                          
    new DEV_Identify("Ceiling Fan #2","HomeSpan","123-ABC","20mA LED","0.9",0);
    new DEV_DimmableLED(0,17);    
    new Service::Fan(ServiceType::Primary);                             
      new Characteristic::Active();             
      new Characteristic::RotationDirection();
      new Characteristic::RotationSpeed(0);
      
  new SpanAccessory();                                                          
    new DEV_Identify("Ceiling Fan #3","HomeSpan","123-ABC","20mA LED","0.9",0);
    new DEV_DimmableLED(0,17);    
      new Characteristic::Name("Main Light");
    new DEV_LED(16);    
      new Characteristic::Name("Night Light");
    new Service::Fan(ServiceType::Primary);                             
      new Characteristic::Active();             
      new Characteristic::RotationDirection();
      new Characteristic::RotationSpeed(0);
      new Characteristic::Name("Fan");

  new SpanAccessory();                                                          
    new DEV_Identify("Ceiling Fan #4","HomeSpan","123-ABC","20mA LED","0.9",0,ServiceType::Primary);
    new DEV_DimmableLED(0,17);    
      new Characteristic::Name("Main Light");
    new DEV_LED(16);    
      new Characteristic::Name("Night Light");
    new Service::Fan();                             
      new Characteristic::Active();             
      new Characteristic::RotationDirection();
      new Characteristic::RotationSpeed(0);
      new Characteristic::Name("Fan");


} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
