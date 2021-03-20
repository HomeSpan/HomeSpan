
/////////////////////////////////////
//        PWM Pin Control          //
/////////////////////////////////////

// A wrapper around the ESP-IDF ledc library to easily set the brightness of an LED from 0-100%.
// Can be used for any device requiring a PWM output (not just an LED).  Frequency of PWM
// is hardcoded to 5000 Hz and either High-Speed Timer-0 (for channels 0-7) or Low-Speed Timer-0
// for channels (8-15) is configured and selected automatically.

#include <Arduino.h>
#include <driver/ledc.h>

class PwmPin {
  uint8_t channel;
  uint8_t pin;
  ledc_channel_config_t ledChannel;

  public:
    PwmPin(uint8_t channel, uint8_t pin);                   // assigns pin to be output of one of 16 PWM channels (0-15)
    void set(uint8_t level);                                // sets the PWM duty to level (0-100)
    void set(uint8_t channel, uint8_t level){set(level);}   // sets the PWM duty to level (0-100) - deprecated, but defined for backwards compatibility
    int getPin(){return pin;}                               // returns the pin number
    
    static void HSVtoRGB(float h, float s, float v, float *r, float *g, float *b );       // converts Hue/Saturation/Brightness to R/G/B

};

/////////////////////////////////////

class LedPin {
  uint8_t pin;
  boolean enabled=false;
  ledc_channel_config_t ledChannel;
  static uint8_t numChannels;

  public:
    LedPin(uint8_t pin, uint8_t level=0);                   // assigns pin to be output of one of 16 PWM channels within initial level
    void set(uint8_t level);                                // sets the PWM duty to level (0-100)
    int getPin(){return pin;}                               // returns the pin number
    
    static void HSVtoRGB(float h, float s, float v, float *r, float *g, float *b );       // converts Hue/Saturation/Brightness to R/G/B

};

/////////////////////////////////////

class ServoPin {
  uint8_t channel;                                          // channel must be in range [0,7] (only HighSpeed Channels will be used)
  uint8_t pin;
  uint16_t minMicros;
  uint16_t maxMicros;
  double minDegrees;
  double microsPerDegree;
  ledc_channel_config_t ledChannel;

  static const double micros2duty;

  public:
    ServoPin(uint8_t channel, uint8_t pin, double initDegrees, uint16_t minMicros, uint16_t maxMicros, double minDegrees, double maxDegrees);
    ServoPin(uint8_t channel, uint8_t pin, double initDegrees=0) : ServoPin(channel,pin,initDegrees,1000,2000,-90,90) {};
    
    void set(double degrees);                               // sets the Servo to degrees, where degrees is bounded by [minDegrees,maxDegrees]  
    int getPin(){return pin;}                               // returns the pin number

};
