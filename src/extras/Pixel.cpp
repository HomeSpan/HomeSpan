
#include "Pixel.h"

///////////////////

Pixel::Pixel(int pin, float high0, float low0, float high1, float low1, float lowReset){

  H0=high0*80;                            // high0, low0, etc. are all in microseconds and must be multiplied by 80 to match 80MHz RFControl clock
  L0=low0*80;
  H1=high1*80;
  L1=low1*80;
  LR=lowReset*80;  

  rf=new RFControl(pin,false);            // set clock to 1/80 usec
  setRGB(0,255,0);
}

///////////////////

void Pixel::setRGB(uint8_t r, uint8_t g, uint8_t b){
  
  if(!*rf)
    return;
    
  rf->clear();
  loadColor(g);
  loadColor(r);
  loadColor(b);
  rf->phase(LR,0);          // end-marker delay/reset
  rf->start();
}

///////////////////

void Pixel::setHSV(float h, float s, float v){
  float r,g,b;
  LedPin::HSVtoRGB(h,s,v,&r,&g,&b);
  setRGB(r*255,g*255,b*255);  
}

///////////////////

void Pixel::loadColor(uint8_t c){
    
  for(int i=7;i>=0;i--){
    if((c>>i)&1)
      rf->add(H1,L1);        // 1-bit
    else
      rf->add(H0,L0);        // 0-bit
  }
}
