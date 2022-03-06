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
// Example 19: Web Logging                                //
//                                                        //
////////////////////////////////////////////////////////////


#include "HomeSpan.h" 
#include "DEV_LED.h"

void setup() {

// This is a duplicate of Example 5 (Two Working LEDs) with the addition of HomesSpan Web Logging

  Serial.begin(115200);

// Below we enable Web Logging.  The first parameter sets the maximum number of log messages to save (as the
// log fills with messages, older ones are replaced by newer ones).  The second parameter specifies a Timer Server
// that HomeSpan calls to set its clock.  Setting the clock in this fashion is optional, and you can leave this
// argument blank (or set to NULL) if you don't care about setting the absolute time of the device.  The third
// argument defines the Time Zone used for setting the device clock.  See the HomeSpan API Reference for complete details
// and more options related to this function call.

  homeSpan.enableWebLog(10,"pool.ntp.org","UTC");

// The rest of the sketch below is identical to Example 5.  All of the Web Logging occurs in DEV_LED.h  
  
  homeSpan.begin(Category::Lighting,"HomeSpan LEDs");
  
  new SpanAccessory(); 
  
    new Service::AccessoryInformation(); 
      new Characteristic::Name("LED #1"); 
      new Characteristic::Manufacturer("HomeSpan"); 
      new Characteristic::SerialNumber("123-ABC"); 
      new Characteristic::Model("20mA LED"); 
      new Characteristic::FirmwareRevision("0.9"); 
      new Characteristic::Identify();            
      
    new Service::HAPProtocolInformation();      
      new Characteristic::Version("1.1.0");     

  //  In Example 2 we instantiated a LightBulb Service and its "On" Characteristic here.  We are now going to replace these two lines (by commenting them out)...

  //  new Service::LightBulb();                   
  //    new Characteristic::On();                 

  // ...with a single new line instantiating a new class we will call DEV_LED():

    new DEV_LED(16);        // this instantiates a new LED Service.  Where is this defined?  What happpened to Characteristic::On?  Keep reading...

  // The full definition and code for DEV_LED is implemented in a separate file called "DEV_LED.h" that is specified using the #include at the top of this program.
  // The prefix DEV_ is not required but it's a helpful convention when naming all your device-specific Services. Note that DEV_LED will include all the required
  // Characterictics of the Service, so you DO NOT have to separately instantiate Characteristic::On --- everything HomeSpan needs for DEV_LED should be implemented
  // in DEV_LED itself (though it's not all that much).  Finally, note that we created DEV_LED to take a single integer argument.  If you guessed this is
  // the number of the Pin to which you have attached an LED, you'd be right.  See DEV_LED.h for a complete explanation of how it works.

  new SpanAccessory(); 
  
    new Service::AccessoryInformation();    
      new Characteristic::Name("LED #2");    
      new Characteristic::Manufacturer("HomeSpan"); 
      new Characteristic::SerialNumber("123-ABC");  
      new Characteristic::Model("20mA LED");   
      new Characteristic::FirmwareRevision("0.9");  
      new Characteristic::Identify();               
      
    new Service::HAPProtocolInformation();          
      new Characteristic::Version("1.1.0");         

  //  new Service::LightBulb();                       // Same as above, this line is deleted...
  //    new Characteristic::On();                     // This line is also deleted...
  
    new DEV_LED(17);                                  // ...and replaced with a single line that instantiates a second DEV_LED Service on Pin 17

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
