# SpanPoint: Point-to-Point Communication between ESP32 Devices

SpanPoint is HomeSpan's easy-to-use implementation of Espressif's ESP-NOW protocol.  ESP-NOW provides bi-directional, point-to-point communication between ESP32 devices without the need for a central WiFi network.

In a typical "Remote Sensor" setup, a "Main" ESP32 device runs a complete HomeSpan sketch, whereas one or more "Remote" ESP32 devices run simple sketches designed to take measurements (temperature, humidity, etc.).  The Main device is paired to HomeKit and communicates with the Home App over your WiFi network in the usual fashion.  In contrast, the Remote devices do not connect to your WiFi network or to HomeKit, but instead send their data directly to the Main device using an encrypted ESP-NOW channel.  The Main device is then responsible for reading this data and determining what, if any, actions to take or updates to send to HomeKit.

You are not limited to the above scenario.  SpanPoint can be used to communicate among devices that are each running a HomeSpan sketch, as well as among devices that are not running a HomeSpan sketch.  As long as your sketch contains `#include "HomeSpan.h"`, you can use SpanPoint.

Perhaps most importantly, because ESP-NOW does not require always-on WiFi connectivity, it draws very little power.  This means you can operate Remote devices with a battery instead of wall-power, which opens up many possibilities, such as installing temperature sensors in remote (outdoor) locations where wall-power may be unavailable.

## *SpanPoint(const char \*macAddress, int sendSize, int receiveSize, int queueDepth=1)*

Creating an instance of this **class** initializes an ESP-NOW connection from the current device to a device with the specified *macAddress*. Arguments, along with their defaults if left unspecified, are as follows:

  * *macAddress* - the MAC Address of the device to which you are connecting, in the standard 6-byte form "XX:XX:XX:XX:XX:XX", where XX represents a single hexidecimal byte
  * *sendSize* - the size, in bytes, of any messages to be sent from this device over this instance.  Set to zero if not sending messages from this device.  Maximum allowed size if 200 bytes.
  * *receiveSize* - the size, in bytes, of any messages to be received by this device over this instance.  Set to zero if not receiving messages on this device.  Maximum allowed size if 200 bytes.
 
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
