
#include "Pixel.h"

///////////////////

Pixel::Pixel(int pin){
    
  rf=new RFControl(pin,false,false);          // set clock to 1/80 usec, no default driver
  setTiming(0.32, 0.88, 0.64, 0.56, 80.0);    // set default timing parameters (suitable for most SK68 and WS28 RGB pixels)

  rmt_isr_register(loadData,(void *)this,0,NULL);       // set custom interrupt handler
  rmt_set_tx_thr_intr_en(rf->getChannel(),true,8);      // enable threshold interrupt (note end-transmission interrupt automatically enabled by rmt_tx_start)

  txEndMask=TxEndMask(rf->getChannel());      // create bit mask for end-of-transmission interrupt specific to this channel

}

///////////////////

void Pixel::setTiming(float high0, float low0, float high1, float low1, uint32_t lowReset){
  
  pattern[0]=RF_PULSE(high0*80+0.5,low0*80+0.5);
  pattern[1]=RF_PULSE(high1*80+0.5,low1*80+0.5);
  resetTime=lowReset;
}

///////////////////

void Pixel::setRGB(uint8_t r, uint8_t g, uint8_t b, uint32_t nPixels){
  
  if(!*rf || nPixels==0)
    return;

  uint32_t data=getColorRGB(r,g,b);
  setColors(&data,nPixels,false);
}

///////////////////

void Pixel::setHSV(float h, float s, float v, uint32_t nPixels){
  float r,g,b;
  LedPin::HSVtoRGB(h,s/100.0,v/100.0,&r,&g,&b);
  setRGB(r*255,g*255,b*255,nPixels);  
}

///////////////////

void Pixel::setColors(const uint32_t *data, uint32_t nPixels, boolean multiColor){
  
  if(!*rf || nPixels==0)
    return;

  status.nPixels=nPixels;
  status.data=data;
  status.iMem=0;
  status.iBit=24;
  status.started=true;
  this->multiColor=multiColor;

  loadData(this);         // load first two bytes of data to get started
  loadData(this);

  rmt_tx_start(rf->getChannel(),true);

  while(status.started);            // wait for transmission to be complete
  delayMicroseconds(resetTime);     // end-of-marker delay
}

///////////////////

uint32_t Pixel::getColorRGB(uint8_t r, uint8_t g, uint8_t b){
  return(g<<16 | r<<8 | b);
}

///////////////////

uint32_t Pixel::getColorHSV(float h, float s, float v){
  float r,g,b;
  LedPin::HSVtoRGB(h,s/100.0,v/100.0,&r,&g,&b);
  return(getColorRGB(r*255,g*255,b*255));
}

///////////////////

void Pixel::loadData(void *arg){

  Pixel *pix=(Pixel *)arg;

  if(RMT.int_st.val & pix->txEndMask){
    RMT.int_clr.val=~0;
    status.started=false;
    return;
  }
  
  RMT.int_clr.val=~0;

  if(status.nPixels==0){
    RMTMEM.chan[pix->rf->getChannel()].data32[status.iMem].val=0;
    return;
  }
  
  for(int i=0;i<8;i++)
    RMTMEM.chan[pix->rf->getChannel()].data32[status.iMem++].val=pix->pattern[(*status.data>>(--status.iBit))&1];
    
  if(status.iBit==0){
    status.iBit=24;
    status.data+=pix->multiColor;
    status.nPixels--;    
  }
  
  status.iMem%=pix->memSize;    
}

///////////////////

volatile Pixel::pixel_status_t Pixel::status;
