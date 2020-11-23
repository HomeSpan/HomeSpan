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
// Example 12: Service Loops (and Event Notifications)    //
//             * implementing a Temperature Sensor        //
//             * implementing an Air Quality Sensor       //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_Identify.h"      
#include "DEV_Sensors.h" 

void setup() {

  // So far we've seen that HomeSpan allows you to create derived Services with their own constructors and update() methods.  For many applications, this
  // will be all that is needed.  However, for many other types of applications you may need to take action or perform some background operations without
  // any prompting or requests from HomeKit.

  // To perform background operations and actions, every Service implements a loop() method.  The default loop() method is to do nothing, which has been
  // fine for all our prior examples.  But if you need to perform some continuous background action, all you need to do is implement a loop() method for
  // your derived Service.  At the end of each HomeSpan polling cycle, the loop() method is called for each Service that implements its own code.
  // In this fashion, the loop() method is similar to the main loop() method in the Arduino IDE itself - except it can be customized for each Service.

  // In this Example 12 we explore the use of loop() methods to implement two new accessories - a Temperature Sensor and an Air Quality Sensor.  Of course
  // we won't actually have these physical devices attached to the ESP32 for the purpose of this example, but we will simulate "reading" their properties.
  // This is one of the main purposes of implementing a loop() method.  It allows you to read a sensor or perform some sort of repetitive, Service-specific
  // action.

  // Once you read (or simulate reading) a sensor's values in a loop() method, you need to somehow communicate this back to HomeKit so the new values can be 
  // reflected in the HomeKit Controller.  This may be strictly for information purposes (such as a temperature sensor) or could be used by HomeKit itself
  // to trigger other devices (as might occur if implementing a Door Sensor).

  // Fortunately, HomeSpan makes communicating the values of Characteristics back to HomeKit easy.  In prior examples we saw how getVal() and getNewVal()
  // are used to read current and updated Characteristic values requested by HomeKit.  To perform the reverse, we simply use a method called setVal().
  // Setting the value of a Characteristic with this function does two things.  First, it causes HomeSpan to send an Event Notification message back to HomeKit
  // letting HomeKit know the new value of the Characteristic.  Since messages create network traffic, HomeSpan keeps track of all setVal() changes across
  // all Services and creates one a single Event Notification message reporting all the changes togther at the end of each polling cycle.

  // The second thing that HomeSpan does when you change the value of a Characteristic with setVal() is to reset an internal timer for that Characteristic that
  // keeps track of how long it's been since the last modification, whether from a previous setVal() instruction, or by HomeKit itself via a call to update().
  // You can query the time since the last modificaton using the method timeVal() which returns the elapsed time in milliseconds.  By calling this function from 
  // within loop() you can determine when it's time for a new sensor read, or when to perform some other action.

  // NOTE: It it NOT recommended to continuously change Characteristic values using setVal() as this will generate a lot of network traffic since HomeSpan
  // sends Event Notifications bck to all registered HomeKit Controllers.  It's fine to perform internal calculations, generate signals on different pins,
  // and perform any other internal actions you may need as frequently as you require.  But limit the use of setVal() to a reasonable frequency, such as maybe
  // one per minute for a temperature sensor.  Do not use setVal() unless the value of the Characteristic changes, but do use it to immediately inform HomeKit of
  // something time-sensitive, such as a door opening, or a smoke alarm triggering.

  // As usual, all of the logic for this example are encapsulated in new standalone derived Services.  You'll find fully-commented definitions for the DEV_TempSensor() and 
  // the DEV_AirQualitySensor() Services instantiated below, in the DEV_Sensors.h file.  As noted, this example is for instructional purposes only -- we do not actually
  // connect a Temperature Sensor or Air Quality Sensor to our ESP32 device.  As such, we did not define the Services to take any arguments to specify pin numbers or any
  // other information needed to implement an actual sensor.  Instead, in order to see how real a device would work, we simulate periodic changes by modifying Characteristic
  // values using setVal() with either a sequence of repeating values, or random values.  See DEV_Sensors.h for complete details.

  // Once you understand these examples, you should be able to use implement your own loop() method and utilize setVal() along with timeVal() for any combination of
  // HomeKit Services with Characteristics that require your device to send periodic update messages to HomeKit Controllers, ranging from Smoke Alarms to Door Sensors.
  
  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");

  new SpanAccessory();  
    new DEV_Identify("Bridge #1","HomeSpan","123-ABC","HS Bridge","0.9",3);
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");
      
  new SpanAccessory();                                                          
    new DEV_Identify("Temp Sensor","HomeSpan","123-ABC","Sensor","0.9",0);
    new DEV_TempSensor();                                                                // Create a Temperature Sensor (see DEV_Sensors.h for definition)

  new SpanAccessory();                                                          
    new DEV_Identify("Air Quality","HomeSpan","123-ABC","Sensor","0.9",0);
    new DEV_AirQualitySensor();                                                          // Create an Air Quality Sensor (see DEV_Sensors.h for definition)

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()

//////////////////////////////////////


// Additional Technical Notes about Event Notifications in HomeKit
// ---------------------------------------------------------------

// HomeKit is designed for two-way communication: HomeSpan devices not only receive and act on operational instructions from HomeKit Controllers, but
// HomeSpan can also send HomeKit unsolicited messages regarding changes to the state of the device.  Though it may not be apparent, this has already been
// ocurring in the background in all prior examples.  This is because when a HomeKit Controller sends an operational request to any HomeKit device, it expects
// to receive a status message back indicating whether the request was successful or not.  This is the purpose of returning StatusCode:OK in custom update()
// methods.  With this information returned, HomeKit can update its own status and properly reflect a change in the device, such as by showing a light is now
// turned on instead of off.  However, HomeKit unfortunately does NOT inform any other HomeKit Controllers of this new information.  So if you have two iPhones
// and use one to turn on a light, this iPhone does not relay a message to the second iPhone that a light has been turned on.  This is the case even
// if you are using an AppleTV or HomePod as a central hub for HomeKit.

// Normally this does not matter much, since the second iPhone will automatically update itself as to the status of all HomeKit devices as soon as the HomeKit
// application is launched on that iPhone.  It does this by sending every HomeKit device a message asking for a status update.  In this fashion the second
// iPhone quickly synchronizes itself as soon as its HomeKit app is opened, but ONLY when it is first opened (or re-opened if you first close it).  However, if you
// have two iPhones BOTH opened to the HomeKit app (or one iPhone and one Mac opened to the HomeKit app) and you use one Controller app to turn on a light, the
// resulting change in status of that light will NOT be reflected in the second Controller app, unless you close tha app and re-open (at which point it goes
// through the request procedure discussed above).  This can be very annoying and counterintuitive.

// Fortunately, HomeKit provides a solution to this in the form of an Event Notification protcol.  This protcol allows a device to send unsoliciated messages
// to all Controllers that have previously registered themselves with the device indicating the Characteristics for which they would like to receive an event
// message from the device whenever there is a change in the status of one or more of those Characteristics.

// HomeSpan takes care of this automatically, and has being doing so in the background in all prior examples.  To see this for yourself, use two iPhones
// (or an iPhone and Mac) with any of the previous examples and open the HomeKit app on both.  Any changes you make to the device using one of the Controllers,
// such as turning on an LED, is immediately reflected in the other Controller.  Not quite magic, but close.

// As described above, and fully explored in this example, HomeSpan automatically generates Event Notification messages and transmits them to all registered
// Controllers every time you change the value of a Characteristic using the setVal() function from within a derived Service's loop() method.
