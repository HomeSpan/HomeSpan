
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
    
  rf->clear();
  loadColor(g);
  loadColor(r);
  loadColor(b);
  rf->phase(6400,0);          // add 80 usec end-marker delay
  rf->start();
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
