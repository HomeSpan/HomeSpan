/* HomeSpan Pixel Example */

#include "Pixel.h"

Pixel px(23);

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.println("\n\nHomeSpan Pixel Example\n\n");

  Serial.printf("PX on Pin=%d check: %s\n",px.getPin(),px?"OKAY":"BAD");
  
} // end of setup()

void loop(){

  for(int i=0;i<5;i++){
    px.setHSV(0,1.0,1.0,3);
    delay(1000);
    px.setHSV(120,1.0,1.0,3);
    delay(1000);
    px.setHSV(240,1.0,1.0,3);
    delay(1000);
  }

} // end of loop()
