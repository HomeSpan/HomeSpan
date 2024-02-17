/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2024 Gregg E. Berman
 *  
 *  https://github.com/HomeSpan/HomeSpan
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *  
 ********************************************************************************/
 
////////////////////////////////////////////
//           Addressable LEDs             //
////////////////////////////////////////////

#include "Pixel.h"

////////////////////////////////////////////
//     Single-Wire RGB/RGBW NeoPixels     //
////////////////////////////////////////////

Pixel::Pixel(int pin, boolean isRGBW){
    
  rf=new RFControl(pin,false,false);          // set clock to 1/80 usec, no default driver
  if(!*rf)
    return;

  if(isRGBW)
    this->lastBit=0;
  else
    this->lastBit=8;
  
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

  onColor.HSV(0,100,100,0);
}

///////////////////

void Pixel::setTiming(float high0, float low0, float high1, float low1, uint32_t lowReset){
  
  pattern[0]=RF_PULSE(high0*80+0.5,low0*80+0.5);
  pattern[1]=RF_PULSE(high1*80+0.5,low1*80+0.5);
  resetTime=lowReset;
}

///////////////////

void Pixel::set(Color *c, int nPixels, boolean multiColor){
  
  if(!*rf || nPixels==0)
    return;

  status.nPixels=nPixels;
  status.color=c;
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
    RMTMEM.chan[status.px->rf->getChannel()].data32[status.iMem++].val=status.px->pattern[(status.color->val>>(--status.iBit))&1];
    
  if(status.iBit==status.px->lastBit){
    status.iBit=32;
    status.color+=status.multiColor;
    status.nPixels--;    
  }
  
  status.iMem%=status.px->memSize;    
}

///////////////////

volatile Pixel::pixel_status_t Pixel::status;

////////////////////////////////////////////
//          Two-Wire RGB DotStars         //
////////////////////////////////////////////

Dot::Dot(uint8_t dataPin, uint8_t clockPin){

  pinMode(dataPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  digitalWrite(dataPin,LOW);
  digitalWrite(clockPin,LOW);

  dataMask=1<<(dataPin%32);
  clockMask=1<<(clockPin%32);

#ifdef CONFIG_IDF_TARGET_ESP32C3
  dataSetReg=&GPIO.out_w1ts.val;
  dataClearReg=&GPIO.out_w1tc.val;
  clockSetReg=&GPIO.out_w1ts.val;
  clockClearReg=&GPIO.out_w1tc.val;
#else
  if(dataPin<32){
    dataSetReg=&GPIO.out_w1ts;
    dataClearReg=&GPIO.out_w1tc;
  } else {
    dataSetReg=&GPIO.out1_w1ts.val;
    dataClearReg=&GPIO.out1_w1tc.val;    
  }

  if(clockPin<32){
    clockSetReg=&GPIO.out_w1ts;
    clockClearReg=&GPIO.out_w1tc;
  } else {
    clockSetReg=&GPIO.out1_w1ts.val;
    clockClearReg=&GPIO.out1_w1tc.val;    
  }
#endif

}

///////////////////

void Dot::set(Color *c, int nPixels, boolean multiColor){
  
  *dataClearReg=dataMask;           // send all zeros
  for(int j=0;j<31;j++){
    *clockSetReg=clockMask;
    *clockClearReg=clockMask;    
  }
  
  for(int i=0;i<nPixels;i++){
    for(int b=31;b>=0;b--){
      if((c->val>>b)&1)
        *dataSetReg=dataMask;
      else
        *dataClearReg=dataMask;
      *clockSetReg=clockMask;
      *clockClearReg=clockMask;
    }
    c+=multiColor;
  }

  *dataClearReg=dataMask;           // send all zeros
  for(int j=0;j<31;j++){
    *clockSetReg=clockMask;
    *clockClearReg=clockMask;    
  }
}

////////////////////////////////////////////
