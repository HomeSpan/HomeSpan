
#include "Pixel.h"

///////////////////

Pixel::Pixel(int pin, uint32_t nPixels){
    
  rf=new RFControl(pin,false,false);          // set clock to 1/80 usec, no default driver
  setTiming(0.32, 0.88, 0.64, 0.56, 80.0);    // set default timing parameters (suitable for most SK68 and WS28 RGB pixels)

  rmt_isr_register(isrHandler,(void *)this,0,NULL);     // end-transmission interrupt automatically enabled by rmt_tx_start
  rmt_set_tx_thr_intr_en(rf->getChannel(),true,8);      // need to also enable threshold interrupt
  channelNum=rf->getChannel();

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

  uint32_t *pulses = (uint32_t *) malloc(24*sizeof(uint32_t));    

  loadColor(getColorRGB(r,g,b),pulses);
  rf->start(pulses,24,nPixels);           // start pulse train and repeat for nPixels
  delayMicroseconds(resetTime);

  free(pulses);
}

///////////////////

void Pixel::setColors(const uint32_t *data, uint32_t nPixels){
  
  if(!*rf || nPixels==0)
    return;

  status.nPixels=nPixels;
  status.data=data;
  status.iMem=0;
  status.iBit=24;
  status.started=true;
  status.txEndMask=TxEndMask(channelNum);

  loadData();             // load first 2 bytes
  loadData();

  rmt_tx_start(rf->getChannel(),true);

  while(status.started);


  return;

//  uint32_t *pulses = (uint32_t *) malloc(nTrain*24*sizeof(uint32_t));
//
//  if(!pulses){
//    Serial.printf("*** ERROR:  Not enough memory to reserve for %d Pixels per batch transmission\n",nTrain);
//    return;
//  }
//
//  int i,j;
//  
//  for(i=0;i<nPixels;){
//    for(j=0;j<nTrain && i<nPixels;j++,i++)
//      loadColor(color[i],pulses+j*24);
//    rf->start(pulses,j*24);
//  }
//
//  free(pulses);
//  delayMicroseconds(resetTime);
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

void Pixel::loadData(){
  
  if(status.nPixels==0){
    RMTMEM.chan[channelNum].data32[status.iMem].val=0;
    return;
  }
  
  for(int i=0;i<8;i++)
    RMTMEM.chan[channelNum].data32[status.iMem++].val=pattern[(*status.data>>(--status.iBit))&1];
    
  if(status.iBit==0){
    status.iBit=24;
    status.data++;
    status.nPixels--;    
  }
  status.iMem%=memSize;
}

///////////////////

void Pixel::isrHandler(void *arg){

  Pixel *pix=(Pixel *)arg;

  if(RMT.int_st.val & status.txEndMask){
    RMT.int_clr.val=~0;
    status.started=false;
    return;
  }
  
  RMT.int_clr.val=~0;
  pix->loadData();
}

///////////////////

volatile pixel_status_t Pixel::status;
