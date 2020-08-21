
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

#include "extras/PwmPin.h"                          // library of various PWM functions

////////////////////////////////////

struct DEV_DimmableLED : Service::LightBulb {       // Dimmable LED

  // This version of the Dimmable LED Service is similar to the one last used in Example 11, but now includes support for 3 physical PushButtons
  // performing the following actions:
  //
  // power button:  SHORT press toggles power on/off; LONG press sets brightness to 100% if power is on, or sets brightness to 5% if power is off
  // raise button:  SHORT press increases brightness by 1%.  LONG press increases brightness by 10%
  // lower button:  SHORT press decreases brightness by 1%.  LONG press decreases brightness by 10%
  
  PwmPin *pwmPin;                                   // reference to PWM Pin
  int ledPin;                                       // pin number defined for this LED
  int powerPin;                                     // NEW! pin with pushbutton to turn on/off LED
  int raisePin;                                     // NEW! pin with pushbutton to increase brightness
  int lowerPin;                                     // NEW! pin with pushButton to decrease brightness
  int channel;                                      // PWM channel used for this LED (should be unique for each LED)
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // reference to the Brightness Characteristic

  // NEW!  Consructor includes 3 additionl arguments to specify pin numbers for power, raise, and lower buttons
  
  DEV_DimmableLED(int channel, int ledPin, int powerPin, int raisePin, int lowerPin, ServiceType sType=ServiceType::Regular) : Service::LightBulb(sType){

    power=new Characteristic::On();     
                
    level=new Characteristic::Brightness(50);       // Brightness Characteristic with an initial value of 50%
    new SpanRange(5,100,1);                         // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%

    // NEW!  Below we create three SpanButton() objects.  In the first we specify the pin number, as required, but allow SpanButton() to use
    // its default values for a LONG press (2000 ms) and a SHORT press (5 ms).  In the second we change the LONG press time to 1000 ms, which 
    // means we only have to hold the raise button for 1 second to trigger a LONG press that increases the brightness by 10%.  In the the third,
    // we change both the LONG press time to 3000 ms (which means holding the button for 3 full seconds before releasing to decrease the brightness
    // by 10%), and the SHORT press time to 500 ms, which means holding down the button for at least half a second (but not longer than 3 seconds)
    // to derease the brightness by 1%.  The logic for increasing/decreasing brightness, as well as turning on/off power, is found in the button()
    // method below.  Note that in practice you likely would not use different combinations of parameters for buttons that perform similar types of
    // functions.  We've only done so here to illustrate how the parameters work.

    new SpanButton(powerPin);                       // NEW! create new SpanButton to control power using PushButton on pin number "powerPin"
    new SpanButton(raisePin,1000);                  // NEW! create new SpanButton to increase brightness using PushButton on pin number "raisePin"
    new SpanButton(lowerPin,3000,500);              // NEW! create new SpanButton to decrease brightness using PushButton on pin number "lowerPin"

    this->channel=channel;                          // save the channel number (from 0-15)
    this->ledPin=ledPin;                            // save LED pin number
    this->powerPin=powerPin;                        // NEW! save power pushbutton pin number
    this->raisePin=raisePin;                        // NEW! save increase brightness pushbutton pin number
    this->lowerPin=lowerPin;                        // NEW! save decrease brightness pushbutton pin number
    this->pwmPin=new PwmPin(channel, ledPin);       // configure the PWM channel and attach the specified ledPin

    Serial.print("Configuring Dimmable LED: Pin="); // initialization message
    Serial.print(ledPin);
    Serial.print(" Channel=");
    Serial.print(channel);
    Serial.print("\n");
    
  } // end constructor

  StatusCode update(){                              // update() method

    LOG1("Updating Dimmable LED on pin=");
    LOG1(ledPin);
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
    
    pwmPin->set(channel,power->getNewVal()*level->getNewVal());    
   
    return(StatusCode::OK);                         // return OK status code
  
  } // update

  // NEW!  Here is the button() method where all the PushButton actions are defined.   Take note of the signature, and use of the word "override" 

  void button(int pin, boolean isLong) override {

    LOG1("Found button press on pin: ");            // always a good idea to log messages
    LOG1(pin);
    LOG1("  type: ");
    LOG1(isLong?"LONG":"SHORT");
    LOG1("\n");

    if(pin==powerPin && !isLong){                   // if a SHORT press of the power PushButton...
      power->setVal(1-power->getVal());             // ...toggle the value of the power Characteristic
    } else
    
    if(pin==powerPin && isLong){                    // if a LONG press of the power PushButton...
      if(power->getVal())                           // ...and power Characteristic is true (LED is on) 
        level->setVal(100);                         // set brightness level Characteristic to 100%
      else                                          // ...else, power Characteristic is false (LED os off)  
        level->setVal(5);                           // so set brightness level Characteristic to 5%
    } else

    if(pin==raisePin){                              // if raise PushButton has been pressed
      int newLevel=level->getVal()+(isLong?10:1);   // get current brightness level and increase by either 10% (LONG press) or 1% (SHORT press)
      if(newLevel>100)                              // don't allow new level to exceed maximium of 100%
        newLevel=100;
      level->setVal(newLevel);                      // set the value of the brightness Characteristic to this new level
    } else

    if(pin==lowerPin){                              // if lower PushButton has been pressed
      int newLevel=level->getVal()-(isLong?10:1);   // get current brightness level and decrease by either 10% (LONG press) or 1% (SHORT press)
      if(newLevel<5)                                // don't allow new level to fall below minimum of 5%  
        newLevel=5;
      level->setVal(newLevel);                      // set the value of the brightness Characteristic to this new level
    }

    // Don't forget to set the new power and level for the actual LED - the above code by itself only changes the values of the Characteristics
    // within HomeKit!  We still need to take an action on the actual LED itself.
    
    // Note the line below is similar to, but not the same as, the pwmPin->set function used in the update() method above.  Within the
    // update() method we used getNewVal() because we wanted to change the LED to match the NEW VALUES requested by the user via the
    // HomeKit Controller.  We did not need to (and must not) use setVal() to modify these values in the update() method since HomeSpan
    // automatically does this for us, provided we return StatusCode::OK at the end of the update() method.
    
    // But in the button() method, getNewVal() means nothing, since the button() method is not called by HomeKit in response to a user request
    // from a HomeKit Controller interface.  Instead, we are manually changing the values of one or more Characteristic using setVal() in response
    // to LONG and SHORT PushButton requests.  These changes are instantaneous, so we can retreive the new values with a subsequent call to getVal(),
    // as shown below.  As usual, HomeSpan will send Event Notifications to all registered HomeKit Controllers letting them know about any changes
    // we made using setVal(). 
    
    pwmPin->set(channel,power->getVal()*level->getVal());       // update the physical LED to reflect the new values

  }

};
      
//////////////////////////////////
