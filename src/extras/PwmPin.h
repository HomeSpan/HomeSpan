
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                ----- PWM Pin Control -----
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Wrappers around the ESP-IDF ledc library to control PWM-based devices:
//
// LedPin(pin) - controls a Dimmable LED on specified pin with frequency=5000 Hz
//             - use set(level) to set brightness from 0-100%
//
// ServoPin(pin) - controls a Servo Motor on specified pin with frequency=50 Hz
//               - use set(degrees) to set position to degrees
//
// Max number of LedPin instantiations:     16
// Max number of ServoPin instantiatons:    8
// Max combined limit (LedPins+ServoPins):  16
//
// Instantiation of an LedPin or ServoPin that causes any of the maximums above to be exceeded throws
// an error message.  The object will still be created, but calls to set(level) or set(degrees) are ignored.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>
#include <driver/ledc.h>

/////////////////////////////////////

class LedPin {
  boolean enabled=false;
  ledc_channel_config_t ledChannel;

  public:
    LedPin(uint8_t pin, uint8_t level=0);                   // assigns pin to be output of one of 16 PWM channels within initial level
    void set(uint8_t level);                                // sets the PWM duty to level (0-100)
    int getPin(){return ledChannel.gpio_num;}               // returns the pin number
    
    static uint8_t numChannels;
    static void HSVtoRGB(float h, float s, float v, float *r, float *g, float *b );       // converts Hue/Saturation/Brightness to R/G/B
};

/////////////////////////////////////

class ServoPin {
  boolean enabled=false;
  uint16_t minMicros;
  uint16_t maxMicros;
  double minDegrees;
  double microsPerDegree;
  ledc_channel_config_t servoChannel;

  static const double micros2duty;

  public:
    ServoPin(uint8_t pin, double initDegrees, uint16_t minMicros, uint16_t maxMicros, double minDegrees, double maxDegrees);
    ServoPin(uint8_t pin, double initDegrees=0) : ServoPin(pin,initDegrees,1000,2000,-90,90) {};
    
    void set(double degrees);                               // sets the Servo to degrees, where degrees is bounded by [minDegrees,maxDegrees]  
    int getPin(){return servoChannel.gpio_num;}             // returns the pin number

    static uint8_t numChannels;
};

//*******************************************************
// DEPRECATED - INCLUDED FOR BACKWARDS COMPATIBILITY ONLY
//*******************************************************

class PwmPin {
  uint8_t channel;
  uint8_t pin;
  ledc_channel_config_t ledChannel;

  public:
    PwmPin(uint8_t channel, uint8_t pin);                   // assigns pin to be output of one of 16 PWM channels (0-15)
    void set(uint8_t channel, uint8_t level);               // sets the PWM duty to level (0-100)
    int getPin(){return pin;}                               // returns the pin number
    
    static void HSVtoRGB(float h, float s, float v, float *r, float *g, float *b );       // converts Hue/Saturation/Brightness to R/G/B
};
