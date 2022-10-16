# SpanPoint: Point-to-Point Communication between ESP32 Devices

SpanPoint is HomeSpan's easy-to-use implementation of the Espressif ESP-NOW protocol.  SpanPoint provides bi-directional, point-to-point communication of small, fixed-size messages directly between ESP32 devices based on their MAC Addresses without the need for a central WiFi network.

To establish connectivity between any two devices simply instantiate a SpanPoint object on each device that references the MAC Address of the other device, as well as specifies the (potentially different) sizes of the messages that each device is expected to send to, and receive from, the other.

SpanPoint creates all the internal data queues needed to manage message flow, configures ESP-NOW to encrypt all message traffic, and auto-sets the WiFi channel used by ESP-NOW for transmission to match whatever is needed by any devices that are also connected to HomeKit through your central WiFi network.

SpanPoint is part of the main HomeSpan library and is accessible by adding `#include "HomeSpan.h"` near the top of your sketch.  Detailed descriptions of the SpanPoint class and all of its methods are provided below.

## *SpanPoint(const char \*macAddress, int sendSize, int receiveSize, int queueDepth=1)*

Creating an instance of this **class** enables the device to send messages to, and/or receive messages from, a "complementary" instance of *SpanPoint* on another ESP32 device.  Arguments, along with their defaults if left unspecified, are as follows:

  * *macAddress* - the MAC Address of the *other* device to which you want to send data to, and/or receive data from, in the standard 6-byte format "XX:XX:XX:XX:XX:XX", where each XX represents a single 2-digit hexidecimal byte from 00 to FF
  * *sendSize* - the size, in bytes, of any messages that will be sent from this device to the *other* device. Allowed range is 0 to 200, where a value of 0 is used to indicate to SpanPoint that you will **not** be using `send()` to transmit any messages from this device to the *other* device
  * *receiveSize* - the size, in bytes, of any messages that will be received by this device from the *other* device.  Allowed range is 0 to 200, where a value of 0 is used to indicate to SpanPoint that you will **not** be using `get()` to retreive any messages transmitted by the *other* device to this device
  * *queueDepth* - the depth of the queue reserved to hold messages of *receiveSize* bytes that were received by this device from the *other* device, but not yet retreived using `get()`. Default=1 if left unspecified, which should be sufficient for most applications.  See `get()` below for further details. 

> SpanPoint objects created on two separate devices are considered "complementary" if the MAC Addresses specified in each SpanPoint object references each other's devices, and the *sendSize* and *receiveSize* of the SpanPoint object on one device matches, respectively, the *receiveSize* and *sendSize* of the SpanPoint object on the *other* device, with the exception that it is always okay to set either the *sendSize* or *receiveSize* to zero regardless of the value set on the *other* device.

SpanPoint will throw a fatal error during instantiation and halt the sketch if:
  * the *macAddress* specified is ill-formed, or
  * either *sendSize* or *receiveSize* is set to greater than 200, or
  * both *sendSize* and *receiveSize* are set to 0, since there is no purpose for a SpanPoint that will neither transmit nor receive data
   
**The following SpanPoint methods are used to transmit and receive messages from a SpanPoint object on one device to a corresponding SpanPoint object on the *other* device:**

* `boolean send(const void *data)`

  * transmits a message using data pointed to by *data* (which may be a standard data type, such as *uint16_t*, or a user-defined *struct*) to the *other* device
  * the size of the *data* element to be transmitted much match the *sendSize* parameter specified when the SpanPoint object was created
  * returns **true** if transmission was successful, otherwise **false** if transmission failed.  Note that a transmission is considered successful as long as the device can find and connect to the *other* device based on its MAC Address, regardless of whether or not the other device has a corresponding SpanPoint object
  
* `boolean get(void *dataBuf)`

  * checks to see if a message has been received into SpanPoint's internal message queue from the *other* device
  * if no message is available, the method returns **false** and *dataBuf* is unmodified
  * if a message is available, it is **moved** from the internal message queue into *dataBuf* and the method returns **true**
  * the size of the message will always be equal to the *receiveSize* parameter specified when the SpanPoint object was created, so make sure *dataBuf* is sufficiently sized to store such a message

Note that whether or or not you call the `get()` method, SpanPoint is configured to store (in an internal queue) any SpanPoint messages it receives from *other* devices, provided that (a) there is room in the internal queue, and (b) the size of the message received matches the *receiveSize* parameter specified when the relevant SpanPoint object was instantiated.  If the internal queue is full when a message is received, the message is *not* moved to the queue and is instead discarded before it can ever be retreived by the `get()` method.  To avoid this, make sure you call `get()` more frequently than you expect to receive messages, or set the *queueDepth* parameter to something greater than 1 when instantating the SpanPoint object.

Also note that regardless of whether or not the queue if full, if the size of a received message does not match the *receiveSize* parameter specified for this instance of the SpanPoint object, the message will be discarded.  If *receiveSize* is greater than zero, a non-fatal run-time warning about size mismatches will also be output on the Serial Monitor.

See tutorial sketch [#10 (RGB_LED)](../examples/10-RGB_LED) for an example of using LedPin to control an RGB LED.

---

[↩️](README.md) Back to the Welcome page
