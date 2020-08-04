
/////////////////////////////////////
//        PWM Pin Control          //
/////////////////////////////////////

// A wrapper around the ESP-IDF ledc library to easily set the brightness of an LED from 0-100%.
// Can be used for any device requiring a PWM output (not just an LED).  Frequency of PWM
// is hardcoded to 5000 Hz and either High-Speed Timer-0 (for channels 0-7) or Low-Speed Timer-0
// for channels (8-15) is configured and selected automatically.

#include <driver/ledc.h>

class PwmPin {
  uint8_t channel;
  uint8_t pin;
  ledc_channel_config_t ledChannel;

  public:
    PwmPin(uint8_t channel, uint8_t pin);               // assigns pin to be output of one of 16 PWM channels (0-15)
    void set(uint8_t channel, uint8_t level);           // sets the PWM duty of channel to level (0-100)
    int getPin(){return pin;}                           // returns the pin number
    
    static void HSVtoRGB(float h, float s, float v, float *r, float *g, float *b );

};
