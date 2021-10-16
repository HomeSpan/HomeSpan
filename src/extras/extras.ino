/* HomeSpan Remote Control Example */

#include "RFControl.h"     // include RF Control Library

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.println("\n\nHomeSpan RF Transmitter Example");

  RFControl rf(10);               // create an instance of RFControl with signal output to pin 17

  rf.clear();

//  rf.add(1000000,1000000);
//  rf.add(1000000,1000000);
  rf.add(100000,1);

  for(int i=100;i>=0;i-=5){
    rf.enableCarrier(38000,i/100.0);
    rf.start(1,4);
  }
//  rf.disableCarrier();
//  rf.start(1,200);

  while(1);

  uint32_t t0=micros();
  rf.start(4,80);
  uint32_t t1=micros();
   
  Serial.println("End Example");
  Serial.println((t1-t0)/1000);

} // end of setup()

void loop(){

} // end of loop()
