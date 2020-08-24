
////////////////////////////////////
//    DEVICE-SPECIFIC SERVICE     //
////////////////////////////////////

#include "extras/RFControl.h"

// Zephyr Vent Hood

// Frequency:   433 MHz
// Encoding:    Fixed pulse duration of 850 usec
// 0-Bit:       230 HIGH / 620 LOW
// 1-Bit:       620 HIGH / 230 LOW
// N-Bits:      20
// N-Cycles:    8
// Cycle Gap:   4000 usec

void transmitZephyr(uint32_t code);

boolean resetLight=false;

//////////////////////////////////

struct DEV_ZephyrLight : Service::LightBulb {

  uint32_t code;
  SpanCharacteristic *power;
  SpanCharacteristic *level;
  int buttonPin;

  DEV_ZephyrLight(uint32_t code, int buttonPin, ServiceType mod=ServiceType::Regular) : Service::LightBulb(mod){

    power=new Characteristic::On(false);
    level=new Characteristic::Brightness(3);
    new SpanRange(0,3,1);
    new Characteristic::Name("Vent Light");
    this->code=code;
    this->buttonPin=buttonPin;

    Serial.print("Configuring Zephyr Vent Hood Light 433MHz Transmitter with code: ");
    Serial.print(code,HEX);
    Serial.print("\n");

    new SpanButton(buttonPin);
  }

  StatusCode update(){

    LOG1("Zephyr Vent Hood Light Power: ");
    LOG1(power->getVal());
    if(power->updated()){
      LOG1("->");
      LOG1(power->getNewVal());
    }
    LOG1("  Level: ");
    LOG1(level->getVal());
    if(level->updated()){
      LOG1("->");
      LOG1(level->getNewVal());
    }
    LOG1("\n");    

    int oldState=power->getVal()*(4-level->getVal());    
    int newState=(power->updated()?power->getNewVal():power->getVal())
              *(4-(level->updated()?level->getNewVal():level->getVal()));
 
    while(newState!=oldState){
      transmitZephyr(code);
      delay(200);
      oldState=(oldState+1)%4;
    }

    return(StatusCode::OK);
      
  } // update

  void button(int pin, boolean isLong) override {

    LOG1("Zephyr Vent Hood Light Short Button Press\n");
    transmitZephyr(code);

    int newLevel=level->getVal()-1;
    if(newLevel<0)
      newLevel=3;
    level->setVal(newLevel);
    power->setVal(newLevel>0);

  } // button

  void loop(){

    if(resetLight){
      power->setVal(false);
      level->setVal(0);
      resetLight=false;
    }
    
  } // loop
    
};
      
//////////////////////////////////

struct DEV_ZephyrFan : Service::Fan {

  uint32_t fanCode;
  uint32_t powerCode;
  SpanCharacteristic *power;
  int fanPin;

  DEV_ZephyrFan(uint32_t fanCode, uint32_t powerCode, int fanPin, ServiceType mod=ServiceType::Regular) : Service::Fan(mod){

    power=new Characteristic::Active();
    new Characteristic::Name("Vent Fan");
    this->fanCode=fanCode;
    this->powerCode=powerCode;
    this->fanPin=fanPin;
    
    Serial.print("Configuring Zephyr Vent Hood Fan 433MHz Transmitter with fan code: ");
    Serial.print(fanCode,HEX);
    Serial.print("  power code: ");
    Serial.print(powerCode,HEX);
    Serial.print("\n");

    new SpanButton(fanPin);
  }

  StatusCode update(){

    if(power->getNewVal()){                           // it's okay to repease ON commands - this just cycles through fan speed
      LOG1("Zephyr Vent Hood Fan: Power On\n");
      transmitZephyr(fanCode);
    } else 
    if(power->getVal()){                              // only transmit power code if we know power is really on, else this OFF command will turn fan ON!
      LOG1("Zephyr Vent Hood Fan: Power Off\n");
      transmitZephyr(powerCode);
      resetLight=true;
    }

    return(StatusCode::OK);
    
  } // update

  void button(int pin, boolean isLong) override {

    if(!isLong){
      LOG1("Zephyr Vent Hood Fan Short Button Press: Speed Change\n");
      transmitZephyr(fanCode);
      if(!power->getVal())
        power->setVal(true);
    } else
    if(power->getVal()){
      LOG1("Zephyr Vent Hood Fan Long Button Press: Power Off\n");
      transmitZephyr(powerCode);
      power->setVal(false);      
      resetLight=true;
    } else {
      LOG1("Zephyr Vent Hood Fan Long Button Press: Power is already off!\n");      
      resetLight=true;
    }

  } // button
    
};
            
//////////////////////////////////

void transmitZephyr(uint32_t code){
  char c[32];
  sprintf(c,"Transmitting code: %lx\n",code);
  LOG1(c);
  
  RF433.clear();
  
  for(int b=19;b>0;b--){
    if(code&(1<<b))
      RF433.add(620,230);
    else
      RF433.add(230,620);
  }
      
  if(code&1)
    RF433.add(620,4230);
  else
    RF433.add(230,4620);
    
  RF433.start(8,1);
  
}
