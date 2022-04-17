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
// Example 4: A variable-speed ceiling fan with           //
//            dimmable ceiling light                      //
//                                                        //
////////////////////////////////////////////////////////////


#include "HomeSpan.h"         // Always start by including the HomeSpan library

void setup() {

  // Example 4 expands on the first Accessory in Example 3 by adding Characteristics to set FAN SPEED, FAN DIRECTION, and LIGHT BRIGHTNESS.
  // For ease of reading, all prior comments have been removed and new comments added to show explicit changes from the previous example.
 
  Serial.begin(115200); 

  homeSpan.begin(Category::Fans,"HomeSpan Ceiling Fan");  

  new SpanAccessory();                            
  
    new Service::AccessoryInformation();                
      new Characteristic::Identify();                        

    new Service::LightBulb();                      
      new Characteristic::On(true);            // NEW: Providing an argument sets its initial value.  In this case it means the LightBulb will be turned on at start-up

    // In addition to setting the initial value of a Characteristic, it is also possible to override the default min/max/step range specified by HAP.
    // We do this with the setRange() method:
    
    // setRange(min, max, step), where
    //
    // min = minimum allowed value
    // max = maximum allowed value
    // step = step size (can be left blank, in which case the HAP default is retained)

    // The setRange() method can be called on any numerical-based Characteristic that supports range overrides.  The easiest way to apply to method is to call it right
    // after instantiating a new Characteristic.  Don't forget to surround the "new" command in parentheses when chaining a method in this fashion.
    
    // Here we create a Brightness Characteristic to set the brightness of the LightBulb with an initial value of 50% and an allowable range
    // from 20-100% in steps of 5%.  See Notes 1 and 2 below for more details:
    
      (new Characteristic::Brightness(50))->setRange(20,100,5);    

    new Service::Fan();                             
      new Characteristic::Active();             
      new Characteristic::RotationDirection();                        // NEW: This allows control of the Rotation Direction of the Fan
      (new Characteristic::RotationSpeed(50))->setRange(0,100,25);    // NEW: This allows control of the Rotation Speed of the Fan, with an initial value of 50% and a range from 0-100 in steps of 25%

  // NOTE 1: Setting the initial value of the Brightness Characteristic to 50% does not by itself cause HomeKit to turn the light on to 50% upon start-up.
  // Rather, this is governed by the initial value of the On Characteristic, which in this case happens to be set to true.  If it were set to false,
  // or left unspecified (default is false) then the LightBulb will be off at start-up.  However, it will jump to 50% brightness as soon as turned on
  // for the first time.  This same logic applies to the Active and RotationSpeed Characteristics for a Fan.

  // NOTE 2: The default range for Characteristics that support a range of values is specified in HAP Section 9.  For Brightness, the range defaults
  // to min=0%, max=100%, step=1%.  Using setRange() to change the minimum Brightness from 0% to 20% (or any non-zero value) provides for a better
  // HomeKit experience.  This is because the LightBulb power is controlled by the On Characteristic, and allowing Brightness to be as low as 0%
  // sometimes results in HomeKit turning on the LightBulb but with Brightness=0%, which is not very intuitive.  This can occur when asking Siri
  // to lower the Brightness all the way, and then turning on the LightBulb.  By setting a minumum value of 20%, HomeKit always ensures that there is
  // some Brightness value whenever the LightBulb is turned on.

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();         // run HomeSpan!
  
} // end of loop()
