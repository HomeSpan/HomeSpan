/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020 Gregg E. Berman
 *  
 *  https://github.com/HomeSpan/HomeSpan
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *  
 ********************************************************************************/
 
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
  // LOG1() and LOG2().  These two macros call Serial.print() depending on HomeSpan's Log Level setting.  A setting
  // of 0 means that LOG1() and LOG2() messages are ignored.  A setting of 1 causes HomeSpan to print LOG1() messages
  // to the Serial Monitor, but ignores LOG2() message.  And a setting of 2 causes HomeSpan to print both LOG1() and
  // LOG2() messages.
  
  // Example 9 illustrates how to add such log messages.  The code is identical to Example 8 (without comments), except
  // that Serial.print() and LOG1() messages have been added to DEV_LED.h.  The Serial.print() messages will always be
  // output to the Arduino Serial Monitor.  The LOG1() messages will only be output if the Log Level is set to 1 or 2.

  // The setLogLevel() method of homeSpan is used to change the log level as follows:
  
  //    homeSpan.setLogLevel(0)     - sets Log Level to 0
  //    homeSpan.setLogLevel(1)     - sets Log Level to 1
  //    homeSpan.setLogLevel(2)     - sets Log Level to 2

  // The method should be called BEFORE homeSpan.begin() - see below for proper use.
  
  // RECOMMENDATION: Since a HomeSpan ESP32 is meant to be physically connected to real-world devices, you may find
  // yourself with numerous ESP32s each configured with a different set of Accessories.  To aid in identification
  // you may want to add Serial.print() statements containing some sort of initialization message to the constructors for
  // each derived Service, such as DEV_LED.  Doing so allows HomeSpan to "report" on its configuration upon start-up.  See
  // DEV_LED for examples.
  
  Serial.begin(115200);

  homeSpan.setLogLevel(1);                                // NEW - Sets Log Level to 1, which causes LOG1() messages to be output

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

  // Defines a Dimmable (PWM-driven) LED Accessory attached to pin 17
  
  new SpanAccessory();                                                        
    new DEV_Identify("Dimmable LED","HomeSpan","123-ABC","20mA LED","0.9",0);       
    new DEV_DimmableLED(17);                                                                                      

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
