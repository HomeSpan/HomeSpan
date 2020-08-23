
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

  DEV_ZephyrLight(uint32_t code, ServiceType mod=ServiceType::Regular) : Service::LightBulb(mod){

    power=new Characteristic::On();
    new SpanTimedReset(500);
    new Characteristic::Name("Vent Light");
    this->code=code;

    Serial.print("Configuring Zephyr Vent Hood Light 433MHz Transmitter with code: ");
    Serial.print(code,HEX);
    Serial.print("\n");
  }

  StatusCode update(){

    LOG1("Updating Zephyr Vent Hood Light:  Power=");
    LOG1(power->value.BOOL?"true":"false");
  
    if(power->isUpdated){
      LOG1("  New Power=");
      LOG1(power->newValue.BOOL?"true":"false");
    }

    LOG1("\n");

    if(power->newValue.BOOL)
      transmitZephyr(code);

    return(StatusCode::OK);
      
  } // update
};
      
//////////////////////////////////

struct DEV_ZephyrFan : Service::Fan {

  uint32_t code;
  SpanCharacteristic *power;

  DEV_ZephyrFan(uint32_t code, ServiceType mod=ServiceType::Regular) : Service::Fan(mod){

    power=new Characteristic::Active();
    new SpanTimedReset(500);
    new Characteristic::Name("Vent Fan");
    this->code=code;

    Serial.print("Configuring Zephyr Vent Hood Fan 433MHz Transmitter with code: ");
    Serial.print(code,HEX);
    Serial.print("\n");
  }

  StatusCode update(){

    LOG1("Updating Zephyr Vent Hood Fan:  Power=");
    LOG1(power->value.BOOL?"true":"false");                 // power is actually a UINT8, but only 0 and 1 are defined so BOOL works as well
  
    if(power->isUpdated){
      LOG1("  New Power=");
      LOG1(power->newValue.BOOL?"true":"false");
    }

    LOG1("\n");

    if(power->newValue.BOOL)
      transmitZephyr(code);
    
    return(StatusCode::OK);
    
  } // update
};
      
//////////////////////////////////

struct DEV_ZephyrPower : Service::Switch {

  uint32_t code;
  SpanCharacteristic *power;

  DEV_ZephyrPower(uint32_t code, ServiceType mod=ServiceType::Regular) : Service::Switch(mod){

    power=new Characteristic::On();
    new SpanTimedReset(500);
    new Characteristic::Name("Vent Power");
    this->code=code;

    Serial.print("Configuring Zephyr Vent Hood Power 433MHz Transmitter with code: ");
    Serial.print(code,HEX);
    Serial.print("\n");
  }

  StatusCode update(){

    LOG1("Updating Zephyr Vent Hood Power:  Power=");
    LOG1(power->value.BOOL?"true":"false");
  
    if(power->isUpdated){
      LOG1("  New Power=");
      LOG1(power->newValue.BOOL?"true":"false");
    }

    LOG1("\n");

    if(power->newValue.BOOL)
      transmitZephyr(code);
    
    return(StatusCode::OK);

  } // update
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
