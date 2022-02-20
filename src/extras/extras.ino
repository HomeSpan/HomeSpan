// This is a placeholder .ino file that allows you to easily edit the contents of this files using the Arduino IDE,
// as well as compile and test from this point.  This file is ignored when the library is included in other sketches.

#include "Pixel.h"

//#define PixelType   Pixel
#define PixelType   Dot

//Pixel p(8);
Dot p(0,1);

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.println("\n\nHomeSpan Pixel Example\n");

  PixelType::Color off=PixelType::RGB(0,0,0);

  p.set(PixelType::RGB(0,0,255),3);
  delay(1000);

  p.set(off,3);
  delay(1000);

  PixelType::Color c[]={p.HSV(120,100,30),p.HSV(0,0,0),p.HSV(0,0,0)};
  p.set(c,3);
  delay(1000);

  c[0].HSV(0,0,0);
  c[1].HSV(60,100,30);
  p.set(c,3);
  delay(1000);
  
  c[1].HSV(0,0,0);
  c[2].HSV(0,100,30);
  p.set(c,3);
}

void loop(){
}
