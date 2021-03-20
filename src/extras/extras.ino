
// This is a placeholder .ino file that allows you to easily edit the contents of this library using the Arduino IDE,
// as well as compile and test from this point.  This file is ignored when the library is included in other sketches.

#include "PwmPin.h"

PwmPin yellow(0,16);
PwmPin red(0,17);

//ServoPin servo(3,18,-90);
ServoPin servo(3,18,0,500,2200,-90,90);

void setup(){
 
  Serial.begin(115200);
  delay(1000);

  Serial.print("\n\nTest sketch for HomeSpan Extras Library\n\n");

  Serial.println("Starting...");

}

void loop(){

  
  double STEP=1;

  for(int i=-100*STEP;i<=100*STEP;i++){
    servo.set((double)i/STEP);
    delay(10);
  }

  for(int i=100*STEP;i>=-100*STEP;i--){
    servo.set((double)i/STEP);
    delay(10);
  }
 
  return;

  for(int i=0;i<100;i++){
    red.set(22,i);
    delay(10);
  }

  for(int i=100;i>=0;i--){
    yellow.set(i);
    delay(10);
  }

}
