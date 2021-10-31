# Frequently Asked Questions

*A list of answers to frequently-asked-questions, as well as discussions of various topics of interest.*

#### How do I set my WiFi SSID and Password?

* In the spirit of promoting good security practices, HomeSpan does not allow you to embed your WiFi SSID and Password (collectively known as WiFi Credentials) into your sketch.  This eliminates the possibility of accidentally disclosing this information should you share your sketch with others.

* Instead, HomeSpan stores your WiFi Credentials in a dedicated non-volatile storage (NVS) partition of the ESP32.  There are two ways of entering this information.  If you are developing a sketch within the Arduino IDE, simply use the HomeSpan Command Line Interface and type 'W' into the Serial Monitor.  HomeSpan will prompt you to input your WiFi SSID and Password (see [CLI](CLI.md) for details).  As an alternative, if your HomeSpan device is not connected to a computer, you can launch HomeSpan's Temporary WiFi Setup Network and input your WiFi Credentials directly into the web forms served by your device (see the [User Guide](UserGuide.md#setting-homespans-wifi-credentials-and-setup-code) for details).

#### Does HomeSpan support video and audio streams?

* No, HomeSpan does not support streaming services, such as Audio Stream Management, Data Stream Transport Management, or Camera RTP Management.  See [HomeSpan Services and Characteristics](ServiceList.md) for a complete list of all Services supported by HomeSpan.

#### Does HomeSpan work with PlatformIO?

* HomeSpan does not explicitly support PlatformIO but multiple users have reported successfully using the HomeSpan library with PlatformIO.

#### Will HomeSpan work on an ESP8266 device?

* No, HomeSpan is coded specifically for the ESP32 and will not operate on an ESP8266 device.

#### Will HomeSpan work on an ESP32-S2 or ESP32-C3?

* Yes!  Starting with version 1.4.0, HomeSpan is fully compatible with Espressif's ESP32-S2 and ESP32-C3 chips, as well as the original ESP32 chip. Note that to select an ESP32-S2 or ESP32-C3 device from the Arduino IDE you'll need to install Version 2 of the [Arduino-ESP32 Board Manager](https://github.com/espressif/arduino-esp32).

#### How can I read HomeSpan's MDNS broadcast mentioned in the [OTA](OTA.md) documentation?

* HomeSpan uses MDNS (multicast DNS) to broadcast a variety of HAP information used by Controllers wishing to pair with HomeSpan.  Apple uses the name *Bonjour* to refer to MDNS, and originally included a Bonjour "Browser" in Safari that has since been discontinued.  However, there are a number of alternative MDNS browsers available for free that operate on both the Mac and the iPhone, such as the [Discovery - DNS-SD Browser](https://apps.apple.com/us/app/discovery-dns-sd-browser/id1381004916?mt=12).  You'll find all your HomeSpan devices, as well as any other HomeKit devices you may have, under the MDNS service named *_hap._tcp.*  The fields broadcast by HomeSpan are a combination of all data elements requires by HAP (HAP-R2, Table 6-7) plus three additional HomeSpan fields:

  * *hspn* - the version number of the HomeSpan library used to compile the sketch
  * *sketch* - the version number of the sketch, as specified with `homeSpan.setSketchVersion(const char *)`, or *n/a* if no version was specified
  * *ota* - either *yes* or *no* depending on whether OTA was enabled for the sketch using the method `homeSpan.enableOTA()` 

#### Does HomeSpan support Television Services?

* Yes.  Though undocumented by Apple and not officially part of HAP-R2, HomeSpan supports HomeKit Television controls.  See [Television Services](../docs/TVServices.md) for details.

#### Can you use HomeSpan with an Ethernet connection instead of a WiFi connection?

* Not as present.  Though with a compatible Ethernet board the ESP32 can be configured to run as an Ethernet Server, using MDNS over Ethernet does not work on the ESP32 due to some apparent problems with the Ethernet UDP stack.  Unfortunately, HomeSpan and HAP-R2 require MDNS to operate.  If anyone has managed to get an Ethernet version of MDNS working on an ESP32 please let me know - it would be great to add Ethernet support to HomeSpan.

#### Does HomeSpan work with SPI and I2C?

* Yes, the standard Arduino libraries `SPI.h` and `Wire.h` both work well within the HomeSpan environment.  Typically the code to read from an SPI or I2C device is implemented within the `loop()` method of a HomeSpan Service with any initialization being done in the constructor for that Service.  See [TemperatureSensorI2C](https://github.com/HomeSpan/TempSensorI2C) for an illustrative example.

#### Can you add a Web Server to HomeSpan?

* Yes, provided you implement your Web Server using standard ESP32-Arduino libraries, such as `WebServer.h`. See [ProgrammableHub](https://github.com/HomeSpan/ProgrammableHub) for an illustrative example of how to easily integrate a Web Server into HomeSpan.  This project also covers various other advanced topics, including TCP slot management, dynamic creation of Accessories, and saving arbitrary data in the ESP32's NVS.
* Note *ESP32AsyncWebServer* cannot be used since it requires a TCP stack that is unfortunately incompatible with HomeSpan.  

---

[↩️](README.md) Back to the Welcome page

