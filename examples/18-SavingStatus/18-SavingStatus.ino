/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2021-2022 Gregg E. Berman
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
// Example 18: Saving Characteristic Status in NVS        //
//             * saving the state of two dimmable LEDs    //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_LED.h"     

void setup() {

  // In many of the prior examples we saw how Characteristics are initialized when first instantiated.  You can either include an argument:
  //
  //    new Characteristic::Brightness(25);
  //
  // in which case the value of the Brightness Characterisrtic is set to 25 when HomeSpan is powered up, or you can leave the argument blank:
  //
  //    new Characteristic::Brightness();
  //
  // in which case HomeSpan will apply a default value.

  // These methods work fine, with the exception that if the HomeSpan device loses power, it will boot up according to the parameters above rather
  // than remembering the state of each Characteristic after you've made any changes via the Home App or with any PushButtons.

  // In this Example 18 we will see how to instruct HomeSpan to automatically save the values of one or more Characteristics in non-volatile storage (NVS)
  // so that they can be restored to their latest state if the power is cycled.  To do so, we call the constructor for a Characteristic with TWO arguments as such:
  //
  //    new Characteristic::Brightness(25, true);
  //
  // This instructs HomeSpan to set the Brightness to 25 the very first time the device is powered on, but to SAVE any changes to this Characteristic
  // in NVS, AND RESTORE the last-saved value whenever the power is cycled!
  
  // Note that though HomeSpan takes care of all the saving and restoring automatically for any Characteristic in which you set the second argument of
  // the constructor to be "true," HomeSpan can't automatically perform any needed initialization of the physical appliance by itself.  In other words,
  // if you change the Brightness to 55 from the Home App and then sometime later the device loses power, HomeSpan will restore the value of the
  // Brightness Characteristic to 55 on start-up, but you'll need to add some code to set the brightness of the actual LED once the value is restored.
  
  // To see how this works in practice, we'll configure HomeSpan to operate two Dimmable LEDs, each with its own on/off PushButton.  As usual, all the code
  // is implemented in DEV_LED.h, with comments highlighting all the new features.  See DEV_LED.h for full details.
  
  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");

  new SpanAccessory();  
    new Service::AccessoryInformation();
      new Characteristic::Identify();

  new SpanAccessory();                                                          
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      new Characteristic::Name("LED 1");   
    new DEV_DimmableLED(17,19);                                         // The first argument specifies the LED pin; the second argument specifies the PushButton pin
 
  new SpanAccessory();                                                          
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      new Characteristic::Name("LED 2");  
    new DEV_DimmableLED(16,18);                                         // The first argument specifies the LED pin; the second argument specifies the PushButton pin

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()

//////////////////////////////////////

// OPERATING NOTES
//
// When the values of Characteristics are saved in NVS, they are stored based on a unique key that combines the UUID of the Characteristic with its AID and IID.
// If you are actively developing a configuration, adding or subtracting a new SpanAccessory or SpanService can alter the AID and IID of other Characteristics whose
// values were already stored in the NVS.  If the new UUID/AID/IID combination is unused, the previously-stored value will not be restored upon the very next
// start-up and instead the value specified in the first argument of the constructor will be used and stored in the NVS as the initial value.
//
// If the new UUID/AID/IID happens to match a combination that was previously used, the value of the Characteristic will restored to whatever is found under that key
// in the NVS.
//
// *** To clear all values stored in the NVS, type 'V' in the HomeSpan CLI.  This ensures that there are no stray key/value pairs in the NVS from prior iterations of your
// configuration.
//
