
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

#include "extras/PwmPin.h"                          // library of various PWM functions

////////////////////////////////////

struct DEV_DimmableLED : Service::LightBulb {       // Dimmable LED

  PwmPin *pwmPin;                                   // reference to PWM Pin
  int ledPin;                                       // pin number defined for this LED
  int powerPin;                                     // NEW! pin with pushbutton to turn on/off LED
  int raisePin;                                     // NEW! pin with pushbutton to increase brightness
  int lowerPin;                                     // NEW! pin with pushButton to decrease brightness
  int channel;                                      // PWM channel used for this LED (should be unique for each LED)
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // reference to the Brightness Characteristic
  
  DEV_DimmableLED(int channel, int ledPin, int powerPin, int raisePin, int lowerPin, ServiceType sType=ServiceType::Regular) : Service::LightBulb(sType){

    power=new Characteristic::On();     
                
    level=new Characteristic::Brightness(50);       // Brightness Characteristic with an initial value of 50%
    new SpanRange(5,100,1);                         // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%

    new SpanButton(powerPin);                       // NEW! create new SpanButton to control power on pin number "powerPin"
    new SpanButton(raisePin,1000);                  // NEW! create new SpanButton to increase brightness on pin number "raisePin"
    new SpanButton(lowerPin,3000,500);              // NEW! create new SpanButton to decrease brightness on pin number "lowerPin"

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

  void button(int pin, boolean isLong){

    LOG1("Found button press on pin: ");
    LOG1(pin);
    LOG1("  type: ");
    LOG1(isLong?"LONG":"SHORT");
    LOG1("\n");

    if(pin==powerPin && !isLong){
      power->setVal(1-power->getVal());
    } else
    
    if(pin==powerPin && isLong){
      if(power->getVal())
        level->setVal(100);
      else
        level->setVal(5);
    } else

    if(pin==raisePin){
      int newLevel=level->getVal()+(isLong?10:1);
      if(newLevel>100)
        newLevel=100;
      level->setVal(newLevel);
    } else

    if(pin==lowerPin){
      int newLevel=level->getVal()-(isLong?10:1);
      if(newLevel<5)
        newLevel=5;
      level->setVal(newLevel);
    }
    
    pwmPin->set(channel,power->getVal()*level->getVal());    

  }

};
      
//////////////////////////////////
