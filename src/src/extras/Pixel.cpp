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

IRAM_ATTR size_t Pixel::pixelEncodeCallback(const void *colors, size_t symbolsTotal,
                     size_t symbolsWritten, size_t symbolsFree,
                     rmt_symbol_word_t *symbols, bool *done, void *arg) {

  if(symbolsWritten==symbolsTotal){         // all symbols have been written
    *done=true;
    return(0);
  } else {
    *done=false;
  }

  callbackArgs_t *callbackArgs=(callbackArgs_t *)arg;
  
  if(symbolsFree < callbackArgs->pixel->symbolsPerPixel)         // not enough space to write an entire pixel
    return(0);

  Color *color = (Color *)colors + (callbackArgs->multiColor ? (symbolsWritten / callbackArgs->pixel->symbolsPerPixel) : 0);

  for(auto i=0; i<callbackArgs->pixel->bytesPerPixel; i++){
    uint8_t colorByte = color->col[callbackArgs->pixel->map[i]];
    for(auto j = 7; j >= 0; j--)
      *symbols++ = (colorByte & (1 << j)) ? callbackArgs->pixel->bit1 : callbackArgs->pixel->bit0;
  }
  
  return(callbackArgs->pixel->symbolsPerPixel);
};

///////////////////

Pixel::Pixel(int pin, const char *pixelType){
    
  this->pin=pin;

  rmt_tx_channel_config_t tx_chan_config;
  memset((void *)&tx_chan_config, 0, sizeof(rmt_tx_channel_config_t));
  tx_chan_config.clk_src = RMT_CLK_SRC_DEFAULT;                       // always use 80MHz clock source
  tx_chan_config.gpio_num = (gpio_num_t)pin;                          // GPIO number
  tx_chan_config.mem_block_symbols = SOC_RMT_MEM_WORDS_PER_CHANNEL;   // set number of symbols to match those in a single channel block
  tx_chan_config.resolution_hz = 80 * 1000 * 1000;                    // set to 80MHz
  tx_chan_config.intr_priority = 3;                                   // medium interrupt priority
  tx_chan_config.trans_queue_depth = 1;                               // set the number of transactions that can pend in the background
  tx_chan_config.flags.invert_out = false;                            // do not invert output signal
  tx_chan_config.flags.with_dma = false;                              // use RMT channel memory, not DMA (most chips do not support use of DMA anyway)
  tx_chan_config.flags.io_loop_back = false;                          // do not use loop-back mode
  tx_chan_config.flags.io_od_mode = false;                            // do not use open-drain output
  
  if(!GPIO_IS_VALID_OUTPUT_GPIO(pin)){
    ESP_LOGE(PIXEL_TAG,"Can't create Pixel(%d) - invalid output pin",pin);
    return;    
  }

  if(rmt_new_tx_channel(&tx_chan_config, &tx_chan)!=ESP_OK){
    ESP_LOGE(PIXEL_TAG,"Can't create Pixel(%d) - no open channels",pin);
    return;
  }
  
  bytesPerPixel=0;
  size_t len=strlen(pixelType);
  boolean invalidMap=false;
  char v[]="RGBWC01234-";                                 // list of valid mapping characters for pixelType
  
  for(int i=0;i<len && i<5;i++){                          // parse and then validate pixelType
    int index=strchrnul(v,toupper(pixelType[i]))-v;
    if(index==strlen(v))                                  // invalid mapping character found
      invalidMap=true;
    map[bytesPerPixel++]=index%5;                         // create pixel map and compute number of bytes per pixel
  }

  if(bytesPerPixel<3 || len>5 || invalidMap){
    ESP_LOGE(PIXEL_TAG,"Can't create Pixel(%d, \"%s\") - invalid pixelType",pin,pixelType);
    return;
  }

  symbolsPerPixel=bytesPerPixel*8;                        // pre-compute and store to save time in callback
  sscanf(pixelType,"%ms",&pType);                         // save pixelType for later use with hasColor()
  
  rmt_enable(tx_chan);                                    // enable channel
  channel=((int *)tx_chan)[0];                            // get channel number
  
  rmt_simple_encoder_config_t simple_config;              // create simple_encoder configuration  
  simple_config.callback = pixelEncodeCallback;           // set callback function to encode data
  simple_config.min_chunk_size=symbolsPerPixel;           // set minimum size to handle a full pixel
  simple_config.arg = &callbackArgs;                      // set callback args  
  rmt_new_simple_encoder(&simple_config, &encoder);       // create simple_encoder using above configuration

  callbackArgs.pixel=this;                                // set callback arg to point back to this pixel instance   
  setTiming(0.32, 0.88, 0.64, 0.56, 80.0);                // set default timing parameters (suitable for most SK68 and WS28 RGB pixels)
  onColor.HSV(0,100,100,0);                               // set onColor
}

///////////////////

Pixel *Pixel::setTiming(float high0, float low0, float high1, float low1, uint32_t lowReset){

  if(channel<0)
    return(this);
  
  bit0.level0=1;
  bit0.duration0=high0*80+0.5;
  bit0.level1=0;
  bit0.duration1=low0*80+0.5;
  
  bit1.level0=1;
  bit1.duration0=high1*80+0.5;
  bit1.level1=0;
  bit1.duration1=low1*80+0.5;

  resetTime=lowReset;
  return(this);
}

///////////////////

void Pixel::set(Color *c, size_t nPixels, boolean multiColor){

  if(channel<0 || nPixels==0)
    return;

  rmt_ll_set_group_clock_src(&RMT, channel, RMT_CLK_SRC_DEFAULT, 1, 0, 0);    // ensure use of DEFAULT CLOCK, which is always 80 MHz, without any scaling

  callbackArgs.multiColor = multiColor;
  rmt_transmit_config_t tx_config{};

  rmt_transmit(tx_chan, encoder, c, nPixels*symbolsPerPixel, &tx_config);     // transmit data (size parameter set to total number of symbols to be written)
  rmt_tx_wait_all_done(tx_chan,-1);                                           // wait until final data is transmitted
  delayMicroseconds(resetTime);                                               // end-of-marker delay
}

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

#if defined(CONFIG_IDF_TARGET_ESP32C3)
  #define OUT_W1TS  &GPIO.out_w1ts.val
  #define OUT_W1TC  &GPIO.out_w1tc.val
  #define OUT1_W1TS  NULL
  #define OUT1_W1TC  NULL
#elif defined(CONFIG_IDF_TARGET_ESP32C6) || defined(CONFIG_IDF_TARGET_ESP32C5)
  #define OUT_W1TS  &GPIO.out_w1ts.val
  #define OUT_W1TC  &GPIO.out_w1tc.val
  #define OUT1_W1TS  &GPIO.out1_w1ts.val
  #define OUT1_W1TC  &GPIO.out1_w1tc.val
#else
  #define OUT_W1TS  &GPIO.out_w1ts
  #define OUT_W1TC  &GPIO.out_w1tc
  #define OUT1_W1TS  &GPIO.out1_w1ts.val
  #define OUT1_W1TC  &GPIO.out1_w1tc.val
#endif

  dataSetReg=     dataPin<32 ? (OUT_W1TS) : (OUT1_W1TS);
  dataClearReg=   dataPin<32 ? (OUT_W1TC) : (OUT1_W1TC);
  clockSetReg=    clockPin<32 ? (OUT_W1TS) : (OUT1_W1TS);
  clockClearReg=  clockPin<32 ? (OUT_W1TC) : (OUT1_W1TC);
}

///////////////////

void Dot::set(Color *c, size_t nPixels, boolean multiColor){
  
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
