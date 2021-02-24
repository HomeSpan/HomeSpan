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

#### How can I read HomeSpan's MDNS broadcast mentioned in the [OTA](OTA.md) documentation?

* HomeSpan uses MDNS (multicast DNS) to broadcast a variety of HAP information used by Controllers wishing to pair with HomeSpan.  Apple uses the name *Bonjour* to refer to MDNS, and originally included a Bonjour "Browser" in Safari that has since been discontinued.  However, there are a number of alternative MDNS browsers available for free that operate on both the Mac and the iPhone, such as the [Discovery - DNS-SD Browser](https://apps.apple.com/us/app/discovery-dns-sd-browser/id1381004916?mt=12).  You'll find all your HomeSpan devices, as well as any other HomeKit devices you may have, under the MDNS service named *_hap._tcp.*  The fields broadcast by HomeSpan are a combination of all data elements requires by HAP (HAP-R2, Table 6-7) plus three additional HomeSpan fields:

  * *hspn* - the version number of the HomeSpan library used to compile the sketch
  * *sketch* - the version number of the sketch, as specified with `homeSpan.setSketchVersion(const char *)`, or *n/a* if no version was specified
  * *ota* - either *yes* or *no* depending on whether OTA was enabled for the sketch using the method `homeSpan.enableOTA()` 

#### Where are the Television Services?

* Though HomeKit now supports Television controls, at the time of this posting Apple has not publicly released any specifications, SDKs, or ADKs, related to HAP Television Services or any HAP Television Characteristics.  It appears that for now these Services are meant only for commercial use.  Support for HAP Television Services and Characteristics will be added to HomeSpan as soon as Apple makes the specifications publicly available in some form or another.

---

[↩️](README.md) Back to the Welcome page

