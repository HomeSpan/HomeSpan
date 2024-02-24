
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

struct DEV_LED : Service::LightBulb {               // ON/OFF LED

  int ledPin;                                       // pin number defined for this LED
  SpanCharacteristic *power;                        // reference to the On Characteristic
  
  DEV_LED(int ledPin) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();                 
    this->ledPin=ledPin;                            
    pinMode(ledPin,OUTPUT);                         
    
  } // end constructor

  boolean update(){                              // update() method

    digitalWrite(ledPin,power->getNewVal());      
   
    return(true);                               // return true
  
  } // update
};
      
//////////////////////////////////

// Here's the new code defining DEV_DimmableLED - changes from above are noted in the comments

struct DEV_DimmableLED : Service::LightBulb {       // Dimmable LED

  LedPin *ledPin;                                   // NEW! Create reference to LED Pin instantiated below
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // NEW! Create a reference to the Brightness Characteristic instantiated below
  
  DEV_DimmableLED(int pin) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();     
                
    level=new Characteristic::Brightness(50);       // NEW! Instantiate the Brightness Characteristic with an initial value of 50% (same as we did in Example 4)
    level->setRange(5,100,1);                       // NEW! This sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1% (different from Example 4 values)

    this->ledPin=new LedPin(pin);                   // NEW! Configures a PWM LED for output to the specified pin.  Note pinMode() does NOT need to be called in advance
    
  } // end constructor

  boolean update(){                              // update() method

    // Here we set the brightness of the LED by calling ledPin->set(brightness), where brightness=0-100.
    // Note HomeKit sets the on/off status of a LightBulb separately from its brightness, which means HomeKit
    // can request a LightBulb be turned off, but still retains the brightness level so that it does not need
    // to be resent once the LightBulb is turned back on.
    
    // Multiplying the newValue of the On Characteristic ("power", which is a boolean) with the newValue of the
    // Brightness Characteristic ("level", which is an integer) is a short-hand way of creating the logic to
    // set the LED level to zero when the LightBulb is off, or to the current brightness level when it is on.
    
    ledPin->set(power->getNewVal()*level->getNewVal());    
   
    return(true);                               // return true
  
  } // update
};
      
//////////////////////////////////
