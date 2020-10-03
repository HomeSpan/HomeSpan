
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
#include "DEV_Identify.h"       

void setup() {

  // In Example 14 we saw how to emulate a PushButton tile within HomeKit by automatically resetting a Characteristic so that
  // it "turns off" after a short period of time.  However, sometimes we want to be able to physically control a device with actual
  // PushButtons (or momentary switches) that trigger an action, such as turning on a light or fan, or opening a garage door.
  // Additionally, we want HomeKit to reflect any changes in the device as a result of such manual actions - HomeKit should know
  // when the light has been turned on or off manually.

  // One way to accomplish would be via custom code added to the loop() method of your derived Service that monitors the button,
  // checks when it is pressed, debounces button noise, performs some actions when pressed, and informs HomeKit of the actions with
  // the setVal() method. Or you can use HomeSpan's built-in SpanButton() object.

  // SpanButton() is a Service-level object, meaning it attaches itself to the last Service you define.  Typically you would instantiate
  // one of more SpanButton() objects directly inside the constructor for your derived Service.
  
  // SpanButton() supports two types of a triggers: a SHORT (momentary) button press, and a LONG (extended) button press.  SpanButton()
  // takes 3 arguments, in the following order:
  //
  //  * the pin number to which the PushButton is attached (required)
  //  * the length of time (in milliseconds) the button needs to be pushed to be considered a LONG press (optional; default=2000 ms)
  //  * the length of time (in milliseconds) the button needs to be pushed to be considered a SHORT press (optional; default=5 ms)

  // When SpanButton() is instantiated, it sets the specified pin on the ESP32 to be an INPUT with PULL-UP, meaning that the pin will
  // normally return a value of HIGH when read.  Your actual PushButton should be connected so that this pin is GROUNDED when the button
  // is pressed.

  // HomeSpan automatically polls all pins with associated SpanButton() objects and checks for LOW values, indicating the button was
  // pressed, but not yet released.  It then starts a timer and waits for the button to be released.
  
  // NOTE! TRIGGERS DO NOT OCCUR UNTIL THE BUTTON IS RELEASED - IF YOU HOLD DOWN A BUTTON INDEFINITELY, NOTHING HAPPENS.
  
  // The length of the press needed to trigger either a SHORT or LONG action is specified by the optional arguments.  Since most buttons
  // create spurious noise when pressed (and then again when released), the default time to trigger a SHORT press is 5ms.  It's fine to change
  // this to a longer value, but a shorter value is not recommended as this may allow spurious triggers unless you debounce your switch
  // with hardware.

  // To use SpanButton() within a derived Service you need to implement a button() method.  Similar to the loop() method, your button()
  // method will typically contain some combination of getVal() functions and setVal() functions, along with code that performs some set
  // of actions on the physical device (seting pins high or low, turning on fans, etc).  However, in contrast to the loop() method, which
  // is called by HomeSpan every polling cycle, HomeSpan only calls the button() method when a button attached to the Service registers a
  // SHORT or LONG press.

  // Also in contrast with the loop method, the button() method takes two arguments, an int and a boolean, and should defined as follows:
  //
  // void button(int pin, boolean isLong)
  //
  // where "pin" is the pin number of the PushButton that was triggered, and "isLong" is a flag indicating whether SpanButton() detected a
  // LONG press (isLong=true) or a SHORT press (isLong=false).  Of course you can replace the variables "pin" and "isLong" with your own
  // names.  The only requirement is the defintiion conform to the "void button(int, boolean)" signature.  When HomeSpan first starts up it checks
  // all Services containing one or more SpanButton() instances to ensure you've implemented your own button(int, boolean) method.  If not,
  // HomeSpan will print a warning message on the Serial Monitor.  Nothing bad happens if you instantiate a SpanButton() but forget to create
  // the button() method, or you create it with the wrong parameters.  Buy nothing good happens either - button presses are just ignored.
  //
  // C++ Note:  For an extra check, you can also place the the contextual keyword "override" after your method definition as such:
  //
  // void button(int buttonPin, boolean longPress) override {...your code...}
  //
  // Doing so allows the compiler to check that you are indeed over-riding the base class button() method and not inadvertently creating a new
  // button() method with an incorrect signature that will never be called by SpanButton().  In fact, you could add "override" to the definition
  // of your update() and loop() methods as well, since these are always supposed to over-ride the base-class method.

  // To demonstrate how PushButtons works in practice, we will implement a Dimmable LED starting with the same LED code use in Example 11,
  // but with 3 SpanButton() objects performing different functions that showcase the different parameters.  As usual, all the code is implemented
  // in DEV_LED.h, with NEW! comments highlighting changes from Example 11.  You'll also notice that we've extended the constructor for this
  // version of our derived Dimmable LED Service to include the pin numbers for each of our buttons.  See DEV_LED.h for details.
  
  Serial.begin(115200);

  homeSpan.setLogLevel(1);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");

  new SpanAccessory();  
    new DEV_Identify("Bridge #1","HomeSpan","123-ABC","HS Bridge","0.9",3);
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");

  new SpanAccessory();                                                          
    new DEV_Identify("PushButton LED","HomeSpan","123-ABC","20mA LED","0.9",0);
    
    new DEV_DimmableLED(0,17,23,5,18);          // NEW! added three extra arguments to specify the pin numbers for three SpanButtons() - see DEV_LED.h
 
} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
