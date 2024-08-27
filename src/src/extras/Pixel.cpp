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

Pixel::Pixel(int pin, pixelType_t pixelType){
    
  this->pin=pin;

  rmt_tx_channel_config_t tx_chan_config;
  tx_chan_config.clk_src = RMT_CLK_SRC_DEFAULT;                       // always use 80MHz clock source
  tx_chan_config.gpio_num = (gpio_num_t)pin;                          // GPIO number
  tx_chan_config.mem_block_symbols = SOC_RMT_MEM_WORDS_PER_CHANNEL;   // set number of symbols to match those in a single channel block
  tx_chan_config.resolution_hz = 80 * 1000 * 1000;                    // set to 80MHz
  tx_chan_config.trans_queue_depth = 4;                               // set the number of transactions that can pend in the background
  tx_chan_config.flags.invert_out = false;                            // do not invert output signal
  tx_chan_config.flags.with_dma = false;                              // use RMT channel memory, not DMA (most chips do not support use of DMA anyway)
  tx_chan_config.intr_priority = 3;                                   // medium interrupt priority
  
  if(!GPIO_IS_VALID_OUTPUT_GPIO(pin)){
    ESP_LOGE(RFControl_TAG,"Can't create Pixel(%d) - invalid output pin",pin);
    return;    
  }

  if(rmt_new_tx_channel(&tx_chan_config, &tx_chan)!=ESP_OK){
    ESP_LOGE(RFControl_TAG,"Can't create Pixel(%d) - no open channels",pin);
    return;
  }

  rmt_enable(tx_chan);                            // enable channel
  channel=((int *)tx_chan)[0];                    // get channel number

  tx_config.loop_count=0;                         // populate tx_config structure
  tx_config.flags.eot_level=0;
  tx_config.flags.queue_nonblocking=0;
  
  rmt_bytes_encoder_config_t encoder_config;          // can leave blank for now - will populate from within setTiming() below
  rmt_new_bytes_encoder(&encoder_config, &encoder);   // create byte encoder

  map=pixelType;
 
  if(map[3]<255)
    bytesPerPixel=4;
  else
    bytesPerPixel=3;
  
  setTiming(0.32, 0.88, 0.64, 0.56, 80.0);    // set default timing parameters (suitable for most SK68 and WS28 RGB pixels)

  onColor.HSV(0,100,100,0);
}

///////////////////

void Pixel::setTiming(float high0, float low0, float high1, float low1, uint32_t lowReset){
  
  rmt_bytes_encoder_config_t encoder_config;

  encoder_config.bit0.val=RF_PULSE(high0*80+0.5,low0*80+0.5);     // create bit0 and bit1 pulses
  encoder_config.bit1.val=RF_PULSE(high1*80+0.5,low1*80+0.5);
  encoder_config.flags.msb_first=1;                               // MSB of data bytes should be converted and transmitted first
  
  rmt_bytes_encoder_update_config(encoder,&encoder_config);       // update config
  
  resetTime=lowReset;  
}

///////////////////

void Pixel::set(Color *c, int nPixels, boolean multiColor){

  if(channel<0 || nPixels==0)
    return;

  Color data[2];      // temp ping/pong structure to store re-mapped color bytes
  int index=0;        // points to current slot in ping/pong structure

  rmt_ll_set_group_clock_src(&RMT, channel, RMT_CLK_SRC_DEFAULT, 1, 0, 0);        // ensure use of DEFAULT CLOCK, which is always 80 MHz, without any scaling
  
  do {
    for(int i=0;i<bytesPerPixel;i++)                                              // remap colors into ping/pong structure
      data[index].col[i]=c->col[map[i]];

    rmt_tx_wait_all_done(tx_chan,-1);                                             // wait until any outstanding data is transmitted
    rmt_transmit(tx_chan, encoder, data[index].col, bytesPerPixel, &tx_config);   // transmit data
    
    index=1-index;                                                                // flips index to second data structure
    if(multiColor)                                                                // move to next color if multiColor requested
      c++;
  } while(--nPixels>0);

  rmt_tx_wait_all_done(tx_chan,-1);                                               // wait until final data is transmitted
  delayMicroseconds(resetTime);                                                   // end-of-marker delay
}

///////////////////

#ifdef HS_DOT

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

#endif
