
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

  StatusCode update(){                              // update() method

    // Here we output log messages whenever update() is called,
    // which is helpful for debugging purposes if your physical device
    // is not functioning as expected.  Since it's just for debugging,
    // we use LOG1() instead of Serial.print().  Note we can output
    // both the current as well as the new power settings.

    LOG1("Updating On/Off LED on pin=");
    LOG1(ledPin);
    LOG1(":  Current Power=");
    LOG1(power->value.BOOL?"true":"false");
    LOG1("  New Power=");
    LOG1(power->newValue.BOOL?"true":"false");
    LOG1("\n");

    digitalWrite(ledPin,power->newValue.BOOL);      
   
    return(StatusCode::OK);                         // return OK status code
  
  } // update
};
      
//////////////////////////////////

struct DEV_DimmableLED : Service::LightBulb {       // Dimmable LED

  PwmPin *pwmPin;                                   // reference to PWM Pin
  int ledPin;                                       // pin number defined for this LED <- NEW!!
  int channel;                                      // PWM channel used for this LED (should be unique for each LED)
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // reference to the Brightness Characteristic
  
  DEV_DimmableLED(int channel, int ledPin) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();     
                
    level=new Characteristic::Brightness(50);       // Brightness Characteristic with an initial value of 50%
    new SpanRange(5,100,1);                         // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%

    this->channel=channel;                          // save the channel number (from 0-15)
    this->ledPin=ledPin;                            // LED pin number <- NEW!!
    this->pwmPin=new PwmPin(channel, ledPin);       // configure the PWM channel and attach the specified ledPin. pinMode() does NOT need to be called.

    // Here we output log messages when the constructor is initially called.
    // We use Serial.print() since to ensure the message is always output
    // regardless of the VERBOSITY setting.

    Serial.print("Configuring Dimmable LED: Pin="); // initialization message
    Serial.print(ledPin);
    Serial.print(" Channel=");
    Serial.print(channel);
    Serial.print("\n");
    
  } // end constructor

  StatusCode update(){                              // update() method

    // Here we output log messages whenever update() is called,
    // which is helpful for debugging purposes if your physical device
    // is not functioning as expected.  Since it's just for debugging,
    // we use LOG1() instead of Serial.print().

    // Note that in the prior example we did not save the ledPin number for
    // DimmableLED since it was only needed by the constructor for initializing
    // PwmPin(). For this example we add ledPin as a saved variable (see the two
    // lines marketed NEW!! above) for the sole purpose of this log message.

    LOG1("Updating Dimmable LED on pin=");
    LOG1(ledPin);
    LOG1(":  Current Power=");
    LOG1(power->value.BOOL?"true":"false");
    LOG1("  Current Brightness=");
    LOG1(level->value.INT);

    // Note that since Dimmable_LED has two updateable Characteristics,
    // HomeKit may be requesting either or both to be updated.  We can
    // use the "isUpdated" flag of each Characteristic to output a message
    // only if HomeKit actually requested an update for that Characteristic.
    // Since update() is called whenever there is an update to at least
    // one of the Characteristics in a Service, either power, level, or both
    // will have its "isUpdated" flag set.
  
    if(power->isUpdated){
      LOG1("  New Power=");
      LOG1(power->newValue.BOOL?"true":"false");
    }

    if(level->isUpdated){
      LOG1("  New Brightness=");
      LOG1(level->newValue.INT);
    } 

    LOG1("\n");
    
    pwmPin->set(channel,power->newValue.BOOL*level->newValue.INT);    
   
    return(StatusCode::OK);                         // return OK status code
  
  } // update
};
      
//////////////////////////////////
