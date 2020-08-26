
#include "HomeSpan.h"

#include "DEV_Identify.h"
#include "DEV_Zephyr.h"

void setup() {

  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"ZephyrVH Bridge");

  new SpanAccessory();
    new DEV_Identify("Zephyr VH Bridge","HomeSpan","ZVH-1","HS Bridge","1.0",3);
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");

  new SpanAccessory();
    new DEV_Identify("Zephyr Light","HomeSpan","ZVH-1","RF-Control","1.0",0);
    new DEV_ZephyrLight(0x51390,19);

  new SpanAccessory();
    new DEV_Identify("Zephyr Fan","HomeSpan","ZVH-1","RF-Control","1.0",0);
    new DEV_ZephyrFan(0x51388,0x61398,18);

}

//////////////////////////////////////

void loop(){
  homeSpan.poll();
}
