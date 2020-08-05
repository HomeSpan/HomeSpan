
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

#include "extras/PwmPin.h"                          // library of various PWM functions

////////////////////////////////////

struct DEV_LED : Service::LightBulb {               // ON/OFF LED

  int ledPin;                                       // pin number defined for this LED
  SpanCharacteristic *power;                        // reference to the On Characteristic
  
  DEV_LED(int ledPin, ServiceType sType=ServiceType::Regular) : Service::LightBulb(sType){        // constructor() method

    power=new Characteristic::On();                 
    this->ledPin=ledPin;                            
    pinMode(ledPin,OUTPUT);                         
    
    Serial.print("Configuring On/Off LED: Pin=");   // initialization message
    Serial.print(ledPin);
    Serial.print("\n");

  } // end constructor

  StatusCode update(){                              // update() method

    LOG1("Updating On/Off LED on pin=");
    LOG1(ledPin);
    LOG1(":  Current Power=");
    LOG1(power->getVal()?"true":"false");
    LOG1("  New Power=");
    LOG1(power->getNewVal()?"true":"false");
    LOG1("\n");

    digitalWrite(ledPin,power->getNewVal());      
   
    return(StatusCode::OK);                         // return OK status code
  
  } // update
};
      
//////////////////////////////////

struct DEV_DimmableLED : Service::LightBulb {       // Dimmable LED

  PwmPin *pwmPin;                                   // reference to PWM Pin
  int ledPin;                                       // pin number defined for this LED
  int channel;                                      // PWM channel used for this LED (should be unique for each LED)
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // reference to the Brightness Characteristic
  
  DEV_DimmableLED(int channel, int ledPin, ServiceType sType=ServiceType::Regular) : Service::LightBulb(sType){        // constructor() method

    power=new Characteristic::On();     
                
    level=new Characteristic::Brightness(50);       // Brightness Characteristic with an initial value of 50%
    new SpanRange(5,100,1);                         // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%

    this->channel=channel;                          // save the channel number (from 0-15)
    this->ledPin=ledPin;                            // save LED pin number
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
};
      
//////////////////////////////////
