/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2022 Gregg E. Berman
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

void setup() {

  // HomeSpan sends a variety of messages to the Serial Monitor of the Arduino IDE whenever the device is connected
  // to a computer.  Message output can be performed either by the usual Serial.print() or Serial.printf() functions,
  // or by one of three user macros: LOG0(), LOG1() and LOG2().  These three macros output messages to the Serial Monitor
  // depending on HomeSpan's Log Level setting:
  
  //    at a setting of 0, only LOG0() message are output; LOG1() and LOG2() messages are ignored
  //    at a setting of 1, both LOG0() and LOG1() messages are output; LOG2() messages are ignored
  //    at a setting of 2, all LOG0(), LOG1(), and LOG2() messages are output
  
  // Example 9 illustrates how to add such log messages.  The code is identical to Example 8 (without comments), except
  // that LOG0() and LOG1() messages have been added to DEV_LED.h.  The LOG0() messages will always be
  // output to the Arduino Serial Monitor.  The LOG1() messages will only be output if the Log Level is set to 1 or 2.

  // The setLogLevel() method of homeSpan can used to change the log level as follows:
  
  //    homeSpan.setLogLevel(0)     - sets Log Level to 0
  //    homeSpan.setLogLevel(1)     - sets Log Level to 1
  //    homeSpan.setLogLevel(2)     - sets Log Level to 2

  // The method should be called BEFORE homeSpan.begin() - see below for proper use.  Note that the Log Level
  // can also be changed dynamically during runtime via the HomeSpan CLI by typing 'L0', 'L1', or 'L2' into the Serial Monitor

  // There are two forms of the LOG0(), LOG1(), and LOG2() macros.  The first form takes only a single argument and outputs
  // messsges using the Serial.print(var) function.  This allows you to output any single variable or text message, but does not allow you
  // to control the format, or to output more than one variable at a time.  The second form take multiple arguments, where the first
  // is a standard C++ formatting string, and any remaining arguments are consumed according to the format string.  This form
  // utilizes the variadic Serial.printf(char *fmt [,var1, var2...]) function.
  
  // RECOMMENDATION: Since a HomeSpan ESP32 is meant to be physically connected to real-world devices, you may find
  // yourself with numerous ESP32s each configured with a different set of Accessories.  To aid in identification
  // you may want to add LOG0() statements containing some sort of initialization message to the constructors for
  // each derived Service, such as DEV_LED.  Doing so allows HomeSpan to "report" on its configuration upon start-up.  See
  // DEV_LED for examples.
  
  Serial.begin(115200);

  homeSpan.setLogLevel(1);                                // NEW - Sets Log Level to 1, which causes LOG1() messages to be output

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");
  
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();            
 
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();            
      new Characteristic::Name("Simple LED");
    new DEV_LED(16);

  new SpanAccessory();
    new Service::AccessoryInformation();    
      new Characteristic::Identify();               
      new Characteristic::Name("Dimmable LED");  
    new DEV_DimmableLED(17);                                                                                     

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
