
////////////////////////////////////
//      Addressable LED Pixel     //
////////////////////////////////////

#pragma once

#include "RFControl.h"
#include "PwmPin.h"

struct pixel_status_t {
  int nPixels;
  const uint32_t *data;
  int iBit;
  int iMem;
  boolean started;
};

class Pixel {
  private:
    uint32_t pattern[2];           // storage for zero-bit and one-bit pulses
    uint32_t resetTime;            // minimum time (in usec) between pulse trains
    int channelNum;                // channel number
    uint32_t txEndMask;            // mask for end-of-transmission interrupt
 
    uint32_t nTrain;               // number of Pixels to transmit per pulse train batch

  #if defined(CONFIG_IDF_TARGET_ESP32)
    const int memSize=64;
    #define TxEndMask(chNum) (1<<(chNum*3))
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
    const int memSize=48;
    #define TxEndMask(chNum) (1<<(chNum*3))
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
    const int memSize=48;
    #define TxEndMask(chNum) (1<<chNum)
  #else
    const int memSize=0;
  #endif
    
    RFControl *rf;

    volatile static pixel_status_t status;
    
    static void loadData(void *arg);            // interrupt handler 
    void loadColor(uint32_t c, uint32_t *p);    // creates pulse pattern for pixel color (encoded as RGB in low 24-bits of *p)
  
  public:
    Pixel(int pin, uint32_t nPixels=1);                             // creates addressable single-wire RGB LED on pin (such as the SK68 or WS28), with OPTIONAL reserve of memory for nPixels
    
    void setTiming(float high0, float low0, float high1, float low1, uint32_t lowReset);    // changes default timings for bit pulse - note parameters are in MICROSECONDS
      
    void setRGB(uint8_t r, uint8_t g, uint8_t b, int nPixels=1);    // sets color of nPixels to RGB values (0-255)
    void setHSV(float h, float s, float v, int nPixels=1);          // sets color of nPixels to HSV values where h=[0,360], s=[0,100], v=[0,100]   
    void setColors(const uint32_t *data, uint32_t nPixels);              // sets colors of nPixels from array of Colors
    int getPin(){return(rf->getPin());}                             // returns pixel pin if valid, else returns -1
    
    static uint32_t getColorRGB(uint8_t r, uint8_t g, uint8_t b);    // return pixel Color from RGB values
    static uint32_t getColorHSV(float h, float s, float v);          // return pixel Color from HSV values
    
    operator bool(){         // override boolean operator to return true/false if creation succeeded/failed
      return(*rf);
    }
};
