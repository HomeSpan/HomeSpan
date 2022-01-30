
#include "Pixel.h"

///////////////////

Pixel::Pixel(int pin, pixel_type_t pType){
    
  rf=new RFControl(pin,false,false);          // set clock to 1/80 usec, no default driver
  if(!*rf)
    return;

  this->pType=pType;
  
  setTiming(0.32, 0.88, 0.64, 0.56, 80.0);    // set default timing parameters (suitable for most SK68 and WS28 RGB pixels)

  rmt_isr_register(loadData,NULL,0,NULL);               // set custom interrupt handler
  
  rmt_set_tx_thr_intr_en(rf->getChannel(),false,8);     // disable threshold interrupt
  txThrMask=RMT.int_ena.val;                            // save interrupt enable vector
  rmt_set_tx_thr_intr_en(rf->getChannel(),true,8);      // enable threshold interrupt to trigger every 8 pulses 
  txThrMask^=RMT.int_ena.val;                           // find bit that flipped and save as threshold mask for this channel 

  rmt_set_tx_intr_en(rf->getChannel(),false);           // disable end-of-transmission interrupt
  txEndMask=RMT.int_ena.val;                            // save interrupt enable vector
  rmt_set_tx_intr_en(rf->getChannel(),true);            // enable end-of-transmission interrupt
  txEndMask^=RMT.int_ena.val;                           // find bit that flipped and save as end-of-transmission mask for this channel 
}

///////////////////

void Pixel::setTiming(float high0, float low0, float high1, float low1, uint32_t lowReset){
  
  pattern[0]=RF_PULSE(high0*80+0.5,low0*80+0.5);
  pattern[1]=RF_PULSE(high1*80+0.5,low1*80+0.5);
  resetTime=lowReset;
}

///////////////////

void Pixel::setRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint32_t nPixels){
  
  if(!*rf || nPixels==0)
    return;

  uint32_t data=getColorRGB(r,g,b,w);
  setColors(&data,nPixels,false);
}

///////////////////

void Pixel::setHSV(float h, float s, float v, double w, uint32_t nPixels){
  float r,g,b;
  LedPin::HSVtoRGB(h,s/100.0,v/100.0,&r,&g,&b);
  setRGB(r*255,g*255,b*255,w*2.555,nPixels);  
}

///////////////////

void Pixel::setColors(const uint32_t *data, uint32_t nPixels, boolean multiColor){
  
  if(!*rf || nPixels==0)
    return;

  status.nPixels=nPixels;
  status.data=data;
  status.iMem=0;
  status.iBit=32;
  status.started=true;
  status.px=this;
  status.multiColor=multiColor;

  loadData(this);         // load first two bytes of data to get started
  loadData(this);

  rmt_tx_start(rf->getChannel(),true);

  while(status.started);            // wait for transmission to be complete
  delayMicroseconds(resetTime);     // end-of-marker delay
}

///////////////////

uint32_t Pixel::getColorRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t w){
//  return(g<<16 | r<<8 | b);
  return(g<<24 | r<<16 | b<<8 | w);
}

///////////////////

uint32_t Pixel::getColorHSV(float h, float s, float v, double w){
  float r,g,b;
  LedPin::HSVtoRGB(h,s/100.0,v/100.0,&r,&g,&b);
  return(getColorRGB(r*255,g*255,b*255,w*2.555));
}

///////////////////

void IRAM_ATTR Pixel::loadData(void *arg){

  if(RMT.int_st.val & status.px->txEndMask){
    RMT.int_clr.val=status.px->txEndMask;
    status.started=false;
    return;
  }
  
  RMT.int_clr.val=status.px->txThrMask;       // if loadData() is called and it is NOT because of an END interrupt (above) then must either be a pre-load, or a threshold trigger

  if(status.nPixels==0){
    RMTMEM.chan[status.px->rf->getChannel()].data32[status.iMem].val=0;
    return;
  }
  
  for(int i=0;i<8;i++)
    RMTMEM.chan[status.px->rf->getChannel()].data32[status.iMem++].val=status.px->pattern[(*status.data>>(--status.iBit))&1];
    
  if(status.iBit==status.px->pType){
    status.iBit=32;
    status.data+=status.multiColor;
    status.nPixels--;    
  }
  
  status.iMem%=status.px->memSize;    
}

///////////////////

volatile Pixel::pixel_status_t Pixel::status;
