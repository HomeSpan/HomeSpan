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

#pragma once

#include "PwmPin.h"
#include "Blinker.h"

#pragma GCC diagnostic ignored "-Wvolatile"

#include <driver/rmt_tx.h>      // IDF 5 RMT driver
#include <soc/rmt_struct.h>     // where RMT register structure is defined
#include <hal/rmt_ll.h>         // where low-level RMT calls are defined

#include <soc/gpio_struct.h>

[[maybe_unused]] static const char* PIXEL_TAG = "Pixel";

////////////////////////////////////////////
//     Single-Wire RGB/RGBW NeoPixels     //
////////////////////////////////////////////

class Pixel : public Blinkable {

  public:
    struct Color {
      uint8_t col[5];

      Color RGB(uint8_t r, uint8_t g, uint8_t b, uint8_t w=0, uint8_t c=0){         // returns Color based on provided RGB(W) values where r/g/b/w=[0-255]
        col[0]=r;
        col[1]=g;
        col[2]=b;
        col[3]=w;
        col[4]=c;
        return(*this);
      }

      Color HSV(float h, float s, float v, double w=0, double c=0){                // returns Color based on provided HSV(W) values where h=[0,360] and s/v/w/c=[0,100]
        float r,g,b;
        LedPin::HSVtoRGB(h,s/100.0,v/100.0,&r,&g,&b);
        col[0]=r*255;
        col[1]=g*255;
        col[2]=b*255;
        col[3]=w*2.555;
        col[4]=c*2.555;
        return(*this);
      }      

      bool operator==(const Color& color){
        boolean eq=true;
        for(int i=0;i<5;i++)
          eq&=(col[i]==color.col[i]);
        return(eq);          
      }
      
      bool operator!=(const Color& color){
        return(!(*this==color));
      }

      Color operator+(const Color& color){
        Color newColor;
        for(int i=0;i<5;i++)
          newColor.col[i]=col[i]+color.col[i];
        return(newColor);
      }

      Color& operator+=(const Color& color){
        for(int i=0;i<5;i++)
          col[i]+=color.col[i];
        return(*this);
      }
            
      Color operator-(const Color& color){
        Color newColor;
        for(int i=0;i<5;i++)
          newColor.col[i]=col[i]-color.col[i];
        return(newColor);
      }

      Color& operator-=(const Color& color){
        for(int i=0;i<5;i++)
          col[i]-=color.col[i];
        return(*this);
      }
            
    }; // Color
  
  private:
    uint8_t pin;
    int channel=-1;
    rmt_channel_handle_t tx_chan = NULL;
    rmt_encoder_handle_t encoder;
    rmt_transmit_config_t tx_config;
  
    uint32_t resetTime;            // minimum time (in usec) between pulse trains
    uint8_t bytesPerPixel;         // WC=2, RGB=3, RGBW=5, RGBWC=5
    uint8_t map[5];                // color map representing order in which color bytes are transmitted
    Color onColor;                 // color used for on() command
  
  public:
    Pixel(int pin, const char *pixelType="GRB");                     // creates addressable single-wire LED of pixelType connected to pin (such as the SK68 or WS28)   
    void set(Color *c, int nPixels, boolean multiColor=true);        // sets colors of nPixels based on array of Colors c; setting multiColor to false repeats Color in c[0] for all nPixels
    void set(Color c, int nPixels=1){set(&c,nPixels,false);}         // sets color of nPixels to be equal to specific Color c
    
    static Color RGB(uint8_t r, uint8_t g, uint8_t b, uint8_t w=0){return(Color().RGB(r,g,b,w));}  // an alternative method for returning an RGB Color
    static Color HSV(float h, float s, float v, double w=0){return(Color().HSV(h,s,v,w));}         // an alternative method for returning an HSV Color
              
    int getPin(){return(channel>=0?pin:-1);}                                                // returns pixel pin if channel is valid, else returns -1
    boolean isRGBW(){return(bytesPerPixel==4);}                                             // returns true if RGBW LED, else false if RGB LED
    void setTiming(float high0, float low0, float high1, float low1, uint32_t lowReset);    // changes default timings for bit pulse - note parameters are in MICROSECONDS
        
    operator bool(){         // override boolean operator to return true/false if creation succeeded/failed
      return(channel>=0);
    }

    void on() {set(onColor);}
    void off() {set(RGB(0,0,0,0));}
    Pixel *setOnColor(Color c){onColor=c;return(this);}
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

      Color RGB(uint8_t r, uint8_t g, uint8_t b, uint8_t driveLevel=31){         // returns Color based on provided RGB values where r/g/b=[0-255] and current-limiting drive level=[0,31]       
        this->red=r;
        this->green=g;
        this->blue=b;
        this->drive=driveLevel;
        this->flags=7;
        return(*this);
      }

      Color HSV(float h, float s, float v, double drivePercent=100){             // returns Color based on provided HSV values where h=[0,360], s/v=[0,100], and current-limiting drive percent=[0,100]
        float r,g,b;
        LedPin::HSVtoRGB(h,s/100.0,v/100.0,&r,&g,&b);
        this->red=r*255;
        this->green=g*255;
        this->blue=b*255;
        this->drive=drivePercent*0.315;
        this->flags=7;           
        return(*this);
      }       

      bool operator==(const Color& color){
        return(val==color.val);
      }
      
      bool operator!=(const Color& color){
        return(val!=color.val);
      }

      Color operator+(const Color& color){
        Color newColor;
        newColor.blue=blue+color.blue;
        newColor.red=red+color.red;
        newColor.green=green+color.green;
        return(newColor);
      }

      Color& operator+=(const Color& color){
        red+=color.red;
        blue+=color.blue;
        green+=color.green;
        return(*this);
      }
            
      Color operator-(const Color& color){
        Color newColor;
        newColor.blue=blue-color.blue;
        newColor.red=red-color.red;
        newColor.green=green-color.green;
        return(newColor);
      }

      Color& operator-=(const Color& color){
        red-=color.red;
        blue-=color.blue;
        green-=color.green;
        return(*this);
      }
            
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
    void set(Color *c, int nPixels, boolean multiColor=true);                               // sets colors of nPixels based on array of Colors c; setting multiColor to false repeats Color in c[0] for all nPixels
    void set(Color c, int nPixels=1){set(&c,nPixels,false);}                                // sets color of nPixels to be equal to specific Color c
    
    static Color RGB(uint8_t r, uint8_t g, uint8_t b, uint8_t driveLevel=31){return(Color().RGB(r,g,b,driveLevel));}  // an alternative method for returning an RGB Color
    static Color HSV(float h, float s, float v, double drivePercent=100){return(Color().HSV(h,s,v,drivePercent));}    // an alternative method for returning an HSV Color
    
};

////////////////////////////////////////////
