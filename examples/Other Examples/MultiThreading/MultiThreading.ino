/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2025 Gregg E. Berman
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
 
// This sketch provides a variation of Example 1 (a Simple LightBulb Accessory) that demonstrates
// the use of multi-threaded functionality to periodically flip the power of the light from a thread
// outside of the main HomeSpan polling process.  You will be able to turn on/off the LightBulb
// Accessory from the Home App as usual, but every 10 seconds the light will flip state automatically
// (turning it ON if it is OFF, or turning it OFF if it is ON).

// Note this example does not implement an actual LED, just the logic to show how things work from 
// within the Home App.

// This sketch can be used with both single- and dual-processor devices.

#include "HomeSpan.h"

Characteristic::On *power;                  // NEW!  Create a global pointer to the On Characterstic (to be used below)

//////////////////////////////////////

void setup() {
 
  Serial.begin(115200);

  homeSpan.begin(Category::Lighting,"HomeSpan LightBulb");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
            
    new Service::LightBulb();
      power = new Characteristic::On();     // NEW!  Save the pointer to the Characteristic in the global variable, power

  homeSpan.autoPoll();                      // NEW!  Start autopolling.  HomeSpan will run its polling process in separate thread
}

//////////////////////////////////////

void loop(){

  // NOTE: we DO NOT call homeSpan.poll() from this loop() since we already started polling in a separate thread above by calling homeSpan.autoPoll()

  delay(10000);         // sleep for 10 seconds - note this has no effect on HomeSpan since the polling process is in a different thread

  Serial.printf("*** Flipping power of Light!\n");

  homeSpanPAUSE;                        // temporarily pause the HomeSpan polling process
  power->setVal(1-power->getVal());     // flip the value of the On Characteristic using the pointer we saved above
  
} // note once at the end of the loop() code block HomeSpan polling automatically resumes (no need to call homeSpanRESUME)
  
