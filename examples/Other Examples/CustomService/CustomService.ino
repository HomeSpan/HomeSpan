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
 
#include "HomeSpan.h" 

// Apple's HomeKit does not provide any native services or characteristics for measuring atmospheric pressure.
// However, Eve for HomeKit does support pressure measurements.

// This brief sketch demonstrates how you can use HomeSpan's Custom Service and Custom Characteristic features
// to create a Pressure Sensor Accessory that will be recognized by the Eve for HomeKit App. Note that the
// Apple Home App will show this as a "Not Supported" Accessory Tile indicating it cannot be used in the Home App.
// However, this does not create any problems or errors in the Home App.

// Step 1:

//    Use the CUSTOM_SERV macro to create a new service named AtmosphericPressureSensor with
//    a UUID=E863F00A-079E-48FF-8F27-9C2605A29F52. This new service will be added to HomeSpan's Service namespace
//    and can be accessed using the fully-qualified name Service::AtmosphericPressureSensor.  The UUID specified
//    will not be recognized by Apple's Home App, but will be recognized by the Eve for HomeKit App. Note you
//    do NOT enclose either of the parameters in quotes!

  CUSTOM_SERV(AtmosphericPressureSensor, E863F00A-079E-48FF-8F27-9C2605A29F52);

// Step 2:

//    Use the CUSTOM_CHAR macro to create a new characteristic named AtmosphericPressure with
//    a UUID=E863F10F-079E-48FF-8F27-9C2605A29F52. This new characteristic will be added to HomeSpan's Characteristic namespace
//    and can be accessed using the fully-qualified name Characteristic::AtmosphericPressure.  The UUID specified will not be
//    recognized by Apple's Home App, but will be recognized by the Eve for HomeKit App. Note you do NOT enclose any of the
//    parameters in quotes!
//
//    The meaning of the parmameters are as follows:
//
//      PR+EV:    sets permission for "read" and "notify"
//      FLOAT:    sets the format to floating-point decimal number
//      1013:     sets the default starting value to 1013, which is 1 atm in millibars
//      700:      sets the default lower range of allowed values to 700 millibars
//      1200:     sets the default upper range of allowed values to 1200 millibars
//      false:    sets the "static range" flag to false, indicating that users CAN override the default range setRange() if desired

  CUSTOM_CHAR(AtmosphericPressure, E863F10F-079E-48FF-8F27-9C2605A29F52, PR+EV, FLOAT, 1013, 700, 1200, false);

// Now that AtmosphericPressureSensor and AtmosphericPressure have been created, they can be used just as any other native HomeSpan
// Service and Characteristic.

//////////////////////////////////////

struct PressureSensor : Service::AtmosphericPressureSensor {         // A standalone Air Pressure Sensor

  Characteristic::AtmosphericPressure pressure;                      // Eve Air Pressure with range 700-1200 hPa (millibars), where 1 atm=1013 hPa
  
  PressureSensor() : Service::AtmosphericPressureSensor{} {
        
    Serial.print("Configuring Air Pressure Sensor");                 // initialization message
    Serial.print("\n");

  } // end constructor

  void loop(){

    if(pressure.timeVal()>5000)                        // here we simulate an actual sensor by generating a random pressure reading every 5 seconds
      pressure.setVal((double)random(900,1100));
       
  } // end loop

}; // end PressureSensor

//////////////////////////////////////

void setup() {
 
  Serial.begin(115200);

  homeSpan.begin(Category::Sensors,"Eve Air Pressure");

  SPAN_ACCESSORY();  
    new PressureSensor();      

}

//////////////////////////////////////

void loop(){ 

  homeSpan.poll();  
}
