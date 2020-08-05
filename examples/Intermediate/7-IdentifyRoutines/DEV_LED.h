
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

#include "extras/PwmPin.h"                          // allows PWM control of LED brightness

struct DEV_LED : Service::LightBulb {               // ON/OFF LED

  int ledPin;                                       // pin number defined for this LED
  SpanCharacteristic *power;                        // reference to the On Characteristic
  
  DEV_LED(int ledPin) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();                 
    this->ledPin=ledPin;                            
    pinMode(ledPin,OUTPUT);                         
    
  } // end constructor

  StatusCode update(){                              // update() method

    digitalWrite(ledPin,power->getNewVal());      
   
    return(StatusCode::OK);                         // return OK status code
  
  } // update
};
      
//////////////////////////////////

struct DEV_DimmableLED : Service::LightBulb {       // Dimmable LED

  PwmPin *pwmPin;                                   // reference to PWM Pin
  int channel;                                      // PWM channel used for this LED (should be unique for each LED)
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // reference to the Brightness Characteristic
  
  DEV_DimmableLED(int channel, int ledPin) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();     
                
    level=new Characteristic::Brightness(50);       // Brightness Characteristic with an initial value of 50%
    new SpanRange(5,100,1);                         // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%

    this->channel=channel;                          // save the channel number (from 0-15)
    this->pwmPin=new PwmPin(channel, ledPin);       // configure the PWM channel and attach the specified ledPin. pinMode() does NOT need to be called.
    
  } // end constructor

  StatusCode update(){                              // update() method
    
    pwmPin->set(channel,power->getNewVal()*level->getNewVal());    
   
    return(StatusCode::OK);                         // return OK status code
  
  } // update
};
      
//////////////////////////////////
