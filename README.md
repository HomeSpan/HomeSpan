# Welcome!

Welcome to HomeSpan - a robust and extremely easy-to-use Arduino library for creating your own [ESP32-based](https://www.espressif.com/en/products/modules/esp32) HomeKit devices entirely within the [Arduino IDE](http://www.arduino.cc).

HomeSpan provides a microcontroller-focused implementation of Apple's HomeKit Accessory Protocol Specification Release R2 (HAP-R2) designed specifically for the Espressif ESP32 microcontroller running within the Arduino IDE.  HomeSpan pairs directly to HomeKit via your home WiFi network without the need for any external bridges or components.  With HomeSpan you can use the full power of the ESP32's I/O functionality to create custom control software and/or hardware to automatically operate external devices from the Home App on your iPhone, iPad, or Mac, or with Siri.

### Requirements

|Component | Requirement | See Note |
|---|:---:|:---:|
|Current HomeSpan Production Release | **2.1.8** | - |
| Supported Chips | **ESP32, S2, S3, C3, C5, and C6** | [^8266] |
| Minimum Required [Arduino-ESP32 Core](https://github.com/espressif/arduino-esp32) | **3.3.0** | [^fail] |
| Latest Core fully tested with HomeSpan | **3.3.8** | [^tested] |
| Minimum Flash Partition Size | **1.9MB** | - |
| Recommended Partition Scheme | **Minimal SPIFFS (1.9MB APP with OTA)** | [^partition] |
| HomeKit Hub | **HomePod or Apple TV** | [^homehub] |

[^8266]:HomeSpan cannot be run on an ESP-8266, though an ESP-8266 can be used as a remote sensor in conjunction with HomeSpan's SpanPoint functionality

[^fail]: The current production release of HomeSpan will fail to compile under any Arduino-ESP32 Core release earlier than the minimum version listed.  However, prior versions of HomeSpan can be compiled and run under earlier versions of the Core.  The README file under the [HomeSpan branch](https://github.com/HomeSpan/HomeSpan/branches/all) for each release provides details on which version of the Core can be used for that release

[^tested]: Later releases may work fine, but have not (yet) been tested.  Note HomeSpan does *not* support the use of alpha, beta, or pre-release candidates of the Arduino-ESP32 Core - testing is only done on production releases of the Core.

[^homehub]: Apple's HomeKit architecture [requires the use of a Home Hub](https://support.apple.com/en-us/HT207057) (either a HomePod or Apple TV) for full and proper operation of any HomeKit device, including those based on HomeSpan.  ***Use of HomeSpan without a Home Hub is NOT supported.***

[^partition]: This recommendation assumes an ESP device with 4MB of flash memory.  For devices with 8MB or more of flash memory you may choose any partition scheme that provides at least 1.9MB APP space and 1.9MB of OTA space (if using OTA)

### HomeSpan Highlights

* Provides a natural, intuitive, and **very** easy-to-use framework
* Utilizes a unique *Service-Centric* approach to creating HomeKit devices
* Takes full advantage of the widely-popular Arduino IDE
* 100% HAP-R2 compliance
* Dozens of integrated HomeKit Services
* Operates in either Accessory or Bridge mode
* Supports pairing with Setup Codes or QR Codes
* Supports both WiFi and Ethernet connectivity to your home network
* Supports dual-stack use of both IPv4 and IPv6 addresses

### For the HomeSpan Developer

* Extensive use of the Arduino Serial Monitor
  * Real-time, easy-to-understand diagnostics
  * Complete transparency to every underlying HomeKit action, data request, and data response
  * Command-line interface with a variety of info, debugging, and configuration commands
* Built-in database validation to ensure your configuration meets all HAP requirements
* Dedicated classes that utilize the ESP32's 16-channel PWM peripheral for easy control of:
  * LED Brightness (including auto-fading)
  * Servo Motors
* Integrated Push Button and Toggle Switch functionality supporting single, double, and long presses of:
  * Physical pushbuttons that connect an ESP32 pin to either ground or VCC
  * Touch pads/sensors connected to an ESP32 pin (for ESP32 devices that support touch pads)
* Integrated access to the ESP32's on-chip Remote Control peripheral for easy generation of IR and RF signals
* Dedicated classes to control one- and two-wire addressable RGB LEDs and LED strips
* Dedicated classes to control stepper motors that can run smoothly in the background without interfering with HomeSpan
* Dedicated class that faciliates seamless point-to-point communication between ESP32 devices using ESP-NOW
* Integrated Web Log for user-defined log messages
* Extensively-commented Tutorial Sketches taking you from the very basics of HomeSpan through advanced HomeKit topics
* Additional examples and projects showcasing real-world implementations of HomeSpan
* A complete set of documentation explaining every aspect of the HomeSpan API

### For the HomeSpan End-User

* Embedded WiFi Access Point and Web Interface to allow end-users (non-developers) to:
  * Set up Homespan with their own home WiFi Credentials
  * Create their own HomeKit Pairing Setup Code
* Status LED and Control Button to allow end-users to:
  * Force-unpair the device from HomeKit
  * Perform a Factory Reset
  * Launch the WiFi Access Point
* A standalone, detailed End-User Guide

## ❗Latest Update - HomeSpan 2.1.8 (26 APR 2026)

### New Features

* **Updated HomeSpan Status logic and functionality for improved diagnostics**
  * Added new *HS_STATUS* enum type **HS_CONNECTED**
    * reflects fully operational state where HomeSpan is connected to a WiFi or Ethernet network, paired to HomeKit *and* is maintaining one or more secure connections to HomeKit
    * Status LED set to steady ON when in this state
  * Modified *HS_STATUS* enum type **HS_PAIRED** 
    * reflects potential "No Response" state where HomeSpan is connected to a WiFi or Ethernet network, paired to HomeKit *but* does **not** (yet) have any open secure connections to HomeKit
    * Status LED set to inverted double-blink when in this state
  * Added new `std::pair<HS_STATUS,uint32_t> homeSpan.getStatus()` method
    * returns *std::pair* containing:
      * current HomeSpan Status as *HS_STATUS* enum type
      * duration (in seconds) since HomeSpan first changed to that Status
    * this thread-safe method allows user to actively poll HomeSpan status and duration from the main `loop()` as alternative to using `homeSpan.setStatusCallback()` (which is only called when the HomeSpan Status changes)
  * Added new `void homeSpan.resetStatusDuration()` method
    * this thread-safe method allows user to reset current HomeSpan Status duration to zero
  * Complete re-write of HomeSpan Status documentation
    * demonstrates how to use the new methods above
    * provides example of using `homeSpan.getStatus()` to reboot HomeSpan if the device has lost secure HomeKit connections for an extended period of time
    * adds table providing graphic representation of all Status LED patterns for each HomeSpan Status state
    * adds direct link to this documentation on the main HomeSpan README.md page
  * See [HomeSpan Status and the HomeSpan Status LED](docs/HS_STATUS.md) for details

* **Improvements to Web Log output**
  * Added new **Client Connections Table** (similar output to 's' CLI command)
    * used as diagnostic to check whether HomeSpan has any active secure connections to HomeKit
  * Added descriptive class names for use with custom style sheets
    * *body* - background and header text (can use *bod1* for backwards compatibility)
    * *infoTable* - the top table that provides general information about the device (can use *tab1* for backwards compatibility)
    * *clientTable* - the newly-added table listing all active client connections (can use *tab2* for backwards compatibility)
    *  *logTable* - the bottom table listed all the individual Web Log entries (can also use *tab2* for backwards compatibility)
  * See [Message Logging](docs/Logging.md) for details
 
* **Created ability to programmatically retrieve base-64 encoded Pairing Data from within a sketch**
  * Added the following two new methods:
    * `const char* homeSpan.getPairingInfo(char **buf)`
      * allocates memory to *buf* and fills with HomeSpan's Device Pairing Data
      * returns *buf*, which must be de-allocated with `free(buf)` when no longer needed
    * `const char* Controller::getPairingInfo(char **buf)`
      *  allocates memory to *buf* and fills with a Controller's Pairing Data
      * returns *buf*, which must be de-allocated with `free(buf)` when no longer needed
  * Using these methods duplicates the exact Accessory and Controller Pairing Data provided by the 'P' CLI Command needed to clone devices
  * See the [API Reference](docs/Reference.md) for details on these methods and the [Cloning Pairing Data](docs/Cloning.md) page for a full example 

* **Added new `boolean homeSpan.usingEthernet()` method**
  * returns true if Ethernet interface is being used, else false if WiFi is being used
* **Added new `homeSpan.setControlTimes(uint32_t comTime, uint32_t resTime)` method**
  * allows users to customize the hold times required for the Control Button to trigger either the Command Mode or a Factory Reset
* **Redirected `setVal()` out-of-range warnings to WEBLOG instead of LOG0 so these warnings will be displayed in both the Serial Monitor and the Web Log**
* **Updated `CUSTOM_CHAR()` logic so that the HAP range and HAP description are automatically set based on the range and name specified in the macro**
              
See [Releases](https://github.com/HomeSpan/HomeSpan/releases) for details on all changes and bug fixes included in this update.

# HomeSpan Resources

HomeSpan includes the following documentation:

* [Getting Started with HomeSpan](docs/GettingStarted.md) - setting up the software and the hardware needed to develop HomeSpan devices
* [HomeSpan API Overview](docs/Overview.md) - an overview of the HomeSpan API, including a step-by-step guide to developing your first HomeSpan Sketch
* [HomeSpan Tutorials](docs/Tutorials.md) - a guide to HomeSpan's tutorial-sketches
* [HomeSpan Services and Characteristics](docs/ServiceList.md) - a list of all HAP Services and Characterstics supported by HomeSpan
* [HomeSpan Accessory Categories](docs/Categories.md) - a list of all HAP Accessory Categories defined by HomeSpan
* [HomeSpan Command-Line Interface (CLI)](docs/CLI.md) - configure a HomeSpan device's WiFi Credentials, modify its HomeKit Setup Code, monitor and update its status, and access detailed, real-time device diagnostics from the Arduino IDE Serial Monitor
* [HomeSpan WiFi and Ethernet Connectivity](docs/Networks.md) - a high-level discussion of HomeSpan's WiFi and Ethernet connectivity options
* [HomeSpan User Guide](docs/UserGuide.md) - turnkey instructions on how to configure an already-programmed HomeSpan device's WiFi Credentials, modify its HomeKit Setup Code, and pair the device to HomeKit.  No computer needed!
* [HomeSpan API Reference](docs/Reference.md) - a complete guide to the HomeSpan Library API
* [HomeSpan Status and the HomeSpan Status LED](docs/HS_STATUS.md) - how to programmatically read HomeSpan's Status and interpret the Status LED flashing patterns
* [HomeSpan QR Codes](docs/QRCodes.md) - create and use QR Codes for pairing HomeSpan devices
* [HomeSpan OTA](docs/OTA.md) - update your sketches Over-the-Air directly from the Arduino IDE without a serial connection
* [HomeSpan Watchdog Timer](docs/WDT.md) - optional protection that can trigger an automatic reboot if your sketch hangs or freezes for an extended period of time
* [HomeSpan PWM](docs/PWM.md) - integrated control of standard LEDs and Servo Motors using the ESP32's on-chip PWM peripheral
* [HomeSpan RFControl](docs/RMT.md) - easy generation of RF and IR Remote Control signals using the ESP32's on-chip RMT peripheral
* [HomeSpan Pixels](docs/Pixels.md) - integrated control of addressable one- and two-wire RGB and RGBW LEDs and LED strips
* [HomeSpan Stepper Motor Control](docs/Stepper.md) - integrated control of stepper motors, including PWM microstepping
* [HomeSpan SpanPoint](docs/NOW.md) - facilitates point-to-point, bi-directional communication between ESP32 Devices using ESP-NOW
* [HomeSpan Television Services](docs/TVServices.md) - how to use HomeKit's undocumented Television Services and Characteristics
* [HomeSpan Message Logging](docs/Logging.md) - how to generate log messages for display on the Arduino Serial Monitor as well as optionally posted to an integrated Web Log page
* [HomeSpan TLV8 Characteristics](docs/TLV8.md) - classes and methods for creating TLV8 objects to use with TLV8-based Characteristics
* [HomeSpan Adaptive Lighting](docs/AdaptiveLighting.md) - drop-in helper that implements Apple HomeKit's Adaptive Lighting feature for any LightBulb Service with both Brightness and ColorTemperature Characteristics
* [HomeSpan Device Cloning](docs/Cloning.md) - seamlessly swap a broken device for a new one without needing to re-pair and lose HomeKit automations
* [HomeSpan Projects](https://github.com/topics/homespan) - real-world applications of the HomeSpan Library
* [HomeSpan FAQ](docs/FAQ.md) - answers to frequently-asked questions
* [Solutions to Common Problems](docs/Solutions.md) - resolutions to some common problems when using/compiling HomeSpan
* [HomeSpan Reference Sketches](https://github.com/HomeSpan/HomeSpanReferenceSketches) - a collection of self-contained Reference Sketches showcasing some of the more complex HomeKit Services, such as Thermostats and Irrigation Systems

Note that all documentation is version-controlled and tied to each branch.  The *master* branch generally points to the latest release.  The *dev* branch, when available, will contain code under active development.

### Translated Documentation

A user-created repository containing a Chinese translation of the HomeSpan documentation can be found at [https://github.com/abackup/HomeSpan-Chinese](https://github.com/abackup/HomeSpan-Chinese).  Note HomeSpan is not responsible for the content of this repository - please direct any questions about the content of the translations to the issues page for that repository. 

# External Resources

In addition to HomeSpan resources, developers who are new to HomeKit programming may find useful Chapters 8 and 9 of Apple's HomeKit Accessory Protocol Specification, Non-Commercial Version, Release R2 (HAP-R2). This document is unfortunately no longer available from Apple (perhaps because it was last updated July, 2019, and is now somewhat out-of-date).  However, you may be able find copies of this document elsewhere on the web.  Note Apple has not replaced the HAP-R2 document with any other versions for non-commercial use, and Apple's open-source [HomeKit ADK](https://github.com/apple/HomeKitADK) only reflects the original HAP-R2 specs (rather than all the latest Services and Characteristics available in HomeKit for commercial devices).

---
### Matter and Thread

There are no plans to make HomeSpan compatible with Matter since HomeSpan was structured entirely around HAP R2.  In addition, both Apple and Espressif have released Matter SDKs for public use, reducing the need for yet another Matter SDK.

### Feedback or Questions?

Please post bug reports or other problems with HomeSpan to the [Issues Board](https://github.com/HomeSpan/HomeSpan/issues).  Please post all other questions about HomeSpan (use, functionality, specs, examples, etc.) or any ideas or recommendations you may have for new functionality, or any general feedback about HomeSpan or HomeKit, to the [Discussion Board](https://github.com/HomeSpan/HomeSpan/discussions).  For more general questions or feedback not related to a specific HomeSpan issue or function, you can email me directly at [homespan@icloud.com](mailto:homespan@icloud.com).

### About the Author

HomeSpan was developed and continues to be maintained and supported by Gregg Berman.  It was originally conceived to solve the pesky problem of not being able to operate an RF-controlled kitchen vent hood with Siri.  I hope you find it useful as well as fun to use.

This is my second large-scale open-source project --- my first was the design of an open-source sytem for operating model railroads using nothing more than an Arduino Uno and Arduino Motor Shield to generate digital command and control (DCC) signals.  Though I have not been involved with the model railroading hobby for many years, videos showcasing my original system (dubbed DCC++), along with detailed tutorials of how it works, are still available on the [DCC++ YouTube Channel](https://www.youtube.com/@dcc2840/videos). 
