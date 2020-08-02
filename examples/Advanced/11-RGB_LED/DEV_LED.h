
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

#include "extras/PwmPin.h"

//////////////////////////////////

struct DEV_LED : Service::LightBulb {               // ON/OFF LED

  int ledPin;                                       // pin number defined for this LED
  SpanCharacteristic *power;                        // reference to the On Characteristic
  
  DEV_LED(int ledPin) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();                 
    this->ledPin=ledPin;                            
    pinMode(ledPin,OUTPUT);                         
    
  } // end constructor

  StatusCode update(){                              // update() method

    digitalWrite(ledPin,power->newValue.BOOL);      
   
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
                
    level=new Characteristic::Brightness(50);       // instantiate the Brightness Characteristic with an initial value of 50%
    new SpanRange(5,100,1);                         // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%

    this->channel=channel;                          // save the channel number (from 0-15)
    this->pwmPin=new PwmPin(channel, ledPin);       // configures the PWM channel and attach the specified ledPin. pinMode() does NOT need to be called.
    
  } // end constructor

  StatusCode update(){                              // update() method
    
    pwmPin->set(channel,power->newValue.BOOL*level->newValue.INT);    
   
    return(StatusCode::OK);                         // return OK status code
  
  } // update
};
      
//////////////////////////////////

struct DEV_RgbLED : Service::LightBulb {            // RGB LED (Command Cathode)

  PwmPin *redPin;                                   
  PwmPin *greenPin;                                 
  PwmPin *bluePin;                                  
  int redChannel;                                   
  int greenChannel;                                   
  int blueChannel;                                   
  SpanCharacteristic *power;                   // reference to the On Characteristic
  SpanCharacteristic *H;                       // reference to the Hue Characteristic
  SpanCharacteristic *S;                       // reference to the Saturation Characteristic
  SpanCharacteristic *B;                       // reference to the Brightness Characteristic
  
  DEV_RgbLED(int redChannel, int greenChannel, int blueChannel, int redPin, int greenPin, int bluePin) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();                    
    H=new Characteristic::Brightness(100);       // instantiate the Brightness Characteristic with an initial value of 100%
    new SpanRange(5,100,1);                         // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%

    this->channel=channel;                          // save the channel number (from 0-15)
    this->pwmPin=new PwmPin(channel, ledPin);       // configures the PWM channel and attach the specified ledPin. pinMode() does NOT need to be called.
    
  } // end constructor

  StatusCode update(){                              // update() method
    
    pwmPin->set(channel,power->newValue.BOOL*level->newValue.INT);    
   
    return(StatusCode::OK);                         // return OK status code
  
  } // update
};
      
//////////////////////////////////
