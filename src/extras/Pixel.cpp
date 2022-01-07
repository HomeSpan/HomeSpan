
#include "Pixel.h"

///////////////////

Pixel::Pixel(int pin, float high0, float low0, float high1, float low1, float lowReset){

  H0=high0*80;                            // high0, low0, etc. are all in microseconds and must be multiplied by 80 to match 80MHz RFControl clock
  L0=low0*80;
  H1=high1*80;
  L1=low1*80;
  LR=lowReset*80;  

  rf=new RFControl(pin,false);            // set clock to 1/80 usec
}

///////////////////

void Pixel::setRGB(uint8_t r, uint8_t g, uint8_t b, int nPixels){
  
  if(!*rf)
    return;
    
  rf->clear();
  for(int i=0;i<nPixels;i++)
    loadColor(getColorRGB(r,g,b));
  rf->phase(LR,0);          // end-marker delay/reset
  rf->start();
}

///////////////////

void Pixel::setColors(color_t *color, int nPixels){
  
  if(!*rf)
    return;
    
  rf->clear();
  for(int i=0;i<nPixels;i++)
    loadColor(color[i]);
  rf->phase(LR,0);          // end-marker delay/reset
  rf->start();
}

///////////////////

void Pixel::setHSV(float h, float s, float v, int nPixels){
  float r,g,b;
  LedPin::HSVtoRGB(h,s/100.0,v/100.0,&r,&g,&b);
  setRGB(r*255,g*255,b*255,nPixels);  
}

///////////////////

void Pixel::loadColor(color_t c){
    
  for(int i=23;i>=0;i--){
    if((c>>i)&1)
      rf->add(H1,L1);        // 1-bit
    else
      rf->add(H0,L0);        // 0-bit
  }
}

///////////////////

color_t Pixel::getColorRGB(uint8_t r, uint8_t g, uint8_t b){
  return(g<<16 | r<<8 | b);
}

///////////////////

color_t Pixel::getColorHSV(float h, float s, float v){
  float r,g,b;
  LedPin::HSVtoRGB(h,s/100.0,v/100.0,&r,&g,&b);
  return(getColorRGB(r*255,g*255,b*255));
}

///////////////////
