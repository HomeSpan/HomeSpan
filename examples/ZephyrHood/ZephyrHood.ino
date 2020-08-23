
#include "HomeSpan.h"

#include "DEV_Identify.h"
#include "DEV_Led.h"
#include "DEV_Zephyr.h"


void setup() {

  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"Example HomeSpan Server");

  new SpanAccessory();
    new DEV_Identify(3,"HomeSpan Bridge","ProRF-32","HomeSpan","ESP32-WROOM","1.0");
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");

  new SpanAccessory();
    new DEV_Identify(0,"Test LED1","ON/OFF Only","HomeSpan","LED-12345","1.9");
    new Service::LightBulb();
      new Characteristic::On();

  new SpanAccessory();
    new DEV_Identify(0,"Test LED2","ON/OFF Only","HomeSpan","LED-12345","1.9");
    new Service::LightBulb();
      new Characteristic::On();

  new SpanAccessory();
    new DEV_Identify(0,"Test LED3","ON/OFF Only","HomeSpan","LED-12345","1.9");
    new Service::LightBulb();
      new Characteristic::On();
      new SpanTimedReset(5000);

  new SpanAccessory();
    new DEV_Identify(0,"Test FAN","Dimmable","HomeSpan","FAN-12345","2.9");
    new Service::Fan();
      new Characteristic::Active();
    new DEV_DimmableLED(15);
      
  new SpanAccessory();
    new DEV_Identify(0,"Dimmable LED 1","Dimmable","HomeSpan","LED-7890","4.9");
    new DEV_DimmableLED(16);

  new SpanAccessory();
    new DEV_Identify(0,"Dimmable LED 2","Dimmable","HomeSpan","LED-7890","4.9");
    new DEV_DimmableLED(17);

  new SpanAccessory();
    new DEV_Identify(0,"Simple FAN","On/Off Light","HomeSpan","FAN-LAMP","2.9");
    new Service::Fan();
      new Characteristic::Active();
    new DEV_DimmableLED(14,ServiceType::Primary);

  new SpanAccessory();
    new DEV_Identify(0,"Zephyr Vent Hood","433 MHz","HomeSpan","ZephyrVH","1.0");
    new DEV_ZephyrLight(0x51390);
    new DEV_ZephyrFan(0x51388);
    new DEV_ZephyrPower(0x61398,ServiceType::Primary);

  new SpanAccessory();
    new DEV_Identify(0,"Zephyr Vent Hood 2","433 MHz","HomeSpan","ZephyrVH","1.0");
    new DEV_ZephyrLight(0x51390,ServiceType::Primary);
    new DEV_ZephyrFan(0x51388);
    new DEV_ZephyrPower(0x61398);

}

//////////////////////////////////////

void loop(){
  homeSpan.poll();
}
