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
// Example 7: Transforming AccessoryInformation into a    //
//            derived Service that implements the         //
//            Identify Characteristic                     //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_LED.h"     
#include "DEV_Identify.h"         // NEW! This is where we store all code for the DEV_Identify Service     

void setup() {

  // In Example 5 we saw how to create a derived Service to encapsulate all the functionality needed to implement DEV_LED
  // in it's own DEV_LED.h file. Then, in Example 6 we extended that further by implementing DEV_DimmableLED.  In this
  // example we do the same for the AccessoryInformation Service. Note how AccessoryInformation, and all of its
  // Characteristics, need to be defined for every Accessory.  By deriving a new Service that implements a multi-argument
  // constructor we can avoid having to separately create each required Characteristic every time.  Creating a derived Service
  // also allows us to implement device-specific code for the Identify Characteristic.  We will call this derived Service
  // DEV_Identify, and store its code in "DEV_Identify.h" which has already been included above.

  // As usual, all previous comments have been deleted and only new changes from the previous example are shown.

  // NOTE: To see how this works in practice, you'll need to unpair your device and re-pair it once the new code is loaded.
  // This will allow oyu to activate the identify routines.
  
  Serial.begin(115200);

  homeSpan.begin(Category::Lighting,"HomeSpan LEDs");
  
  new SpanAccessory(); 

  // Rather than instantiate the AccessoryInformation Service and all of it's required Characteristics,
  // we'll delete these line (comment them out)...
  
  //  new Service::AccessoryInformation(); 
  //    new Characteristic::Name("On/Off LED"); 
  //    new Characteristic::Manufacturer("HomeSpan"); 
  //    new Characteristic::SerialNumber("123-ABC"); 
  //    new Characteristic::Model("20mA LED"); 
  //    new Characteristic::FirmwareRevision("0.9"); 
  //    new Characteristic::Identify();            

  // ...and replace them with this single line that implements everything above.  See DEV_Identify.h for
  // details on how this is defined.  Note there is an extra argument at the end we set to 3.
  // This optional argument will be used to run the identify routine (see code for details)

    new DEV_Identify("On/Off LED","HomeSpan","123-ABC","20mA LED","0.9",3);    // NEW!  This implements all the Characteristics above

    new Service::HAPProtocolInformation();      
      new Characteristic::Version("1.1.0");     

    new DEV_LED(16);                // create an on/off LED attached to pin 16 (same as in Example 5)

  new SpanAccessory(); 

  // Same as above, we can replace all of this...
  
  //  new Service::AccessoryInformation();    
  //    new Characteristic::Name("Dimmable LED");    
  //    new Characteristic::Manufacturer("HomeSpan"); 
  //    new Characteristic::SerialNumber("123-ABC");  
  //    new Characteristic::Model("20mA LED");   
  //    new Characteristic::FirmwareRevision("0.9");  
  //    new Characteristic::Identify();               

  // ...with this (note we set last argument to 5 this time - see code for what this does)

    new DEV_Identify("Dimmable LED","HomeSpan","123-ABC","20mA LED","0.9",5);    // NEW!  This implements all the Characteristics above
      
    new Service::HAPProtocolInformation();          
      new Characteristic::Version("1.1.0");         
  
    new DEV_DimmableLED(17);        // create a dimmable (PWM-driven) LED attached to pin 17

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
