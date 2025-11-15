# Welcome!

Welcome to HomeSpan - a robust and extremely easy-to-use Arduino library for creating your own [ESP32-based](https://www.espressif.com/en/products/modules/esp32) HomeKit devices entirely within the [Arduino IDE](http://www.arduino.cc).

HomeSpan provides a microcontroller-focused implementation of Apple's HomeKit Accessory Protocol Specification Release R2 (HAP-R2) designed specifically for the Espressif ESP32 microcontroller running within the Arduino IDE.  HomeSpan pairs directly to HomeKit via your home WiFi network without the need for any external bridges or components.  With HomeSpan you can use the full power of the ESP32's I/O functionality to create custom control software and/or hardware to automatically operate external devices from the Home App on your iPhone, iPad, or Mac, or with Siri.

### Requirements

|Component | Requirement | See Note |
|---|:---:|:---:|
|Current HomeSpan Production Release | **2.1.6** | - |
| Supported Chips | **ESP32, S2, S3, C3, C5, and C6** | [^8266] |
| Minimum Required [Arduino-ESP32 Core](https://github.com/espressif/arduino-esp32) | **3.1.0** | [^fail] |
| Latest Core fully tested with HomeSpan | **3.3.4** | [^tested] |
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

## ❗Latest Update - HomeSpan 2.1.6 (26 Oct 2025)

### New Features

* **HomeSpan now supports the ESP32-C5!**

  * the ESP32-C5 has the ability to use both the **5.0 GHz** and **2.4 GHz** WiFi bands 
    * added WiFi band information to all log file output to indicate which band is being used
  * to require the ESP-C5 to use the 5.0 GHz band, add the following to the `setup()` function in your sketch:
    * `WiFi.STA.begin(); WiFi.setBandMode(WIFI_BAND_MODE_5G_ONLY);`
    * note: `setBandMode()` is only available in Arduino-ESP32 Core 3.3.0 or greater
  * see [WiFi and Ethernet Connectivity](docs/Networks.md) for details

### Updates and Corrections

* **Updated OTA password storage to use SHA256 instead of MD5 hashing to conform with latest ArduinoOTA library protocol**
  * for backwards compatibility with prior Cores, HomeSpan uses SHA256 hashing only if compiled under Core 3.3.2 or greater, else it continues to use MD5
  * if you set your OTA password from within your sketch using `homeSpan.enableOTA(char *pwd)`, the new hashing will be automatic
  * if instead you previously entered your password into the Serial Monitor using the "O" CLI command, you will need to re-enter it again so that HomeSpan can save it as SHA256
    * if not re-entered, uploading OTA sketches using Core 3.3.2 or greater will still work, but a diagnostic message will warn you to migrate from MD5 to SHA256 hashing
  * to facilitate the ability to set a password as a **hash** instead of **plain-text** from within a sketch, `homeSpan.enableOTA(char *pwd)` has been modified so that if *pwd* starts with "0x" followed by 64 hexidecimal characters, *pwd* will be interpreted as a SHA256 hash instead of plain-text and HomeSpan will store it directly instead of first hashing it
    * specifying the hash of your OTA password inside a sketch is more secure than specifying the plain-text password
    * useful for devices that cannot be readily connected to a Serial port, which prevents you from using the "O" CLI command to enter your OTA password

* **Added new "c" CLI command that outputs to the Serial Monitor the same chip and sketch configuration information HomeSpan displays during initial start-up**
              
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
