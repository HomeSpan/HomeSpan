
// This is a placeholder .ino file that allows you to easily edit the contents of this library using the Arduino IDE,
// as well as compile and test from this point.  This file is ignored when the library is included in other sketches.

#include "PwmPin.h"

void setup(){
 
  Serial.begin(115200);
  delay(1000);

  Serial.print("\n\nTest sketch for HomeSpan Extras Library\n\n");

  Serial.println("Starting...");

  LedPin *led[20];

//  uint8_t p[]={33,27,4,32,18,19,16,17,5};       // ESP32 test
    uint8_t p[]={11,7,3,1,38,33,9,10};          // ESP32-S2 test

  for(int i=0;i<sizeof(p);i++)
    led[i]=new LedPin(p[i],20,5000);    

  led[7]->set(100);

  while(1);
}

void loop(){
}
