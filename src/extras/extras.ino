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

  color_t RED=px.getColorRGB(50,0,0);
  
  color_t x0[8]={0,0,0,0,0,0,0,0};

  color_t x1[8]={RED,RED,RED,RED,RED,RED,RED,RED};
  color_t x2[8]={0,0,0,0,0,0,RED,RED};
  color_t x3[8]={0,0,0,0,0,RED,0,0};
  color_t x4[8]={0,0,0,0,RED,0,0,0};
  color_t x5[8]={0,0,0,0,0,RED,0,0};
  color_t x6[8]={0,0,0,0,0,0,RED,RED};
  color_t x7[8]={RED,RED,RED,RED,RED,RED,RED,RED};

  color_t x8[8]={RED,RED,RED,RED,RED,RED,RED,RED};
  color_t x9[8]={RED,0,0,0,RED,0,0,RED};
  color_t x10[8]={RED,0,0,0,RED,0,0,RED};
  color_t x11[8]={RED,0,0,0,RED,0,0,RED};
  color_t x12[8]={RED,0,0,0,0,0,0,RED};
  color_t x13[8]={RED,0,0,0,0,0,0,RED};
  color_t x14[8]={RED,0,0,0,0,0,0,RED};


  uint32_t d=2;
  
  while(1){
    px.setColor(x1,8);
    delay(d);
    px.setColor(x2,8);
    delay(d);
    px.setColor(x3,8);
    delay(d);
    px.setColor(x4,8);
    delay(d);
    px.setColor(x5,8);
    delay(d);
    px.setColor(x6,8);
    delay(d);
    px.setColor(x7,8);
    delay(d);
    px.setColor(x0,8);
    delay(d);
    px.setColor(x8,8);
    delay(d);
    px.setColor(x9,8);
    delay(d);
    px.setColor(x10,8);
    delay(d);
    px.setColor(x11,8);
    delay(d);
    px.setColor(x12,8);
    delay(d);
    px.setColor(x13,8);
    delay(d);
    px.setColor(x14,8);
    delay(d);
    px.setColor(x0,8);
    delay(d);
  }

//  x[0]=Pixel::getColorHSV(0,1,0.2);
//  x[1]=px.getColorHSV(0,0.7,0.2);
//  px.setColor(x,2);
//  delay(1000);
//  x[0]=px.getColorHSV(0,0.7,0.2);
//  x[1]=px.getColorHSV(0,1,0.2);
//  px.setColor(x,2);
//  delay(1000);

} // end of loop()
