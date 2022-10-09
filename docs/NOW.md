# SpanPoint: Point-to-Point Communication between ESP32 Devices

SpanPoint is HomeSpan's easy-to-use implementation of Espressif's ESP-NOW protocol.  ESP-NOW provides bi-directional, point-to-point communication between ESP32 devices without the need for a central WiFi network.

In a typical setup, a "Main" ESP32 device runs a complete HomeSpan sketch, whereas one or more "Remote" ESP32 devices run simple sketches designed to take measurements (temperature, humidity, etc.).  The Main device is paired to HomeKit and communicates with the Home App over your WiFi network in the usual fashion.  The Remote devices do not connect to your WiFi network or to HomeKit, but instead send their data directly to the Main device using an encrypted ESP-NOW channel.  The Main device is then responsible for reading this data and determining what, if any, actions to take or updates to send to HomeKit.

Most importantly, because ESP-NOW does not require always-on WiFi connectivity, it draws very little power.  This means you can operate Remote devices with a battery instead of wall-power, which makes them much easier to place in remote locations (such as outdoors).

## *SpanPoint(const char \*macAddress, int sendSize, int receiveSize, int queueDepth=1)*

Creating an instance of this **class** initializes an ESP-NOW connection from the current device to a device with the specified *macAddress*. Arguments, along with their defaults if left unspecified, are as follows:

  * *macAddress* - the MAC Address of the device to which you are connecting, in the form "XX:XX:XX:XX:XX:XX" the pin on which the PWM control signal will be output
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

---

[↩️](README.md) Back to the Welcome page
