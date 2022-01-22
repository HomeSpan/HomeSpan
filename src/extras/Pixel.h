
////////////////////////////////////
//      Addressable LED Pixel     //
////////////////////////////////////

#pragma once

#include "RFControl.h"
#include "PwmPin.h"

class Pixel {

  struct pixel_status_t {
    int nPixels;
    const uint32_t *data;
    int iBit;
    int iMem;
    boolean started;
    Pixel *px;
    boolean multiColor;
  };
  
  private:
    static RFControl *rf;          // Pixel utilizes RFControl (shared across all Pixel instances)
    uint32_t pattern[2];           // storage for zero-bit and one-bit pulses
    uint32_t resetTime;            // minimum time (in usec) between pulse trains
    uint32_t txEndMask;            // mask for end-of-transmission interrupt
    uint32_t txThrMask;            // mask for threshold interrupt
    int pin=-1;                    // pin number to which pixel is connected
    
    const int memSize=sizeof(RMTMEM.chan[0].data32)/4;    // determine size (in pulses) of one channel
     
    static void loadData(void *arg);            // interrupt handler 
    volatile static pixel_status_t status;      // storage for volatile information modified in interupt handler   
  
  public:
    Pixel(int pin);                             // creates addressable single-wire RGB LED on pin (such as the SK68 or WS28), with OPTIONAL reserve of memory for nPixels
    
    void setTiming(float high0, float low0, float high1, float low1, uint32_t lowReset);    // changes default timings for bit pulse - note parameters are in MICROSECONDS
      
    void setRGB(uint8_t r, uint8_t g, uint8_t b, uint32_t nPixels=1);                 // sets color of nPixels to RGB values (0-255)
    void setHSV(float h, float s, float v, uint32_t nPixels=1);                       // sets color of nPixels to HSV values where h=[0,360], s=[0,100], v=[0,100]   
    void setColors(const uint32_t *data, uint32_t nPixels, bool multiColor=true);     // sets colors of nPixels from array of colors stored in data
    
    int getPin(){return(pin);}                                       // returns pixel pin (or -1 if initialization failed)
    
    static uint32_t getColorRGB(uint8_t r, uint8_t g, uint8_t b);    // return pixel Color from RGB values
    static uint32_t getColorHSV(float h, float s, float v);          // return pixel Color from HSV values
    
    operator bool(){         // override boolean operator to return true/false if creation succeeded/failed
      return(*rf);
    }
};
