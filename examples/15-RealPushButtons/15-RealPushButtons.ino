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
// Example 15: Real PushButtons                           //
//             * manually controlling a Dimmable LED      //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_LED.h"     

void setup() {

  // In Example 14 we saw how to emulate a PushButton tile within HomeKit by automatically resetting a Characteristic so that
  // it "turns off" after a short period of time.  However, sometimes we want to be able to physically control a device with actual
  // PushButtons (or momentary switches) that trigger an action, such as turning on a light or fan, or opening a garage door.
  // Additionally, we want HomeKit to reflect any changes in the device as a result of such manual actions - HomeKit should know
  // when the light has been turned on or off manually.

  // One way to accomplish would be via custom code added to the loop() method of your derived Service that monitors a pushbutton,
  // checks when it is pressed, debounces button noise, performs some actions when pressed, and informs HomeKit of the actions with
  // the setVal() method. Or you can simply use HomeSpan's built-in SpanButton() object.

  // SpanButton() is a Service-level object, meaning it attaches itself to the last Service you define.  Typically you would instantiate
  // one of more SpanButton() objects directly inside the constructor for your derived Service.
  
  // SpanButton() supports three types of a triggers: a SINGLE button press, a DOUBLE press, and a LONG (extended) press.
  
  // The length of the presses needed to trigger these different types can be specified by optional arguments to SpanButton().
  // Since most buttons create spurious noise when pressed (and then again when released), the default time to trigger a SINGLE press is 5ms.
  // It's fine to change this to a longer value, but a shorter value is not recommended as this may allow spurious triggers unless
  // you debounce your switch with hardware.
  
  // The SpanButton() constructor takes 5 arguments, in the following order:
  //
  //  pin         - the pin number to which the PushButton is attached (required)
  //  longTime    - the minimum length of time (in milliseconds) the button needs to be pushed to be considered a LONG press (optional; default=2000 ms)
  //  singleTime  - the minimum length of time (in milliseconds) the button needs to be pushed to be considered a SINGLE press (optional; default=5 ms)
  //  doubleTime  - the maximum length of time (in milliseconds) between button presses to create a DOUBLE press (optional; default=200 ms)
  //  triggerType - the action that causes a trigger on the pin (optional; default=SpanButton::TRIGGER_ON_LOW).  Built-in choices include:
  //
  //      SpanButton::TRIGGER_ON_LOW:   used for a button that connects pin to GROUND
  //      SpanButton::TRIGGER_ON_HIGH:  used for a button that connects pin to VCC (typically +3.3V)
  //      SpanButton::TRIGGER_ON_TOUCH: used when a pin is connected to a touch pad/sensor

  // When a SpanButton() is first instantiated, HomeSpan configures the specified pin in accordance with the triggerType chosen.

  // Then, HomeSpan continuously polls all pins with associated SpanButton() objects and checks for triggers, which indicates the button was
  // pressed, but not yet released.  It then starts a timer.  If the button is released after being pressed for less than singleTime milliseconds,
  // nothing happens.  If the button is released after being pressed for more than singleTime milliseconds, but for less than longTime milliseconds,
  // a SINGLE press is triggered, unless you press once again within doubleTime milliseconds to trigger a DOUBLE press.  If the button is held for more
  // than longTime milliseconds without being released, a LONG press is triggered. Once a LONG press is triggered the timer resets so that if you keep
  // holding the button, another LONG press will be triggered in another longTime milliseconds.  This continues until you finally release the button.    

  // Note if you set longTime > singleTime, SpanButton() will only trigger LONG presses.  Also, if you set doubleTime to zero, SpanButton() will not be
  // able to trigger a DOUBLE press.

  // To use SpanButton() within a derived Service you need to implement a button() method.  Similar to the loop() method, your button()
  // method will typically contain some combination of getVal() functions and setVal() functions, along with code that performs some set
  // of actions on the physical device (seting pins high or low, turning on fans, etc).  However, in contrast to the loop() method, which
  // is called by HomeSpan every polling cycle, HomeSpan only calls the button() method when a button attached to the Service registers a
  // SINGLE, DOUBLE, or LONG press.

  // Also in contrast with the loop method, the button() method takes two 'int' arguments, and should defined as follows:
  //
  //    void button(int pin, int pressType)
  //
  // where "pin" is the pin number of the PushButton that was triggered, and pressType is set to 0 for a SINGLE press, 1 for a DOUBLE press,
  // and 2 for a LONG press.  You can also use the pre-defined constants SpanButton::SINGLE, SpanButton::DOUBLE, and SpanButton::LONG in place
  // of the numbers 0, 1, and 2 (this is recommended, though you will see in Example 16 why these integers can't be replaced by an C++ enum class).
  
  // Of course you can replace the variables "pin" and "pressType" with your own names.  The only requirement is the definition conform to
  // the "void button(int, int)" signature.  When HomeSpan first starts up it checks all Services containing one or more SpanButton() instances to
  // ensure you've implemented your own button(int, int) method.  If not, HomeSpan will print a warning message on the Serial Monitor.  Nothing bad
  // happens if you instantiate a SpanButton() but forget to create the button() method, or you create it with the wrong parameters. But nothing good
  // happens either - button presses are just ignored.
  //
  // C++ Note:  For an extra check, you can also place the the contextual keyword "override" after your method definition as such:
  //
  //    void button(int buttonPin, int pressType) override {...your code...}
  //
  // Doing so allows the compiler to check that you are indeed over-riding the base class button() method and not inadvertently creating a new
  // button() method with an incorrect signature that will never be called by SpanButton().  In fact, you could add "override" to the definition
  // of your update() and loop() methods as well, since these are always supposed to over-ride the base-class method.

  // To demonstrate how SpanButtons works in practice, we will implement a Dimmable LED starting with the same LED code use in Example 11,
  // but with 3 SpanButton() objects performing different functions that showcase the different types of presses.
  //
  //  * A "power" SpanButton that will toggle the power in response a SINGLE press, turn on the power and set the brightness to a "favorite" level
  //    in response to the DOUBLE press, and set a new "favorite" level in response to a LONG press.
  //
  //  * A "raise brightness" SpanButton that will increase the brightness by 1% in response to a SINGLE press, repeatedly increase the brightness
  //    by 10% in response to a LONG press, and jump to the maximum brightness in response to a DOUBLE press.
  //
  //  * A "lower brightness" SpanButton that will decrease the brightness by 1% in response to a SINGLE press, repeatedly decrease the brightness
  //    by 10% in response to a LONG press, and jump to the minimum brightness in response to a DOUBLE press.
  
  // As usual, all the code is implemented in DEV_LED.h, with NEW! comments highlighting changes from Example 11.  You'll also notice that we've
  // extended the constructor for this version of our derived Dimmable LED Service to include the pin numbers for each of our buttons.
  // See DEV_LED.h for details.
  
  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");

  new SpanAccessory();  
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 

  new SpanAccessory();                                                          
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      new Characteristic::Name("PushButton LED");
    
    new DEV_DimmableLED(17,23,5,18);          // NEW! added three extra arguments to specify the pin numbers for three SpanButtons() - see DEV_LED.h
 
} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()

////////////////  ADDITIONAL NOTES ////////////////////////
  
  // DEFAULT VALUES AND ALTERNATIVE CONSTRUCTORS
  // --------------------------------------------
  
  // As shown in this example, the following creates a SpanButton suitable for connecting pin 23 to GROUND via a pushbutton, and uses
  // SpanButton's default values for longTime, singleTime, and doubleTime:
  //
  //    new SpanButton(23);
  //
  // This is exactly the same as if you explicitly set each parameter to its default value:
  //
  //    new SpanButton(23,2000,5,200,SpanButton::TRIGGER_ON_LOW);  // equivalent to above
  //    
  // If instead you want to create a SpanButton that connects pin 23 to VCC via a pushbutton using SpanButton::TRIGGER_ON-HIGH,
  // you need to explictly set all the other parameters, even if you are satisfied with their default values, since triggerType
  // is the last argument in the constructor:
  //
  //    new SpanButton(23,2000,5,200,SpanButton::TRIGGER_ON_HIGH);
  //
  // Because this can be cumbersome, SpanButton includes an alternative constructor where triggerType is the second paramater, instead
  // of the last.  In this case triggerType is required, but longTime, singleTime, and doubleTime are still optional.
  //
  // For example, the following creates a SpanButton suitable for connecting pin 23 to a touch pad/sensor, and uses
  // SpanButton's default values for longTime, singleTime, and doubleTime:  
  //
  //    new SpanButton(23,SpanButton::TRIGGER_ON_TOUCH);
  //
  // which is of course equivalent to:
  //
  //    new SpanButton(23,SpanButton::TRIGGER_ON_TOUCH,2000,5,200);

  
  // TOUCH PAD/SENSOR CALIBRATION
  // ----------------------------
  
  // SpanButton makes use of the ESP32's internal touch sensor peripheral to monitor pins for "touches".  There are a number
  // of paramaters that must be specified for touches to be accurately detected, depending on the exact size and shape of your
  // touch pads.  Upon instantiation of a SpanButton() with triggerType=SpanButton::TRIGGER_ON_TOUCH, SpanButton will conveniently
  // perform an automatic calibration that sets an appropriate threshold level for detecting touches.
  //
  // However, if you need to, you can override this calibration process using the following two class-level functions:
  //
  //    SpanButton::setTouchThreshold()    - explicitly sets the threshold for detecting touches (i.e. overrides the auto-calibration)
  //    SpanButton::setTouchCycles()       - explicitly sets the measurement and sleep times used by the ESP32's internal touch peripheral
  //
  // See the SpanButton secion of the Reference API for details on how to use these optional functions.


  // THE triggerType FUNCTION
  // -------------------------
  
  // Though the three triggerType objects supported by SpanButton (SpanButton::TRIGGER_ON_LOW, etc.) may appear to be nothing more than
  // constants, they are actually boolean functions that each accept a single integer argument.  When SpanButton calls the triggerType function,
  // it passes the pin number specified in the constructor as the integer argument, and the triggerType function returns TRUE if the
  // "pushbutton" associated with the pin number is "pressed," or FALSE if it is not.
  //
  // For example, the definitions of SpanButton::TRIGGER_ON_LOW and SpanButton::TRIGGER_ON_HIGH are as follows:
  //
  //    boolean TRIGGER_ON_LOW(int pinArg)  { return( !digitalRead(pinArg) ); }
  //    boolean TRIGGER_ON_HIGH(int pinArg) { return(  digitalRead(pinArg) ); }
  //
  // The definitions for SpanButton::TRIGGER_ON_TOUCH are more complicated since the ESP32 touch sensor library returns either a 2-byte
  // or 4-byte numeric value when the state of pin configured as a touch sensor is read, rather than a simple 0 or 1.  The triggerType
  // function must therefore compare the value read from the touch sensor pin to some pre-computed "threshold" to determine whether or not
  // the touch pad has in fact been touched.  This is the threshold value that HomeSpan auto-calibrates for you as described above.  
  // 
  // Making things even more complex is that the ESP32 touch pins work in the reverse direction as touch pins on the ESP32-S2 and ESP32-S3.
  // On the former, the values read from a touch sensor DECREASE when the touch pad is touched.  On the latter, the values increase when the
  // touch pad is touched.  This means that for ESP32 devices, HomeSpan uses the following definition for SpanButton::TRIGGER_ON_TOUCH:
  //
  //    boolean TRIGGER_ON_TOUCH(int pinArg) { return ( touchRead(pinArg) < threshold ); }
  //
  // whereas on ESP32-S2 and ESP32-S3 devices, HomeSpan uses a definition that flips the direction of the comparison:
  //
  //    boolean TRIGGER_ON_TOUCH(int pinArg) { return ( touchRead(pinArg) > threshold ); }
  //
  // For ESP32-C3 devices, HomeSpan does not define TRIGGER_ON_TOUCH at all since there are no touch pins on an ESP32-C3 device!  The compiler
  // will throw an error if you try to create a SpanButton with triggerType=SpanButton::TRIGGER_ON_TOUCH, or if you call either of the
  // calibration functions above.
  //

  // CREATING YOUR OWN triggerType FUNCTION
  // --------------------------------------

  // You are not limited to choosing among HomeSpan's three built-in triggerType functions. You can instead create your own triggerType function
  // and pass it to SpanButton as the triggerType parameter in the SpanButton constructor.  Your function must be of the form `boolean func(int)`,
  // and should return TRUE if the "pushbutton" associated with the pin number that HomeSpan passes to your function as the integer argument
  // has been "pressed", or FALSE if it has not.  This allows you to expand the used of SpanButton to work with pin multiplexers, pin extenders,
  // or any device that may require custom handling via a third-party library.
  //
  // For example, if you were using an MCP I/O Port Expander with the Adafruit mcp library, you could create a triggerType function for a pin
  // on the MCP device that is connected to ground through a pushbutton as such:
  //
  //    boolean MCP_READ(int mcpPin) { return ( !mcp.digitalRead(mcpPin) ); }
  //
  // And then simply pass MCP_READ to SpanButton as the triggerType parameter using any of the SpanButton constuctors:
  //
  //    new SpanButton(23,MCP_READ);                      // uses default longTime, singleTime, and doubleTime
  //    new SpanButton(23,MCP_READ,2000,5,200);           // expliclty sets longTime, singleTime, and doubletime
  //    new SpanButton(23,2000,5,200,MCP_READ);           // alternative constructor with arguments in a different order
  //
  // Alternatively, you can use a lambda function as the triggerType parameter, thus creating your function on the fly when instantiating a SpanButton:
  //
  //    new SpanButton(23,[](int mcpPin)->boolean{ return ( !mcp.digitalRead(mcpPin) ); });
  //
  // Note: If you create your own triggerType function, don't forget to perform any initialization of the "pin", or setup/configuration of a
  // pin extender, etc., prior to instantiating a SpanButton that uses your custom function.  HomeSpan cannot do this for you.
  //
  
  
