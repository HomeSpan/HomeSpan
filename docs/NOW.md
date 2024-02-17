# SpanPoint: Point-to-Point Communication between ESP32 Devices

SpanPoint is HomeSpan's easy-to-use implementation of the Espressif ESP-NOW protocol.  SpanPoint provides bi-directional, point-to-point communication of small, fixed-size messages directly between ESP32 devices based on their MAC Addresses without the need for a central WiFi network.  SpanPoint can also be used on an ESP-8266 when configured as a remote device.

To establish connectivity between any two devices simply instantiate a SpanPoint object on each device that references the MAC Address of the other device, as well as specifies the (potentially different) sizes of the messages that each device is expected to send to, and receive from, the other.

SpanPoint creates all the internal data queues needed to manage message flow, configures ESP-NOW to encrypt all message traffic, and auto-sets the WiFi channel used by ESP-NOW for transmission to match whatever is needed by any devices that are also connected to HomeKit through your central WiFi network.

SpanPoint is part of the main HomeSpan library and is accessible by adding `#include "HomeSpan.h"` near the top of your sketch.  Detailed descriptions of the SpanPoint class and all of its methods are provided below.

## *SpanPoint(const char \*macAddress, int sendSize, int receiveSize [, int queueDepth=1 [, boolean useAPaddress=false]])*

Creating an instance of this **class** enables the device to send messages to, and/or receive messages from, a "complementary" instance of *SpanPoint* on another ESP32 device.  Arguments, along with their defaults if left unspecified, are as follows:

  * *macAddress* - the MAC Address of the *other* device to which you want to send data to, and/or receive data from, in the standard 6-byte format "XX:XX:XX:XX:XX:XX", where each XX represents a single 2-digit hexidecimal byte from 00 to FF
  * *sendSize* - the size, in bytes, of any messages that will be sent from this device to the *other* device. Allowed range is 0 to 200, where a value of 0 is used to indicate to SpanPoint that you will **not** be using `send()` to transmit any messages from this device to the *other* device
  * *receiveSize* - the size, in bytes, of any messages that will be received by this device from the *other* device.  Allowed range is 0 to 200, where a value of 0 is used to indicate to SpanPoint that you will **not** be using `get()` to retreive any messages transmitted by the *other* device to this device
  * *queueDepth* - the depth of the queue reserved to hold messages of *receiveSize* bytes that were received by this device from the *other* device, but not yet retreived using `get()`. Default=1 if left unspecified, which should be sufficient for most applications.  See `get()` below for further details
  * *useAPaddress* - SpanPoint normally communicates via the ESP32's WiFi Station (STA) Interface using the STA MAC Address.  Setting *useAPaddress* to *true* causes SpanPoint to instead communicate via the ESP32's WiFi Access Point (AP) Interface using the AP MAC Address.  This is needed when using an ESP-8266 as a Remote Device (see below).  Default=*false* if left unspecified

A list of all SpanPoint objects instantiated in a sketch, their parameters as specified above, and the specific MAC Address each Remote Device should use to connect back to the Main HomeSpan Device, is displayed in the Serial Monitor by typing 'i' into the CLI

> SpanPoint objects created on two separate devices are considered "complementary" if the MAC Addresses specified in each SpanPoint object references each other's devices, and the *sendSize* and *receiveSize* of the SpanPoint object on one device matches, respectively, the *receiveSize* and *sendSize* of the SpanPoint object on the *other* device, with the exception that it is always okay to set either the *sendSize* or *receiveSize* to zero regardless of the value set on the *other* device

SpanPoint will throw a fatal error during instantiation and halt the sketch if:
  * the *macAddress* specified is ill-formed, or
  * either *sendSize* or *receiveSize* is set to greater than 200, or
  * both *sendSize* and *receiveSize* are set to 0, since there is no purpose for a SpanPoint that will neither transmit nor receive data
   
**The following SpanPoint methods are used to transmit and receive messages from a SpanPoint object on one device to a complementary SpanPoint object on the *other* device:**

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

**Other methods supported by SpanPoint are as follows:**

* `uint32_t time()`

  * returns the time elapsed (in millis) since a SpanPoint object last received a valid message
  * valid messages are those that can be properly decrypted and whose size matches the *receiveSize* parameter, regardless of whether or not there is room in the queue to store the message
  * reading a message in the queue with `get()` has no impact on the elapsed time calculation
  * this method is typically used to check whether messages from a transmitting device are overdue (suggesting a potential problem with that device)

* `static void setPassword(const char *pwd)`

  * this *optional* **class-level** method changes the default passphrase used to generate ESP-NOW encryption keys for all SpanPoint objects from the default passphrase ("HomeSpan") to *pwd*, which can be a character string of any length
  * if used, this method must be called *before* the instantiation of any SpanPoint objects.  Example: `SpanPoint::setPassword("MyPassword");`
  * the same passphrase must be used among all devices that are communicating via SpanPoint, else the receiving device will not be able to decrypt messages it receives

* `static void setEncryption(boolean encrypt)`

  * this *optional* **class-level** method provides the ability to enable or disable encryption according to whether *encrypt* is set to *true* or *false*
  * by default, encryption is normally enabled (using the password above)
  * if used, this method must be called *before* the instantiation of any SpanPoint objects.  Example: `SpanPoint::setEncryption(false);` disables encryption for all SpanPoint connections
  * note that this is a global setting - if SpanPoint encryption is disabled on the main device, it must also be disabled on every remote device, else communication between devices will fail
  * enabling/disabling encryption impacts that total number of SpanPoint connections that can be supported by the ESP32's ESP-NOW functionality:
    * with encryption enabled, the ESP32 can support a maximum of 7 ESP-NOW links (i.e. 7 instances of SpanPoint)
    * with encryption disabled, the ESP32 can support a maximum of 20 ESP-NOW links (i.e. 20 instances of SpanPoint)

* `static void setChannelMask(uint16_t mask)`

  * this *optional* **class-level** method changes the default channel bitmask from 0x3FFE (i.e. 0011 1111 1111 1110) to *mask*
  * the channel bitmask is used to limit which of the standard channels (1-13) supported by the ESP32 WiFi radio should be tried whenever SpanPoint needs to reset the ESP-NOW channel after a transmission failure
  * setting bit number *N* to 1 in the bitmask, where N=[1,13], enables the use of WiFi channel number *N*
  * setting bit number *N* to 0 in the bitmask, where N=[1,13], disables the use of WiFi channel number *N*
  * example: `SpanPoint::setChannelMask(1<<1 | 1<<6 | 1<<11);` causes SpanPoint to try only WiFi channels 1, 6, and 11 when transmitting messages
  * this method will throw a fatal error and halt the sketch if called with a *mask* that does not enable at least one channel
  * this method has no effect on SpanPoint if used within a full HomeSpan sketch that connects to HomeKit via a central WiFi network, since under these conditions the WiFi channel must remain set to whatever the central WiFi network requires

## Typical Use Cases

One of the primary reasons for using SpanPoint is to enable the deployement of battery-powered devices.  Since HomeKit requires an always-on WiFi connection, wall-power is a must.  But ESP-NOW does not require always-on connectivity to a central WiFi network, which makes it possible to power things like remote-sensor devices with just a battery.  Such battery-powered "Remote Devices" can take periodic local measurements and transmit them via SpanPoint messages to a wall-powered "Main Device" that is running a full HomeSpan sketch connected to HomeKit via a central WiFi network.

Examples showing such a configuration can be found in the Arduino IDE under [*File → Examples → HomeSpan → Other Examples → RemoteSensors*](../examples/Other%20Examples/RemoteSensors).  This folder contains the following sketches:

* *MainDevice.ino* - a full HomeSpan sketch that implements two Temperature Sensor Accessories, but instead of taking its own temperature measurements, it uses SpanPoint to read messages containing temperature updates from other Remote Devices
* *RemoteDevice.ino* - a lightweight sketch that simulates taking periodic temperature measurements, which are then transmitted to the Main Device via SpanPoint
* *RemoteTempSensor.ino* - a lightweight sketch that is similar to *RemoteDevice.ino*, except that instead of simulating a temperature sensor, it implements an actual Adafruit ADT7410 I2C-based temperature sensor.  This sketch also uses some power-management techniques to extend battery life, such as lowering the CPU frequency and entering into deep-sleep after each measurement is taken
* *RemoteDevice8266.ino* - similar in function to *RemoteDevice.ino*, but implemented to run on an ESP8266 device using native ESP-NOW commands (since neither HomeSpan nor SpanPoint support the ESP8266).  Note that the "complementary" SpanPoint object on the ESP32 that receives data from the ESP8266 must be configured to use the ESP32's *AP MAC Address* (instead of the *STA MAC Address*) by setting *useAPaddress* to *true* in the SpanPoint constructor
---

[↩️](../README.md) Back to the Welcome page
