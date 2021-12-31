
////////////////////////////////////
//      Addressable LED Pixel     //
////////////////////////////////////

#pragma once

#include "RFControl.h"
#include "PwmPin.h"

class sk68xx {
  private:
    int pin=-1;
    RFControl *rf;
    void loadColor(uint8_t c);
  
  public:
    sk68xx(int pin);                                  // creates addressable SK68XX RGB LED on pin
    void setRGB(uint8_t r, uint8_t g, uint8_t b);     // sets color to rgb values (0-255)
    void setHSV(float h, float s, float v);           // sets color to hsv values where h=[0,360], s=[0,1], v=[0,1]
    int getPin(){return(rf->getPin());}               // returns pin number

    operator bool(){         // override boolean operator to return true/false if creation succeeded/failed
      return(*rf);
    }
};
