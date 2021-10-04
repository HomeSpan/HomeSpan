/* HomeSpan Remote Control Example */

#include "RFControl.h"     // include RF Control Library

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.println("\n\nHomeSpan RF Transmitter Example");

  RFControl rf(18);               // create an instance of RFControl with signal output to pin 6

#define NPOINTS 3
  
  uint32_t data[NPOINTS];

  for(int i=0;i<NPOINTS;i++){
    if(i<NPOINTS-1)
        data[i]=RF_PULSE(1000,9000);
    else
        data[i]=RF_PULSE(1000,30000);
  }
 
  rf.start(data,NPOINTS,2,100);
   
  Serial.println("End Example");
  
} // end of setup()

void loop(){

} // end of loop()
