# SpanPoint: Point-to-Point Communication between ESP32 Devices

SpanPoint is HomeSpan's easy-to-use implementation of Espressif's ESP-NOW protocol.  ESP-NOW provides bi-directional, point-to-point communication between ESP32 devices without the need for a central WiFi network.

In a typical "Remote Sensor" setup, a "Main" ESP32 device runs a complete HomeSpan sketch, whereas one or more "Remote" ESP32 devices run simple sketches designed to take measurements (temperature, humidity, etc.).  The Main device is paired to HomeKit and communicates with the Home App over your WiFi network in the usual fashion.  In contrast, the Remote devices do not connect to your WiFi network or to HomeKit, but instead send their data directly to the Main device using an encrypted ESP-NOW channel.  The Main device is then responsible for reading this data and determining what, if any, actions to take or updates to send to HomeKit.

Note that since ESP-NOW does not require always-on WiFi connectivity, it draws very little power.  This means you can operate Remote devices with a battery instead of wall-power, which opens up many possibilities, such as installing temperature sensors in remote (outdoor) locations where wall-power may be unavailable.

You are of course not limited to the above Main/Remote scenario.  SpanPoint can be used to send messages back and forth among any combination of ESP32 devices, regardless if none or them, some of them, or all of them, are running a complete HomeSpan sketch.  

SpanPoint is part of the main HomeSpan library, so all you need to do is place `#include "HomeSpan.h"` near the top of your sketch to access the *SpanPoint* class and all of its methods.

## *SpanPoint(const char \*macAddress, int sendSize, int receiveSize, int queueDepth=1)*

Creating an instance of this **class** enables the device to send messages to, and/or receive messages from, a "matching" instance of *SpanPoint* on another ESP32 device.  Arguments, along with their defaults if left unspecified, are as follows:

  * *macAddress* - the MAC Address of the other device to which you want to send data to, and/or receive data from, in the standard 6-byte format "XX:XX:XX:XX:XX:XX", where each XX represents a single 2-digit hexidecimal byte from 00 to FF
  * *sendSize* - the size, in bytes, of any message that will be sent from this device to the device with MAC Address specified above.  Maximum allowed size = 200 bytes.  Set to zero if you will *not* be sending any messages from this device over this instance of *SpanPoint*
  * *receiveSize* - the size, in bytes, of any message that will be received by this device from the device with MAC Address specified above.  Maximum allowed size = 200 bytes.  Set to zero if you do not intend to be reading any messages from the other device over this instance of *SpanPoint* (in which case any messages the other device happens to transmit to this device will be ignored)
  * *queueDepth* - the depth of the queue reserved to hold incoming messages of *receiveSize* bytes.  Default=1 is left unspecified, which should be sufficient for most applications.  Set to greater than 1 if you expect to receive incoming messages from the other device at a rate that is faster than can be read and processed by the current device (a very unlikely scenario)
 
For example, if:

  * **Device A** (MAC Address=*A1:A2:A3:A4:A5:A6*) is going to send 4-byte messages to both **Device B** (MAC Address=*B1:B2:B3:B4:B5:B6*) and **Device C** (MAC Addresses=*C1:C2:C3:C4:C5:C6*), and 
  * **Device B** is going to send 128-byte messages to **Device A**, and
  * **Device C** is going to send 91-byte messages to **Device A**,
  
then the sketch for **Device A** would contain something like this:

```C++
#include "HomeSpan.h"

SpanPoint *devB, *devC;

void setup(){
 devB = new SpanPoint("B1:B2:B3:B4:B5:B6",4,128);  // create SpanPoint to device B with sendSize=4 and receiveSize=128
 devC = new SpanPoint("C1:C2:C3:C4:C5:C6",4,91);   // create SpanPoint to device C with sendSize=4 and receiveSize=91
 ...
}
```

the sketch for **Device B** would contain something like this:
 
```C++
#include "HomeSpan.h"

SpanPoint *devA;

void setup(){
 devA = new SpanPoint("A1:A2:A3:A4:A5:A6",128,4);  // create SpanPoint to device A with sendSize=128 and receiveSize=4
 ...
}
```

and the sketch for **Device C** would contain something like this:
 
```C++
#include "HomeSpan.h"

SpanPoint *devA;

void setup(){
 devA = new SpanPoint("A1:A2:A3:A4:A5:A6",91,4);  // create SpanPoint to device A with sendSize=91 and receiveSize=4
 ...
}
```

Once instances of *SpanPoint* are created, the following methods can be used to send and receive messages:

* `boolean send(void *data)`

  * transmits data referenced by the pointer *data* to the other device
  * returns **true** if transmission was successful, otherwise **false** if transmission failed.  Note that this method will return **true** as long as it can find and connect to the device with the *macAddress* specified when the relevant SpanPoint object was instantiated, regardless of whether or not the other device successful reads the message
  * example (using the above scenario):  `float humidity=35.8; devB->send(&humidity);` sends a 4-byte floating number to **Device B**.  Note that the pointer *data* should always reference a data element or structure whose size exactly matches the *sendSize* parameter specified when the relevant SpanPoint object was instantiated
  
* `boolean get(void *dataBuf)`

  * checks to see if a message has been received into SpanPoint's internal queue from a device with the *macAddress* specified when the relevant SpanPoint object was instantiated.  If so, the message is copied into the memory pointed to by *dataBuf*, the internal queue is cleared, and the method returns **true**.  If no message is available, the *dataBuf* memory is unmodified and the method returns **false**
  * note that the pointer *dataBuf* should always reference a data element or structure whose size equals or exceeds the *receiveSize* parameter specified when the relevant SpanPoint object was instantiated
  * once the internal SpanPoint queue is full, any messages received from the same device are discarded until the queue is cleared by calling this *get()* method.  Setting the optional *queueDepth* parameter to a number greater than one when the relevant SpanPoint object was instantiated allows SpanPoint to retain up to *queueDepth* messages even without a call to the this *get()* method.  However, once the queue is full, additional messages received will be discarded until messages are "gotten" of the queue with the *get()* method
 
 
 
See tutorial sketch [#10 (RGB_LED)](../examples/10-RGB_LED) for an example of using LedPin to control an RGB LED.

---

[↩️](README.md) Back to the Welcome page
