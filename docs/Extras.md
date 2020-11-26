# HomeSpan Extras

HomeSpan includes integrated access to a number of ESP32 features you'll likely find particularly useful when constructing your HomeSpan devices.

## Pulse Width Modulation (PWM)

PWM on the ESP32 is more flexible, but slighly more complicated, than PWM on most Arduino devices (like the Uno or Mega).  On the ESP32, you use one of 16 built-in timer-channels to create a PWM signal, and then link that channel to any ESP32 pin.  HomeSpan includes a library that makes this very easy, and is accessed as follows:

`#include "extras/PwmPin.h"`

### *PwmPin(uint8_t channel, uint8_t pin)*

Creating an instance of this class links one of 16 timer-channels to an ESP32 pin.

* *channel* - an ESP32 timer-channel number (0-15) which will generate the PWM signal
* *pin* - the ESP32 pin that will output the PWM signal produced by the channel

The following methods are supported:

* `void set(uint8_t channel, uint8_t level)`
  * sets that PWM %duty-cycle of timer-channel *channel* to *level*, where *level* ranges from 0 (off) to 100 (steady on)

---

[↩️](README.md) Back to the Welcome page
