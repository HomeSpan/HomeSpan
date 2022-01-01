/* HomeSpan Pixel Example */

#include "Pixel.h"

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.println("\n\nHomeSpan Pixel Example\n\n");

  Pixel px(8);
  Pixel test7(7);
  Pixel test6(6);

  Serial.printf("PX on Pin=%d check: %s\n",px.getPin(),px?"OKAY":"BAD");
  Serial.printf("Test 7 on Pin=%d check: %s\n",test7.getPin(),test7?"OKAY":"BAD");
  Serial.printf("Test 6 on Pin=%d check: %s\n",test6.getPin(),test6?"OKAY":"BAD");

for(int i=0;i<5;i++){
  px.setHSV(60,0.9,0.5);
  delay(1000);
  px.setHSV(120,0.9,0.5);
  delay(1000);
  px.setHSV(240,0.9,0.5);
  delay(1000);
}



//  while(1){
//    for(int i=0;i<50;i++){
//      px.setRGB(i,0,0);
//      delay(2);
//    }
//    for(int i=50;i>=0;i--){
//      px.setRGB(i,0,0);
//      delay(2);
//    }
//  }

  Serial.println("Done!");
  
} // end of setup()

void loop(){

} // end of loop()
