# SpanPoint: Point-to-Point Communication between ESP32 Devices

SpanPoint is HomeSpan's integrated implementation of Espressif's ESP-NOW protocol that enables bi-directional communication from one ESP32 device directly to another without the need for a central WiFi network.  In a typical setup, one ESP32 device runs a full HomeSpan sketcs (perhaps for a Thermostat Accessory) and connects to HomeKit in the usual fashion.  


ike most commercial HomeKit devices, HomeSpan requires a power-consuming always-on WiFi connection, which unfortunately means powering a HomeKit device with batteries is generally not possible.  For most applications, this is not a problem since your HomeSpan device will be controlling a real-world light, fan, thermostat, etc., and will likely be plugged into a wall outlet.  However, there are some real-world applications where wall outlets are not readily available and battery-power is essential, such as remote temperature sensors, door and window sensors, or standalone switches.

To address this problem, HomeSpan includes an integrated implementation of Espressif's ESP-NOW protocol that allows for the point-to-point transmission of short messages between ESP32 devices requiring **very little power**.  Importantly, ESP-NOW uses the ESP32's existing WiFi radio, so no new hardware is required.

What all this means is you can readily create a multi-device Accessory where:

* HomeSpan, running on an ESP32 plugged into wall-power, provides the required always-on connectivity to HomeKit via your home WiFi network, and
* One or more remote devices, running on battery-powered ESP32's, monitor their local environment (e.g. temperature, humidity) and communicate this information back to the main HomeSpan device via the low-powered ESP-NOW protocol.
* The main HomeSpan device processes this information and updates HomeKit and the Home App as needed, just as if were taking its own temperature and humidity measurements directly.

All of the required logic needed to implement point-to-point communication between multiple ESP32 devices is embedded in HomeSpan's easy-to-use SpanPoint class, fully described below. 

## *LedPin(uint8_t pin [,float level [,uint16_t frequency]])*

Creating an instance of this **class** configures the specified *pin* to output a PWM signal suitable for a controlling dimmable LED.  Arguments, along with their defaults if left unspecified, are as follows:

  * *pin* - the pin on which the PWM control signal will be output
  * *level* - sets the initial %duty-cycle of the PWM from from 0 (LED completely off) to 100 (LED fully on).  Default=0 (LED initially off)
  * *frequency* - sets the PWM frequency, in Hz, from 1-65535 (ESP32 only) or 5-65535 (ESP32-S2 and ESP32-C3).  Defaults to 5000 Hz if unspecified, or if set to 0
 
 The following methods are supported:

* `void set(float level)`

  * sets the PWM %duty-cycle to *level*, where *level* ranges from 0 (LED completely off) to 100 (LED fully on)
  
* `int getPin()`

  * returns the pin number (or -1 if LedPin was not successfully initialized)
  
LedPin also includes a static class function that converts Hue/Saturation/Brightness values (typically used by HomeKit) to Red/Green/Blue values (typically used to control multi-color LEDS).

* `static void HSVtoRGB(float h, float s, float v, float *r, float *g, float *b)`

  * *h* - input Hue value, range 0-360
  * *s* - input Saturation value, range 0-1
  * *v* - input Brightness value, range 0-1
  * *r* - output Red value, range 0-1
  * *g* - output Green value, range 0-1
  * *b* - output Blue value, range 0-1

See tutorial sketch [#10 (RGB_LED)](../examples/10-RGB_LED) for an example of using LedPin to control an RGB LED.

## *ServoPin(uint8_t pin [,double initDegrees [,uint16_t minMicros, uint16_t maxMicros, double minDegrees, double maxDegrees]])*

Creating an instance of this **class** configures the specified *pin* to output a 50 Hz PWM signal, which is suitable for controlling most Servo Motors.  There are three forms of the constructor: one with just a single argument; one with two arguments; and one with all six arguments.  Arguments, along with their defaults if left unspecified, are as follows:

  * *pin* - the pin on which the PWM control signal will be output.  The control wire of a Servo Motor should be connected this pin
  * *initDegrees* - the initial position (in degrees) to which the Servo Motor should be set (default=0°)
  * *minMicros* - the pulse width (in microseconds) that moves the Servo Motor to its "minimium" position of *minDegrees* (default=1000𝛍s)
  * *maxMicros* - the pulse width (in microseconds) that moves the Servo Motor to its "maximum" position of *maxDegrees* (default=2000𝛍s)
  * *minDegrees* - the position (in degrees) to which the Servo Motor moves when receiving a pulse width of *minMicros* (default=-90°)
  * *maxDegrees* - the position (in degrees) to which the Servo Motor moves when receiving a pulse width of *maxMicros* (default=90°)

The *minMicros* parameter must be less than the *maxMicros* parameter, but setting *minDegrees* to a value greater than *maxDegrees* is allowed and can be used to reverse the minimum and maximum positions of the Servo Motor. The following methods are supported:

* `void set(double position)`

  * sets the position of the Servo Motor to *position* (in degrees).  In order to protect the Servo Motor, values of *position* less than *minDegrees* are automatically reset to *minDegrees*, and values greater than *maxDegrees* are automatically reset to *maxDegrees*.
  
* `int getPin()`

  * returns the pin number (or -1 if ServoPin was not successfully initialized)

A worked example showing how ServoPin can be used to control the Horizontal Tilt of a motorized Window Shade can be found in the Arduino IDE under [*File → Examples → HomeSpan → Other Examples → ServoControl*](../Other%20Examples/ServoControl).

### PWM Resource Allocation and Limitations

The following PWM resources are available:

* ESP32: 16 Channels / 8 Timers (arranged in two distinct sets of 8 Channels and 4 Timers)
* ESP32-S2: 8 Channels / 4 Timers
* ESP32-C3: 6 Channels / 4 Timers
* ESP32-S3: 8 Channels / 4 Timers

HomeSpan *automatically* allocates Channels and Timers to LedPin and ServoPin objects as they are instantiated. Every pin assigned consumes a single Channel;  every *unique* frequency specified among all channels (within the same set, for the ESP32) consumes a single Timer.  HomeSpan will conserve resources by re-using the same Timer for all Channels operating at the same frequency.  *HomeSpan also automatically configures each Timer to support the maximum duty-resolution possible for the frequency specified.*

HomeSpan will report a non-fatal error message to the Arduino Serial Monitor when insufficient Channel or Timer resources prevent the creation of a new LedPin or ServoPin object.  Calls to the `set()` method for objects that failed to be properly created are silently ignored.

---

[↩️](README.md) Back to the Welcome page
