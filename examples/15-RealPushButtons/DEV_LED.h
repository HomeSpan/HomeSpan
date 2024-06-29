
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
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
  int raisePin;                                     // NEW! pin with pushbutton to increase brightness
  int lowerPin;                                     // NEW! pin with pushButton to decrease brightness
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // reference to the Brightness Characteristic
  int favoriteLevel=50;                             // NEW! keep track of a 'favorite' level

  // NEW!  Consructor includes 3 additional arguments to specify pin numbers for power, raise, and lower buttons
  
  DEV_DimmableLED(int pin, int powerPin, int raisePin, int lowerPin) : Service::LightBulb(){

    power=new Characteristic::On();     
                
    level=new Characteristic::Brightness(favoriteLevel);       // Brightness Characteristic with an initial value equal to the favorite level
    level->setRange(5,100,1);                                  // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%

    // NEW!  Below we create three SpanButton() objects.  In the first we specify the pin number, as required, but allow SpanButton() to use
    // its default values for a LONG press (2000 ms), a SINGLE press (5 ms), and a DOUBLE press (200 ms).  In the second and third we change the
    // default LONG press time to 500 ms, which works well for repeatedly increasing or decreasing the brightness.  Since we do not specify
    // a triggerType, SpanButton uses the default TRIGGER_ON_TOUCH, which is suitable for a pushbutton that connects pin to GROUND when pressed.
    
    // All of the logic for increasing/decreasing brightness, turning on/off power, and setting/resetting a favorite brightness level is found
    // in the button() method below.

    new SpanButton(powerPin);                       // NEW! create new SpanButton to control power using pushbutton on pin number "powerPin"
    new SpanButton(raisePin,500);                   // NEW! create new SpanButton to increase brightness using pushbutton on pin number "raisePin"
    new SpanButton(lowerPin,500);                   // NEW! create new SpanButton to decrease brightness using pushbutton on pin number "lowerPin"

    this->powerPin=powerPin;                        // NEW! save power pushbutton pin number
    this->raisePin=raisePin;                        // NEW! save increase brightness pushbutton pin number
    this->lowerPin=lowerPin;                        // NEW! save decrease brightness pushbutton pin number
    this->ledPin=new LedPin(pin);                   // configures a PWM LED for output to the specified pin

    Serial.print("Configuring Dimmable LED: Pin="); // initialization message
    Serial.print(ledPin->getPin());
    Serial.print("\n");
    
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

  // NEW!  Here is the button() method where all the PushButton actions are defined.   Take note of the signature, and use of the word "override" 

  void button(int pin, int pressType) override {

    LOG1("Found button press on pin: ");            // always a good idea to log messages
    LOG1(pin);
    LOG1("  type: ");
    LOG1(pressType==SpanButton::LONG?"LONG":(pressType==SpanButton::SINGLE)?"SINGLE":"DOUBLE");
    LOG1("\n");

    int newLevel;

    if(pin==powerPin){
      if(pressType==SpanButton::SINGLE){            // if a SINGLE press of the power button...
        power->setVal(1-power->getVal());           // ...toggle the value of the power Characteristic
      } else
      
      if(pressType==SpanButton::DOUBLE){            // if a DOUBLE press of the power button...
        power->setVal(1);                           // ...turn on power
        level->setVal(favoriteLevel);               // ...and set brightness to the favorite level        
      } else
      
      if(pressType==SpanButton::LONG) {             // if a LONG press of the power button...
        favoriteLevel=level->getVal();              // ...save the current brightness level
        LOG1("Saved new brightness level=");        // ...and output log message
        LOG1(favoriteLevel);
        LOG1("\n");        
        ledPin->set((1-power->getVal())*level->getVal());       // blink the LED to indicate new level has been saved
        delay(100);
        ledPin->set((1-power->getVal())*level->getVal());
      }
      
    } else

    if(pin==raisePin){                                                   
      if(pressType==SpanButton::DOUBLE){            // if a DOUBLE press of the raise button...
        power->setVal(1);                           // ...turn on power
        level->setVal(100);                         // ...and set brightness to the max level
      } else {
      
      newLevel=level->getVal()+(pressType==SpanButton::LONG?10:1);   // get current brightness level and increase by either 10% (LONG press) or 1% (SINGLE press)
      if(newLevel>100)                                               // don't allow new level to exceed maximium of 100%
        newLevel=100;
      level->setVal(newLevel);                                       // set the value of the brightness Characteristic to this new level
      }
      
    } else

    if(pin==lowerPin){                                                   
      if(pressType==SpanButton::DOUBLE){            // if a DOUBLE press of the lower button...
        power->setVal(1);                           // ...turn on power
        level->setVal(5);                           // ...and set brightness to the min level
      } else {
      
      newLevel=level->getVal()-(pressType==SpanButton::LONG?10:1);   // get current brightness level and decrease by either 10% (LONG press) or 1% (SINGLE press)
      if(newLevel<5)                                                 // don't allow new level to fall below minimum of 5%  
        newLevel=5;
      level->setVal(newLevel);                                       // set the value of the brightness Characteristic to this new level
      }

    }

    // Don't forget to set the new power and level for the actual LED - the above code by itself only changes the values of the Characteristics
    // within HomeKit!  We still need to take an action on the actual LED itself.
    
    // Note the line below is similar to, but not the same as, the ledPin->set function used in the update() method above.  Within the
    // update() method we used getNewVal() because we wanted to change the LED to match the NEW VALUES requested by the user via the
    // HomeKit Controller.  We did not need to (and must not) use setVal() to modify these values in the update() method since HomeSpan
    // automatically does this for us, provided we return StatusCode::OK at the end of the update() method.
    
    // But in the button() method, getNewVal() means nothing, since the button() method is not called by HomeKit in response to a user request
    // from a HomeKit Controller interface.  Instead, we are manually changing the values of one or more Characteristic using setVal() in response
    // to SINGLE, DOUBLE, and LONG SpanButton requests.  These changes are instantaneous, so we can retreive the new values with a subsequent call to getVal(),
    // as shown below.  As usual, HomeSpan will send Event Notifications to all registered HomeKit Controllers letting them know about any changes
    // we made using setVal(). 
    
    ledPin->set(power->getVal()*level->getVal());       // update the physical LED to reflect the new values

  }

};
      
//////////////////////////////////
