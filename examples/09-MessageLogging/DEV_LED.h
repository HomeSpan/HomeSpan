
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
    
    // Here we output log messages when the constructor is initially called.
    // We use Serial.print() since to ensure the message is always output
    // regardless of the VERBOSITY setting.

    Serial.print("Configuring On/Off LED: Pin=");   // initialization message
    Serial.print(ledPin);
    Serial.print("\n");

  } // end constructor

  boolean update(){                              // update() method

    // Here we output log messages whenever update() is called,
    // which is helpful for debugging purposes if your physical device
    // is not functioning as expected.  Since it's just for debugging,
    // we use LOG1() instead of Serial.print().  Note we can output
    // both the current as well as the new power settings.

    LOG1("Updating On/Off LED on pin=");
    LOG1(ledPin);
    LOG1(":  Current Power=");
    LOG1(power->getVal()?"true":"false");
    LOG1("  New Power=");
    LOG1(power->getNewVal()?"true":"false");
    LOG1("\n");

    digitalWrite(ledPin,power->getNewVal());      
   
    return(true);                               // return true
  
  } // update
};
      
//////////////////////////////////

struct DEV_DimmableLED : Service::LightBulb {       // Dimmable LED

  LedPin *ledPin;                                   // reference to Led Pin
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // reference to the Brightness Characteristic
  
  DEV_DimmableLED(int pin) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();     
                
    level=new Characteristic::Brightness(50);       // Brightness Characteristic with an initial value of 50%
    level->setRange(5,100,1);                       // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%

    this->ledPin=new LedPin(pin);                   // configures a PWM LED for output to the specified pin

    // Here we output log messages when the constructor is initially called.
    // We use Serial.print() since to ensure the message is always output
    // regardless of the VERBOSITY setting.  Note that ledPin has a method getPin()
    // that retrieves the pin number so you don't need to store it separately.

    Serial.print("Configuring Dimmable LED: Pin="); // initialization message
    Serial.print(ledPin->getPin());
    Serial.print("\n");
    
  } // end constructor

  boolean update(){                              // update() method

    // Here we output log messages whenever update() is called,
    // which is helpful for debugging purposes if your physical device
    // is not functioning as expected.  Since it's just for debugging,
    // we use LOG1() instead of Serial.print().

    LOG1("Updating Dimmable LED on pin=");
    LOG1(ledPin->getPin());
    LOG1(":  Current Power=");
    LOG1(power->getVal()?"true":"false");
    LOG1("  Current Brightness=");
    LOG1(level->getVal());

    // Note that since Dimmable_LED has two updateable Characteristics,
    // HomeKit may be requesting either or both to be updated.  We can
    // use the "isUpdated" flag of each Characteristic to output a message
    // only if HomeKit actually requested an update for that Characteristic.
    // Since update() is called whenever there is an update to at least
    // one of the Characteristics in a Service, either power, level, or both
    // will have its "isUpdated" flag set.
  
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
};
      
//////////////////////////////////
