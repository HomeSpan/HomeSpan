
#include "Pixel.h"

///////////////////

sk68xx::sk68xx(int pin){
  rf=new RFControl(pin,false);
  setRGB(0,0,0);
}

///////////////////

void sk68xx::setRGB(uint8_t r, uint8_t g, uint8_t b){
  
  if(!*rf)
    return;

  Serial.printf("%d %d %d\n",r,g,b);
    
  rf->clear();
  loadColor(g);
  loadColor(r);
  loadColor(b);
  rf->phase(6400,0);          // add 80 usec end-marker delay
  rf->start();
}

///////////////////

void sk68xx::setHSV(float h, float s, float v){
  float r,g,b;
  LedPin::HSVtoRGB(h,s,v,&r,&g,&b);
  setRGB(r*255,g*255,b*255);  
}

///////////////////

void sk68xx::loadColor(uint8_t c){
    
  for(int i=7;i>=0;i--){
    if((c>>i)&1)
      rf->add(51,45);
    else
      rf->add(26,70);
  }
}
