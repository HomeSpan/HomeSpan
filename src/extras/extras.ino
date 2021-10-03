/* HomeSpan Remote Control Example */

#include "RFControl.h"     // include RF Control Library

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.print("\n\nHomeSpan RF Transmitter Example\n\n");

  RFControl rf(17);               // create an instance of RFControl with signal output to pin 17 of the ESP32

//  rf.phase(1000,HIGH);
//  rf.phase(9000,LOW);
//  rf.phase(1000,HIGH);
//  rf.phase(9000,LOW);
//  rf.phase(1000,HIGH);
//  rf.phase(30000,LOW);

//  rf.add(1000,9000);              // create a pulse train with three 5000-tick high/low pulses
//  rf.add(1000,9000);              // create a pulse train with three 5000-tick high/low pulses
//  rf.add(1000,9000);              // create a pulse train with three 5000-tick high/low pulses
//  rf.add(1000,30000);              // create a pulse train with three 5000-tick high/low pulses
//
//  rf.start(2,100);
//  Serial.println("Done");
//  while(1);

//
//#define NPOINTS 3
//  
//  uint32_t data[NPOINTS];
//
//  for(int i=0;i<NPOINTS;i++){
//    if(i<NPOINTS-1)
//        data[i]=RF_PULSE(1000,9000);
//    else
//        data[i]=RF_PULSE(1000,30000);
//
//    Serial.println((uint32_t)data[i],HEX);
//  }
// 
//  rf.start(data,NPOINTS,1,100);
//
//  Serial.println("Done.");
//  while(1);
  

  rf.clear();                     // clear the pulse train memory buffer

  rf.add(5000,5000);              // create a pulse train with three 5000-tick high/low pulses
  rf.add(5000,5000);
  rf.add(5000,10000);             // double duration of final low period

  Serial.print("Starting 4 cycles of three 500 ms on pulses...");
  
  rf.start(4,100);                // start transmission of 4 cycles of the pulse train with 1 tick=100 microseconds

  Serial.print("Done!\n");

  delay(2000);

  rf.clear();

  for(int i=1000;i<10000;i+=1000)
    rf.add(i,10000-i);
  rf.add(10000,10000);
  
  Serial.print("Starting 3 cycles of 100-1000 ms pulses...");
  
  rf.start(3,100);                // start transmission of 3 cycles of the pulse train with 1 tick=100 microseconds

  Serial.print("Done!\n");
  
  Serial.print("\nEnd Example");
  
} // end of setup()

void loop(){

} // end of loop()
