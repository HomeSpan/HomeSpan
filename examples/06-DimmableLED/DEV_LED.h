
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

#include "extras/PwmPin.h"                          // NEW! Include this HomeSpan "extra" to create LED-compatible PWM signals on one or more pins

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

  PwmPin *pwmPin;                                   // NEW! Create reference to PWM Pin instantiated below
  int channel;                                      // NEW! Store the PWM channel used for this LED (should be unique for each LED)
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // NEW! Create a reference to the Brightness Characteristic instantiated below
  
  DEV_DimmableLED(int channel, int ledPin) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();     
                
    level=new Characteristic::Brightness(50);       // NEW! Instantiate the Brightness Characteristic with an initial value of 50% (same as we did in Example 4)
    new SpanRange(5,100,1);                         // NEW! This sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1% (different from Example 4 values)

    this->channel=channel;                          // NEW! Save the channel number (from 0-15)
    this->pwmPin=new PwmPin(channel, ledPin);       // NEW! Configures the PWM channel and attach the specified ledPin. pinMode() does NOT need to be called.
    
  } // end constructor

  boolean update(){                              // update() method

    // Here we set the duty cycle (brightness) of the LED by callng pwmPin with the appropriate channel.
    // The second argument should be a number from 0-100 (representing %brightness).  HomeKit sets the on/off 
    // status of the LED ("power") separately from the brightness of the LED ("level").  This means HomeKit can
    // request the LED be turned off, but retain the brightness level so that it does not need to be resent if
    // the LED is turned back on.  Multiplying the newValue of the power Characteristic (as a boolean) with the
    // newValue of the Brightness Characteristic (as an int) is a short-hand way of creating the logic to
    // set the PWM level when the LED is off (always zero) or on (whatever the brightness level is).
    
    pwmPin->set(channel,power->getNewVal()*level->getNewVal());    
   
    return(true);                               // return true
  
  } // update
};
      
//////////////////////////////////
