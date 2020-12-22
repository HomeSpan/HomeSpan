
// This is a placeholder .ino file that allows you to easily edit the contents of this library using the Arduino IDE,
// as well as compile and test from this point.  This file is ignored when the library is included in other sketches.

#include "RFControl.h"

void setup(){

  Serial.begin(115200);
  delay(1000);

  Serial.print("\n\nTest sketch for HomeSpan Extras Library\n\n");

  RFControl rf(4);

  Serial.println("Starting...");
  
  rf.clear();
  for(int i=0;i<3;i++)
    rf.add(2000,2000);
  rf.phase(10000,0); 
  rf.start(5,100);
  
  Serial.println("Done!");
  
}

void loop(){
  
}
