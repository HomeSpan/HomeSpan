# HomeSpan Extras

HomeSpan includes integrated access to a number of ESP32 features you'll likely find particularly useful when constructing your HomeSpan devices.

## Pulse Width Modulation (PWM)

PWM on the ESP32 is more flexible, but slighly more complicated, than PWM on most Arduino devices (like the Uno or Mega).  On the ESP32, you use one of 16 built-in timer-channels to create a PWM signal, and then link that channel to any ESP32 pin.  HomeSpan includes a library that makes this very easy, and is accessed as by placing the following near the top of your sketch:

`#include "extras/PwmPin.h"`

### *PwmPin(uint8_t channel, uint8_t pin)*

Creating an instance of this **class** links one of 16 timer-channels to an ESP32 pin.

* *channel* - the ESP32 timer-channel number (0-15) to generate the PWM signal
* *pin* - the ESP32 pin that will output the PWM signal produced by the channel

The following methods are supported:

* `void set(uint8_t channel, uint8_t level)`

  * sets the PWM %duty-cycle of timer-channel *channel* (0-15) to *level*, where *level* ranges from 0 (off) to 100 (steady on)
  
* `int getPin()`

  * returns the pin number
  
PwmPin also includes a static class function that converts Hue/Saturation/Brightness values (typically used by HomeKit) to Red/Green/Blue values (typically used to control multi-color LEDS).

* `static void HSVtoRGB(float h, float s, float v, float *r, float *g, float *b)`

  * *h* - input Hue value, range 0-360
  * *s* - input Saturation value, range 0-1
  * *v* - input Brightness value, range 0-1
  * *r* - output Red value, range 0-1
  * *g* - output Green value, range 0-1
  * *b* - output Blue value, range 0-1

See tutorial sketch [#10 (RGB_LED)](../examples/10-RGB_LED) for an example of using PwmPin to control an RGB LED.

## Radio Frequency / Infrared Signal Generation

The ESP32 has an on-chip signal-generator peripheral designed to drive an RF or IR transmitter.  HomeSpan includes an easy-to-use library that interfaces with this peripheral so that with a few additional electronic components you can create a HomeSpan device that controls an RF or IR appliance directly from the Home App on your iPhone, or via Siri.  The library is accessed the following near the top of your sketch:

`#include "extras/RFControl.h"`

### *RFControl(int pin)*

Creating an instance of this **class** initializes the RF/IR signal generator and specifies the ESP32 *pin* to output the signal.  You may create more than one instance of this class if driving more than one RF/IR transmitter (each connected to different *pin*).

Signals are defined as a sequence of pulses, where you specify the duration, in *ticks*, of paired high/low periods of a each pulse in a pulse train, shown respectively as H1-H4 and L1-L4 in the diagram below.

![Pulse Train](images/pulseTrain.png)

Since most RF/IR signals repeat the same train of pulses more than once, the duration of the last LOW period should be extended to account for the delay between repeats of the pulse train.  The following methods are used to construct the pulse train, set the number of repeats, set the duration of a *tick*, and start the transmission:

* `static void add(uint16_t onTime, uint16_t offTime)`

  * appends a new pulse to the pulse train memory buffer, which has room to store a maximum of 511 high/low pulses.  Requests to add more than 511 pulses are ignores but raise a non-fatal warning message.  Note that this is a class-level method - there is only one pulse train memory buffer that is **shared** across all instances of an RFControl object
  
    * *onTime* - the duration, in *ticks* of the high portion of the pulse.  Allowable range is 0-32767 ticks.  Requests to add a pulse with an *onTime* of greater than 32767 ticks are ignored but raise non-fatal warning message

    * *offTime* - the duration, in *ticks* of the low portion of the pulse.  Allowable range is 0-32767 ticks.  Requests to add a pulse with an *offTime* of greater than 32767 ticks are ignored but raise non-fatal warning message
    
  * Note that a pulse with either a zero onTime *or* zero *offTime* is permitted, but both cannot be zero as this is used to indicate the end of the pulse train.
  
* `static void clear()`

  * clears the pulse train buffer

* `void start(uint8_t _numCycles, uint8_t tickTime)`

 * starts the transmission of the pulse train stored in the pulse train memory buffer.  Output will on the pin specified when RFControl was instantiated.  This is a blocking call, and the methods waits until transmission is completed before returning.  This should not provide a noticeable delay in program operations since most RF/IR pulse trains are only a few tens of milliseconds long
 
   * *numCycles* - the total number of times to transmit the pulse train (i.e. a value of 3 means pulse train will be transmitted once and then re-transmitted 2 more times)
   
   * *tickTime* - the duration, in **microseconds**, of a *tick*.  This is an optional arugment with a default of 1 microsecond if not specified.
 
---

[↩️](README.md) Back to the Welcome page
