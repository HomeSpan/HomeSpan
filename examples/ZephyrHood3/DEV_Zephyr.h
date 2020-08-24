
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

  uint32_t lightCode;
  SpanCharacteristic *power;
  int lightPin;
  int state=0;                        // 0=off, 1=dim, 2=medium, 3=bright

  DEV_ZephyrLight(uint32_t lightCode, int lightPin, ServiceType mod=ServiceType::Regular) : Service::LightBulb(mod){

    power=new Characteristic::On();
    new Characteristic::Name("Vent Light");
    this->lightCode=lightCode;
    this->lightPin=lightPin;

    Serial.print("Configuring Zephyr Vent Hood Light 433MHz Transmitter with code: ");
    Serial.print(lightCode,HEX);
    Serial.print("\n");

    new SpanButton(lightPin);
  }

  StatusCode update(){

    if(!power->getVal() && power->getNewVal()){      // only transmit code to turn on light if we know power is really off
      LOG1("Zephyr Vent Hood Light: Power On\n");
      transmitZephyr(lightCode);
      state=3;                                       // light always turns on in brightest setting
    } else 
    
    if(!power->getNewVal()){                         // send light code until state=0
      LOG1("Zephyr Vent Hood Light: Power Off\n");
      while(state){
        transmitZephyr(lightCode);
        state--;
      }
    }    

    return(StatusCode::OK);
      
  } // update

  void button(int pin, boolean isLong) override {

    LOG1("Zephyr Vent Hood Light Short Button Press\n");
    transmitZephyr(lightCode);

    state--;                      // decrement state
    
    if(state==0){                 // if reached zero, set power to OFF
      power->setVal(false);
    } else
    
    if(state==-1){                // if state was already zero, reset to 3 and set power to ON
      state=3;
      power->setVal(true);
    }
    
  } // button

  void loop(){

    if(resetLight){             
      power->setVal(false);
      state=0;
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

    if(power->getNewVal()){                           // it's okay to repeat ON commands - this just cycles through fan speed
      LOG1("Zephyr Vent Hood Fan: Power On\n");
      transmitZephyr(fanCode);
    } else 
    
    if(power->getVal()){                              // only transmit power code if we know power is really on, else this OFF command will turn everything ON!
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

  delay(200);               // wait 200 ms before returning to ensure vent has sufficient time to process request
  
}
