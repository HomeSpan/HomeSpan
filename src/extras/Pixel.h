
////////////////////////////////////////////
//           Addressable LEDs             //
////////////////////////////////////////////

#pragma once

#include "RFControl.h"
#include "PwmPin.h"

////////////////////////////////////////////
//     Single-Wire RGB/RGBW NeoPixels     //
////////////////////////////////////////////

class Pixel {

  public: 
    struct Color {
      union{
        struct {
          uint8_t white:8;
          uint8_t blue:8;
          uint8_t red:8;
          uint8_t green:8;
        };
        uint32_t val;
      };
    };
  
  private:
    struct pixel_status_t {
      int nPixels;
      Color *color;
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
    uint32_t lastBit;              // 0=RGBW; 8=RGB
    
    const int memSize=sizeof(RMTMEM.chan[0].data32)/4;    // determine size (in pulses) of one channel
     
    static void loadData(void *arg);            // interrupt handler 
    volatile static pixel_status_t status;      // storage for volatile information modified in interupt handler   
  
  public:    
    Pixel(int pin, boolean isRGBW=false);                                                   // creates addressable single-wire RGB (false) or RGBW (true) LED connected to pin (such as the SK68 or WS28)   
    static Color RGB(uint8_t red, uint8_t green, uint8_t blue, uint8_t white=0);            // returns Color based on provided RGB(W) values where r/g/b/w=[0-255]
    static Color HSV(float h, float s, float v, double w=0);                                // returns Color based on provided HSV(W) values where h=[0,360] and s/v/w=[0,100]
    void set(Color *c, int nPixels, boolean multiColor=true);                               // sets colors of nPixels based on array of Colors c; setting multiColor to false repeats Color in c[0] for all nPixels
    void set(Color c, int nPixels=1){set(&c,nPixels,false);}                                // sets color of nPixels to be equal to specific Color c
              
    int getPin(){return(rf->getPin());}                                                     // returns pixel pin if valid, else returns -1
    void setTiming(float high0, float low0, float high1, float low1, uint32_t lowReset);    // changes default timings for bit pulse - note parameters are in MICROSECONDS
        
    operator bool(){         // override boolean operator to return true/false if creation succeeded/failed
      return(*rf);
    }
};

////////////////////////////////////////////
//          Two-Wire RGB DotStars         //
////////////////////////////////////////////

class Dot {

  public:
    struct Color {
      union{
        struct {
          uint8_t red:8;
          uint8_t green:8;
          uint8_t blue:8;
          uint8_t drive:5;
          uint8_t flags:3;
        };
        uint32_t val;
      };
    };

  private:
    uint32_t dataMask;
    uint32_t clockMask;
    volatile uint32_t *dataSetReg;
    volatile uint32_t *dataClearReg;
    volatile uint32_t *clockSetReg;
    volatile uint32_t *clockClearReg;

  public:
    Dot(uint8_t dataPin, uint8_t clockPin);                                                 // creates addressable two-wire RGB LED connected to dataPin and clockPin (such as the DotStar SK9822 or APA102)
    static Color RGB(uint8_t red, uint8_t green, uint8_t blue, uint8_t drive=31);           // returns Color based on provided RGB values where r/g/b=[0-255] and current-limiting drive=[0,31]
    static Color HSV(float h, float s, float v, double level=100);                          // returns Color based on provided HSV values where h=[0,360], s/v=[0,100], and current-limiting drive=[0,100]
    void set(Color *c, int nPixels, boolean multiColor=true);                               // sets colors of nPixels based on array of Colors c; setting multiColor to false repeats Color in c[0] for all nPixels
    void set(Color c, int nPixels=1){set(&c,nPixels,false);}                                // sets color of nPixels to be equal to specific Color c
};

////////////////////////////////////////////
