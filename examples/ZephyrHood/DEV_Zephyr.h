
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

//////////////////////////////////

struct DEV_ZephyrLight : Service::LightBulb {

  uint32_t code;
  SpanCharacteristic *power;
  int buttonPin;

  DEV_ZephyrLight(uint32_t code, int buttonPin, ServiceType mod=ServiceType::Regular) : Service::LightBulb(mod){

    power=new Characteristic::On();
    new Characteristic::Name("Vent Light");
    this->code=code;
    this->buttonPin=buttonPin;

    Serial.print("Configuring Zephyr Vent Hood Light 433MHz Transmitter with code: ");
    Serial.print(code,HEX);
    Serial.print("\n");

    new SpanButton(buttonPin);
  }

  StatusCode update(){
  
    if(power->getNewVal()){
      LOG1("Activating Zephyr Vent Hood Light\n");
      transmitZephyr(code);
    }

    return(StatusCode::OK);
      
  } // update

  void loop(){

    if(power->getVal() && power->timeVal()>500){   // check that power is true, and that time since last modification is greater than 3 seconds 
      LOG1("Resetting Zephyr Vent Hood Light Control\n");     // log message  
      power->setVal(false);                         // set power to false
    }      
    
  } // loop  

  void button(int pin, boolean isLong) override {

    LOG1("Activating Zephyr Vent Hood Light\n");
    transmitZephyr(code);
    power->setVal(true); 

  } // button
    
};
      
//////////////////////////////////

struct DEV_ZephyrFan : Service::Fan {

  uint32_t code;
  SpanCharacteristic *power;
  int buttonPin;

  DEV_ZephyrFan(uint32_t code, int buttonPin, ServiceType mod=ServiceType::Regular) : Service::Fan(mod){

    power=new Characteristic::Active();
    new Characteristic::Name("Vent Fan");
    this->code=code;
    this->buttonPin=buttonPin;
    
    Serial.print("Configuring Zephyr Vent Hood Fan 433MHz Transmitter with code: ");
    Serial.print(code,HEX);
    Serial.print("\n");

    new SpanButton(buttonPin);
  }

  StatusCode update(){

    if(power->getNewVal()){
      LOG1("Activating Zephyr Vent Hood Fan\n");
      transmitZephyr(code);
    }

    return(StatusCode::OK);
    
  } // update

  void loop(){

    if(power->getVal() && power->timeVal()>500){   // check that power is true, and that time since last modification is greater than 3 seconds 
      LOG1("Resetting Zephyr Vent Hood Fan Control\n");     // log message  
      power->setVal(false);                         // set power to false
    }      
    
  } // loop  

  void button(int pin, boolean isLong) override {

    LOG1("Activating Zephyr Vent Hood Fan\n");
    transmitZephyr(code);
    power->setVal(true); 

  } // button
    
};
      
//////////////////////////////////

struct DEV_ZephyrPower : Service::Switch {

  uint32_t code;
  SpanCharacteristic *power;
  int buttonPin;

  DEV_ZephyrPower(uint32_t code, int buttonPin, ServiceType mod=ServiceType::Regular) : Service::Switch(mod){

    power=new Characteristic::On();
    new Characteristic::Name("Vent Power");
    this->code=code;
    this->buttonPin=buttonPin;

    Serial.print("Configuring Zephyr Vent Hood Power 433MHz Transmitter with code: ");
    Serial.print(code,HEX);
    Serial.print("\n");
    new SpanButton(buttonPin);

  }

  StatusCode update(){

    if(power->getNewVal()){
      LOG1("Activating Zephyr Vent Hood Power\n");
      transmitZephyr(code);
    }
    
    return(StatusCode::OK);

  } // update

  void loop(){

    if(power->getVal() && power->timeVal()>500){   // check that power is true, and that time since last modification is greater than 3 seconds 
      LOG1("Resetting Zephyr Vent Hood Power Control\n");     // log message  
      power->setVal(false);                         // set power to false
    }      
    
  } // loop  

  void button(int pin, boolean isLong) override {

    LOG1("Activating Zephyr Vent Hood Power\n");
    transmitZephyr(code);
    power->setVal(true); 

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
