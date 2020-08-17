
////////////////////////////////////////////////////////////
//                                                        //
//    HomeSpan: A HomeKit implementation for the ESP32    //
//    ------------------------------------------------    //
//                                                        //
// Example 14: Target States and Current States           //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_Identify.h"      
#include "DEV_DoorsWindows.h" 

void setup() {

  // HomeKit is designed for two-way communication: HomeSpan devices not only receive and act on operational instructions from HomeKit Controllers, but
  // HomeSpan can also send HomeKit unsolicited messages regarding changes to the state of the device.  Though it may not be apparent, this has already been
  // ocurring in the background in all prior examples.  This is because when a HomeKit Controller sends an operational request to any HomeKit device, it expects
  // to receive a status message back indicating whether the request was successful or not.  This is the purpose of returning StatusCode:OK in custom update()
  // methods.  With this information returned, HomeKit can update its own status and properly reflect a change in the device, such as by showing a light is now
  // turned on instead of off.  However, HomeKit unfortunately does NOT inform any other HomeKit Controllers of this new information.  So if you have two iPhones
  // and use one to turn on a light, the other first iPhone does not relay a message to the second iPhone that a light has been turned on.  This is the case even
  // if you are using an AppleTV or HomePod as a central hub for HomeKit.

  // Normally this does not matter much, since the second iPhone will naturally update itself as to the status of all HomeKit devices as soon as the HomeKit
  // application is launched on that iPhone.  It does this by sending every HomeKit device a message asking for a status update.  In this fashion the second
  // iPhone quickly synchronizes itself as soon as the HomeKit app is opened, but ONLY when it is first opened (or re-opened if you first close it).  But if you
  // have two iPhones BOTH opened to the HomeKit app (or one iPhone and one Mac opened to the HomeKit app) and you use one Controller app to turn on a light, the
  // resulting change in status of that light will NOT be reflected in the second Controller app, unless you close tha app and re-open (at which point it goes
  // through the request procedure discussed above).  This is very annoying and counterintuitive.

  // Fortunately, HomeKit provides a solution to this in the form of an Event Notification protcol.  This protcol allows a device to send unsoliciated messages
  // to all Controllers that have previously registered themselves with the device indicating the Characteristics for which they would like to receive an event
  // message from the device whenever there is a change in the status of one or more of those Characteristics.

  // The good news is that HomeSpan takes care of this automatically.  To see this for yourself, use two iPhones (or an iPhone and Mac) with any of the previous examples
  // and open the HomeKit app on both.  Any changes you make to the device using one of the Controllers, such as turning on an LED, is immediately reflected
  // in the other Controller.  Not quite magic, but close.

  // A different use of Event Notifications was also working behind in the scenes in Example 10 - Timed Resets.  In this case, HomeSpan sent an unsolited Event message
  // to all registered Controllers letting them know that a device that was previously turned on, is now in fact turned off.

  // In this Example 13 we explore the explicit use of Event Notifications to support Services that require constants updates from the device to all HomeKit Controllers.
  // The two Services we will use below are a Temperature Sensor and an Air Quality Sensor.  Neither of these Services have any operational controls.  They cannot be
  // turn on or off, or operated in any way.  As such, they do not need to implement an update() method, since HomeKit Controllers will never ask them to change
  // any of their Characteristics.

  // Rather, HomeKit is expecting to get periodic Event Notification messages from such Services so that the HomeKit Controllers can accurately reflect the status
  // and values of the Characteristics for those Services, such as the temperature, in the HomeKit Controller.

  // There are two steps to accomplishing this.  The first is to implement an event() method for each Service that uses a setVal() function to change the values
  // for one or more Characteristics for that Service.  The second step is to instantiate a new SpanEvent() object for each Service that you want HomeSpan to invoke your
  // event() method.  The SpanEvent object take only one argument - the number of milliseconds to wait between calls to a Service's event() method.

  // As usual, all of the logic for this is encapsulated in new standalone derived Services.  You'll find fully-commented definitions for the DEV_TempSensor() and 
  // the DEV_AirQualitySensor() Services instantiated below, in the DEV_Sensors.h file.  Note that this example is for instructional purposes only -- we do not actually
  // connect a Temperature Sensor or Air Quality Sensor to our ESP32 device.  As such, we did not define the Services to take any arguments to specify pin numbers or any
  // other information needed to implement an actual sensor.  Instead, in order to see how real a device would work, we will send Event messages by manufacturing simulated
  // updates.  See DEV_Sensors.h for complete details.

  // Once you understand these examples, you should be able to use Event Notifications for any combination of HomeKit Services with Characteristics that require your device to
  // send periodic update messages to HomeKit Controllers, ranging from Smoke Alarms to Door Sensors.
  
  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");


  new SpanAccessory();  
    new DEV_Identify("Bridge #1","HomeSpan","123-ABC","HS Bridge","0.9",3);
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");
      
  new SpanAccessory();                                                          
    new DEV_Identify("Garage Door","HomeSpan","123-ABC","Door","0.9",0);
    new DEV_GarageDoor();                                                               // Create a Garage Door Opener (see DEV_DoorsWindows.h for definition)

  new SpanAccessory();                                                          
    new DEV_Identify("Window Shade","HomeSpan","123-ABC","Shade","0.9",0);
    new DEV_WindowShade();                                                               // Create a motorized Window Shade (see DEV_DoorsWindows.h for definition)

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
