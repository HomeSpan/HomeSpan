
////////////////////////////////////////////////////////////
//                                                        //
//    HomeSpan: A HomeKit implementation for the ESP32    //
//    ------------------------------------------------    //
//                                                        //
// Example 10: Timed Resets - emulating a "pushbutton"    //
//             in HomeKit                                 //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_Blinker.h"     
#include "DEV_Identify.h"       

void setup() {

   // Though HomeKit and the HomeKit Accessory Protocol (HAP) Specification provide a very flexible framework
   // for creating iOS- and MacOS-controlled devices, they does not contain every possible desired feature.
   //
   // One very common Characteristic HomeKit does not seem to contain is a simple pushbutton, like the type you
   // would find on a remote control.  Unlike switches that can be "on" or "off", a pushbutton has no state.
   // Rather, a pushbutton performs some action when it's pushed, and that's all it does until it's pushed
   // again.
   //
   // Though HomeKit does not contain such a Characteristic, it's easy to emulate in HomeSpan.  To do so, simply
   // define a Service with a boolen Characteristic (such as the On Characteristic), and create an update()
   // method to peform the operations to be executed when the "pushbutton" is "pressed".  The update() method
   // should ignore the newValue requested by HomeKit, since the only thing that matters is that update() is called.
   // 
   // You could stop there and have something in HomeKit that acts like a pushbutton, but it won't look like a
   // pushbutton because every time you press the tile for your device in HomeKit, the Controller will toggle
   // between showing it's on and showing it's off.  Pressing a tile that shows the status is already on, in order
   // to cause HomeKit to trigger the update() to perform a new action, is not very satisfying.
   //
   // Ideally, we'd like HomeKit to acknowledge you've pressed the tile for the device, maybe by lighting up for a
   // second or so, and then it should reset to the "off" position.  This would emulate a light-up pushbutton.
   //
   // Fortunately, HomeSpan includes a way of doing exactly this, using an object called SpanTimedReset().  Similar
   // to SpanRange(), you create a new SpanTimedReset() object with a single argument representing the number of
   // milliseconds HomeSpan should wait before telling HomeKit to reset, or "turn off", the device tile it just turned
   // on when you pressed it.  How does SpanTimedReset() know which Characteristic it should attach itself to?
   // Similar to all other HomeSpan objects, SpanTimedReset() attaches to the last object you instantiated (and
   // will throw an error message at start-up if you try to instantiate a new SpanTimedReset() object without having just
   // instantiated a boolean Characteristic of some type).
   //
   // In Example 10 below we create a single pushbutton that blinks an LED three times.  This is not very useful, but
   // you can think about the LED as an IR LED that is transmitting a Volume-Up command to a TV, or an RF signal to
   // some remote device, like a ceiling fan.
   //
   // All the functionality is wrapped up in a newly-defined "DEV_Blinker" Service, which can be found in DEV_Blinker.h.
   // This new Service is a copy of the DEV_LED service we've been working so far, with modifications to make it into 
   // a generic blinking LED.  As usual, changes and new lines are notably commented.
  
  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");
  
  // Defines the Bridge Accessory
 
  new SpanAccessory();  
    new DEV_Identify("Bridge #1","HomeSpan","123-ABC","HS Bridge","0.9",3);
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");
  
  // *** NEW *** defines an LED Blinker Accessory attached to pin 16 which blinks 3 times

  new SpanAccessory();                                                          
    new DEV_Identify("LED Blinker","HomeSpan","123-ABC","20mA LED","0.9",0);
    new DEV_Blinker(16,3);                                                      // DEV_Blinker takes two arguments - pin, and number of times to blink

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
