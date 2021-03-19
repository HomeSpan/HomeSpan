
// This is a placeholder .ino file that allows you to easily edit the contents of this library using the Arduino IDE,
// as well as compile and test from this point.  This file is ignored when the library is included in other sketches.

#include "PwmPin.h"

PwmPin yellow(0,16);
PwmPin red(1,17);

void setup(){

  Serial.begin(115200);
  delay(1000);

  Serial.print("\n\nTest sketch for HomeSpan Extras Library\n\n");

  Serial.println("Starting...");
    
}

void loop(){

  for(int i=0;i<100;i++){
    red.set(22,i);
    delay(10);
  }

  for(int i=100;i>=0;i--){
    yellow.set(i);
    delay(10);
  }

}
