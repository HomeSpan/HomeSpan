
////////////////////////////////////
//      Addressable LED Pixel     //
////////////////////////////////////

#pragma once

#include "RFControl.h"
#include "PwmPin.h"

class Pixel {

  public:
    enum pixel_type_t {
      RGB=8,
      RGBW=0
    };    
 
  private:
    struct pixel_status_t {
      int nPixels;
      const uint32_t *data;
      int iBit;
      int iMem;
      boolean started;
      Pixel *px;
      boolean multiColor;
    };
  
    RFControl *rf;                 // Pixel utilizes RFControl
    uint32_t pattern[2];           // storage for zero-bit and one-bit pulses
    uint32_t resetTime;            // minimum time (in usec) between pulse trains
    uint32_t txEndMask;            // mask for end-of-transmission interrupt
    uint32_t txThrMask;            // mask for threshold interrupt
    pixel_type_t pType;            // type of Pixel (RGB or RGBW)
    
    const int memSize=sizeof(RMTMEM.chan[0].data32)/4;    // determine size (in pulses) of one channel
     
    static void loadData(void *arg);            // interrupt handler 
    volatile static pixel_status_t status;      // storage for volatile information modified in interupt handler   
  
  public:    
    Pixel(int pin, pixel_type_t pType=RGB);                                                 // creates addressable single-wire RGB or RGBW LED on pin (such as the SK68 or WS28)
    
    void setTiming(float high0, float low0, float high1, float low1, uint32_t lowReset);    // changes default timings for bit pulse - note parameters are in MICROSECONDS
      
    void setRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t w=0, uint32_t nPixels=1);          // sets color of nPixels to RGB(W) values (0-255)
    void setHSV(float h, float s, float v, double w=0, uint32_t nPixels=1);                 // sets color of nPixels to HSV(W) values where h=[0,360], s/v/w=[0,100]
    void setColors(const uint32_t *data, uint32_t nPixels, bool multiColor=true);           // sets colors of nPixels from array of colors stored in data
    
    int getPin(){return(rf->getPin());}                                                     // returns pixel pin if valid, else returns -1
    
    static uint32_t getColorRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t w=0);              // return pixel Color from RGB(W) values
    static uint32_t getColorHSV(float h, float s, float v, double w=0);                     // return pixel Color from HSV(W) values
    
    operator bool(){         // override boolean operator to return true/false if creation succeeded/failed
      return(*rf);
    }
};
