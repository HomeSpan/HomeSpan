
#include "Pixel.h"

///////////////////

Pixel::Pixel(int pin, uint32_t nPixels){
    
  rf=new RFControl(pin,false);                // set clock to 1/80 usec
  setTiming(0.32, 0.88, 0.64, 0.56, 80.0);    // set default timing parameters (suitable for most SK68 and WS28 RGB pixels)
  
  if(nPixels==0)        // must reserve at least enough memory for one pixel per transmission batch
    nPixels=1;

  nTrain=nPixels;
}

///////////////////

void Pixel::setTiming(float high0, float low0, float high1, float low1, uint32_t lowReset){
  
  pattern[0]=RF_PULSE(high0*80+0.5,low0*80+0.5);
  pattern[1]=RF_PULSE(high1*80+0.5,low1*80+0.5);
  resetTime=lowReset;
}

///////////////////

void Pixel::setRGB(uint8_t r, uint8_t g, uint8_t b, int nPixels){
  
  if(!*rf)
    return;

  uint32_t *pulses = (uint32_t *) malloc(96);    

  loadColor(getColorRGB(r,g,b),pulses);
  rf->start(pulses,24,nPixels);           // start pulse train and repeat for nPixels
  delayMicroseconds(resetTime);

  free(pulses);
}

///////////////////

void Pixel::setColors(color_t *color, int nPixels){
  
  if(!*rf)
    return;

  uint32_t x0,x1,x2;
    
  x0=micros();

  uint32_t *pulses = (uint32_t *) malloc(nTrain*96);

  for(int i=0;i<nPixels;i++){
    loadColor(color[i],pulses);
    rf->start(pulses,24);                   // start pulse train
  }

  x1=micros();
  Serial.printf("%d\n",x1-x0);

  while(1);
  delayMicroseconds(resetTime);
  free(pulses);
}

///////////////////

void Pixel::setHSV(float h, float s, float v, int nPixels){
  float r,g,b;
  LedPin::HSVtoRGB(h,s/100.0,v/100.0,&r,&g,&b);
  setRGB(r*255,g*255,b*255,nPixels);  
}

///////////////////

void Pixel::loadColor(color_t c, uint32_t *p){

  uint32_t count=24;
  p+=23;
  
  while(count--){
    *p--=pattern[c&1];
    c=c>>1;
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
