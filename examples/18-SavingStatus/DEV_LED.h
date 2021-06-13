
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

#include "extras/PwmPin.h"                          // library of various PWM functions

////////////////////////////////////

struct DEV_DimmableLED : Service::LightBulb {       // Dimmable LED

  // This version of the Dimmable LED Service is similar to the one last used in Example 11, but now includes support for 3 physical PushButtons
  // performing the following actions:
  //
  // power button:  SHORT press toggles power on/off; LONG press saves current brightness as favorite level; DOUBLE press sets brightness to favorite level
  // raise button:  SHORT press increases brightness by 1%; LONG press increases brightness by 10%; DOUBLE press increases brightness to maximum
  // lower button:  SHORT press decreases brightness by 1%; LONG press decreases brightness by 10%; DOUBLE press decreases brightness to minimum
  
  LedPin *ledPin;                                   // reference to Led Pin
  int powerPin;                                     // NEW! pin with pushbutton to turn on/off LED
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // reference to the Brightness Characteristic

  // NEW!  Consructor includes 3 additional arguments to specify pin numbers for power, raise, and lower buttons
  
  DEV_DimmableLED(int pin, int powerPin) : Service::LightBulb(){

    power=new Characteristic::On(0,true);     
                
    level=new Characteristic::Brightness(5,true);        // Brightness Characteristic with an initial value equal to the favorite level
    level->setRange(5,100,1);                       // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%

    // NEW!  Below we create three SpanButton() objects.  In the first we specify the pin number, as required, but allow SpanButton() to use
    // its default values for a LONG press (2000 ms), a SINGLE press (5 ms), and a DOUBLE press (200 ms).  In the second and third we change the
    // default LONG press time to 500 ms, which works well for repeatedly increasing or decreasing the brightness.
    
    // All of the logic for increasing/decreasing brightness, turning on/off power, and setting/resetting a favorite brightness level is found
    // in the button() method below.

    new SpanButton(powerPin);                       // NEW! create new SpanButton to control power using pushbutton on pin number "powerPin"

    this->powerPin=powerPin;                        // NEW! save power pushbutton pin number
    this->ledPin=new LedPin(pin);                   // configures a PWM LED for output to the specified pin

    Serial.print("Configuring Dimmable LED: Pin="); // initialization message
    Serial.print(ledPin->getPin());
    Serial.print("\n");

    ledPin->set(power->getVal()*level->getVal());    
    
  } // end constructor

  boolean update(){                              // update() method

    LOG1("Updating Dimmable LED on pin=");
    LOG1(ledPin->getPin());
    LOG1(":  Current Power=");
    LOG1(power->getVal()?"true":"false");
    LOG1("  Current Brightness=");
    LOG1(level->getVal());
  
    if(power->updated()){
      LOG1("  New Power=");
      LOG1(power->getNewVal()?"true":"false");
    }

    if(level->updated()){
      LOG1("  New Brightness=");
      LOG1(level->getNewVal());
    } 

    LOG1("\n");
    
    ledPin->set(power->getNewVal()*level->getNewVal());    
   
    return(true);                               // return true
  
  } // update

  void button(int pin, int pressType) override {

    LOG1("Found button press on pin: ");            // always a good idea to log messages
    LOG1(pin);
    LOG1("  type: ");
    LOG1(pressType==SpanButton::LONG?"LONG":(pressType==SpanButton::SINGLE)?"SINGLE":"DOUBLE");
    LOG1("\n");

    if(pin==powerPin && pressType==SpanButton::SINGLE){
      power->setVal(1-power->getVal());                     // toggle the value of the power Characteristic
      ledPin->set(power->getVal()*level->getVal());         // update the physical LED to reflect the new values
    }   

  } // button

};
      
//////////////////////////////////
