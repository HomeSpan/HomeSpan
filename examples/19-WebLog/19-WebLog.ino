/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2022 Gregg E. Berman
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
// Example 19: Web Logging with time-keeping              //
//                                                        //
////////////////////////////////////////////////////////////


#include "HomeSpan.h" 
#include "DEV_LED.h"

void setup() {

// This is a duplicate of Example 5 (Two Working LEDs) with the addition of HomeSpan Web Logging

  Serial.begin(115200);

// Below we enable Web Logging.  The first parameter sets the maximum number of log messages to save.  As the
// log fills with messages, older ones are replaced by newer ones.  The second parameter specifies a Timer Server
// that HomeSpan calls to set the device clock.  Setting the clock is optional, and you can leave this
// argument blank (or set to NULL) if you don't care about setting the absolute time of the device.  The third
// argument defines the Time Zone used for setting the device clock.  The fourth argument specifies the URL page
// of the Web Log.  See the HomeSpan API Reference for complete details, as well as additional options, related
// to this function call.

  homeSpan.enableWebLog(10,"pool.ntp.org","UTC","myLog");           // creates a web log on the URL /HomeSpan-[DEVICE-ID].local:[TCP-PORT]/myLog

// The full URL of the Web Log will be shown in the Serial Monitor at boot time for reference.
// The Web Log output displays a variety of device parameters, plus any log messages you choose
// to provide with the WEBLOG() macro (see DEV_LED.h)

// Note the rest of the sketch below is identical to Example 5.  All of the Web Logging occurs in DEV_LED.h  
  
  homeSpan.begin(Category::Lighting,"HomeSpan LEDs");           // Note we can set Category to Lighting even if device is configured as a Bridge
  
  new SpanAccessory(); 
    new Service::AccessoryInformation(); 
      new Characteristic::Identify();               

  new SpanAccessory(); 
    new Service::AccessoryInformation(); 
      new Characteristic::Identify();               
      new Characteristic::Name("LED #1"); 
    new DEV_LED(16);

  new SpanAccessory();   
    new Service::AccessoryInformation();    
      new Characteristic::Identify();                       
      new Characteristic::Name("LED #2");      
    new DEV_LED(17);

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
