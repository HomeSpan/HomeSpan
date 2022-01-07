
////////////////////////////////////
//      Addressable LED Pixel     //
////////////////////////////////////

#pragma once

#include "RFControl.h"
#include "PwmPin.h"

typedef uint32_t color_t;

class Pixel {
  private:
    uint32_t H0, L0;      // High and Low times for a zero-pulse (in units of 1/80 microseconds)
    uint32_t H1, L1;      // High and Low times for a one-pulse (in units of 1/80 microseconds)
    uint32_t LR;          // Low time for a reset/end-of-data (in units of 1/80 microseconds)
    
    RFControl *rf;
    void loadColor(color_t c);    // creates pulse pattern for pixel color (encoded as RGB in low 24-bits)
  
  public:
    Pixel(int pin, float high0, float low0, float high1, float low1, float lowReset);    // creates addressable single-wire RGB LED on pin (such as the SK68 or WS28); parameters are in MICROSECONDS!
    Pixel(int pin) :  Pixel(pin, 0.32, 0.88, 0.64, 0.56, 80.0) {};                       // default parameters for SK68XXMINI-HS LEDs, though will likely work with many other variations as well
    
    void setRGB(uint8_t r, uint8_t g, uint8_t b, int nPixels=1);    // sets color of nPixels to RGB values (0-255)
    void setHSV(float h, float s, float v, int nPixels=1);          // sets color of nPixels to HSV values where h=[0,360], s=[0,1], v=[0,1]   
    void setColor(color_t *color, int nPixels);                     // sets color of nPixels from array of Colors
    int getPin(){return(rf->getPin());}                             // returns pixel pin if valid, else returns -1
    
    static color_t getColorRGB(uint8_t r, uint8_t g, uint8_t b);            // return pixel Color from RGB values
    static color_t getColorHSV(float h, float s, float v);                  // return pixel Color from HSV values

    operator bool(){         // override boolean operator to return true/false if creation succeeded/failed
      return(*rf);
    }
};
