/* HomeSpan Pixel Example */

#include "Pixel.h"

Pixel px(21);

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.println("\n\nHomeSpan Pixel Example\n\n");

  Serial.printf("PX on Pin=%d check: %s\n",px.getPin(),px?"OKAY":"BAD");

  px.setRGB(0,0,0,8);
  
} // end of setup()

void loop(){

//  px.setHSV(0,1.0,0.2,8);
//  delay(1000);
//  px.setHSV(120,1.0,0.2,4);
//  delay(1000);
//  px.setHSV(240,1.0,0.2,2);
//  delay(1000);

  color_t x[2];

  x[0]=Pixel::getColorHSV(0,1,0.2);
  x[1]=px.getColorHSV(0,0.7,0.2);
  px.setColor(x,2);
  delay(1000);
  x[0]=px.getColorHSV(0,0.7,0.2);
  x[1]=px.getColorHSV(0,1,0.2);
  px.setColor(x,2);
  delay(1000);

} // end of loop()
