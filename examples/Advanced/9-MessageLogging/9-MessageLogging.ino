
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

  // HomeSpan sends a variety of messages to the Serial Monitor of the Arduino IDE whenever the device is connected
  // to a computer.  Message output is performed either by the usual Serial.print() function, or by one of two macros,
  // LOG1() and LOG2().  These two macros are defined as Serial.print() or as no operation (), depending on the
  // level of the VERBOSITY constant specified in the "Settings.h" file.  Setting VERBOSITY to 0 sets both LOG1() and
  // LOG2() to no-op, which means only messages explicitly sent with Serial.print() will be output by HomeSpan.  Setting
  // VERBOSITY to 1 means messages formed by the LOG1() macros will also be sent.  And setting VERBOSITY to 2 causes
  // both LOG1() and LOG2() messages to be sent.
  //
  // You can create your own log messages as needed through Serial.print() statements, but you can also create them with
  // the LOG1() or LOG2() macros enabling you can turn them on or off by setting VERBOSITY to the appropriate level.
  // Use LOG1() and LOG2() just as you would Serial.print().
  //
  // Example 9 illustrates how to add such log messages.  The code is identical to Example 8 (without comments), except
  // that Serial.print() and LOG1() messages have been added to DEV_LED.h.  The Serial.print() messages will always be
  // output to the Arduino Serial Monitor.  The LOG1() messages will only be output if VERBOSITY is set to 1 or 2.
  //
  // RECOMMENDATION: Since a HomeSpan ESP32 is meant to be physically connected to real-world devices, you may find
  // yourself with numerous ESP32s each configured with a different set of Accessories.  To aid in identification
  // you may want to add Serial.print() statements containing some sort of initialization message to the constructors for
  // each derived Service, such as DEV_LED.  Doing so allows HomeSpan to "report" on its configuration upon start-up.  See
  // DEV_LED for examples.
  
  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");
  
  // Defines the Bridge Accessory

  new SpanAccessory();  
    new DEV_Identify("Bridge #1","HomeSpan","123-ABC","HS Bridge","0.9",3);
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");
  
  // Defines an ON/OFF LED Accessory attached to pin 16

  new SpanAccessory();                                                          
    new DEV_Identify("On/Off LED","HomeSpan","123-ABC","20mA LED","0.9",0);
    new DEV_LED(16);

  // Defines a Dimmable LED Accessory attached to pin 17 using PWM channel 0
  
  new SpanAccessory();                                                        
    new DEV_Identify("Dimmable LED","HomeSpan","123-ABC","20mA LED","0.9",0);       
    new DEV_DimmableLED(0,17);                                            

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
