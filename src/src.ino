
// This is a placeholder .ino file that allows you to easily edit the contents of this library using the Arduino IDE,
// as well as compile and test from this point.  This file is ignored when the library is included in other sketches.

#include "HomeSpan.h"

CUSTOM_CHAR_DATA(DataTest, 87654321-079E-48FF-8F27-9C2605A29F52, PW+PR+EV);
Characteristic::DataTest *eveTest;


void setup() {
 
  Serial.begin(115200);

  homeSpan.setLogLevel(2);
  homeSpan.begin(Category::Lighting,"HomeSpan LightBulb");
  
  new SpanAccessory();

    new Service::AccessoryInformation();  
      new Characteristic::Identify();
      
    new Service::LightBulb();
      new Characteristic::On();
      new Characteristic::ConfiguredName();
      eveTest=new Characteristic::DataTest();

  uint8_t x[]={0x01,0x26,0xFF,0x01,0x26,0xFF};
  eveTest->setData(x,6);
  uint8_t y[6]={0};
  int n=eveTest->getData(y,10);
  Serial.printf("%d:",n);
  for(int i=0;i<n;i++){
    Serial.printf(" %02X",y[i]);
    y[i]++;
  }
  Serial.printf("\n\n");
  eveTest->setData(y,6);
  n=eveTest->getData(x,10);
  Serial.printf("%d:",n);
  for(int i=0;i<n;i++){
    Serial.printf(" %02X",x[i]);
  }
  Serial.printf("\n\n");
  
  

}

//////////////////////////////////////

void loop(){

  homeSpan.poll();         // run HomeSpan!
  
}
