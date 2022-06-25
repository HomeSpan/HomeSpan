
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

#include "extras/PwmPin.h"                          // library of various PWM functions

////////////////////////////////////

struct DEV_GenericLED : Service::LightBulb {       // Generic LED

  // This version of the LED Service allows for dimmable and non-dimmable devices. Status of both the
  // power state and the brightness of the LED are stored in NVS for restoration if the device reboots.
  
  LedPin *LED;                                      // reference to an LedPin
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // reference to the Brightness Characteristic
  boolean isDimmable;                               // flag to indicate whether light is dimmable
 
  DEV_GenericLED(int ledPin, uint8_t dimmable=0) : Service::LightBulb(){

    power=new Characteristic::On(0,true);           // second argument is true, so the value of the On Characteristic (initially set to 0) will be saved in NVS
    isDimmable=dimmable;

    if(isDimmable){
      level=new Characteristic::Brightness(50,true); // second argument is true, so the value of the Brightness Characteristic (initially set to 50) will be saved in NVS               
      level->setRange(5,100,1);                       // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%
    }

    this->LED=new LedPin(ledPin);                   // configures a PWM LED for output to pin number "ledPin"

    Serial.printf("Configuring LED: Pin=%d %s\n",LED->getPin(),isDimmable?"(Dimmable)":""); // initialization message

    LED->set(power->getVal()*(isDimmable?(level->getVal()):100));             // set the LED to its initial state at startup.
    
  } // end constructor

  boolean update(){                              // update() method

    LOG1("Updating LED on pin=");
    LOG1(LED->getPin());
    LOG1(":  Current Power=");
    LOG1(power->getVal()?"true":"false");
    if(isDimmable){
      LOG1("  Current Brightness=");
      LOG1(level->getVal());
    }
  
    if(power->updated()){
      LOG1("  New Power=");
      LOG1(power->getNewVal()?"true":"false");
    }

    if(isDimmable && level->updated()){
      LOG1("  New Brightness=");
      LOG1(level->getNewVal());
    } 

    LOG1("\n");
    
    LED->set(power->getNewVal()*(isDimmable?(level->getNewVal()):100));       // update the physical LED to reflect the new values    
   
    return(true);                               // return true
  
  } // update

};
      
//////////////////////////////////
