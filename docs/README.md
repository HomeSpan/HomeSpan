# Welcome!

Welcome to HomeSpan - a robust and extremely easy-to-use Arduino library for creating your own [ESP32-based](https://www.espressif.com/en/products/modules/esp32) HomeKit devices entirely within the [Arduino IDE](http://www.arduino.cc).

HomeSpan provides a microcontroller-focused implementation of [Apple's HomeKit Accessory Protocol Specification Release R2 (HAP-R2)](https://developer.apple.com/homekit/specification/) designed specifically for the Espressif ESP32 microcontroller running within the Arduino IDE.  HomeSpan pairs directly to HomeKit via your home WiFi network without the need for any external bridges or components.  With HomeSpan you can use the full power of the ESP32's I/O functionality to create custom control software and/or hardware to automatically operate external devices from the Home App on your iPhone, iPad, or Mac, or with Siri.

HomeSpan is fully compatible with both Versions 1 and 2 of the [Arduino-ESP32 Board Manager](https://github.com/espressif/arduino-esp32).  Under Version 1, HomeSpan can be run only on the original ESP32.  Under Version 2, HomeSpan can be run on the original ESP32 as well as Espressif's ESP32-S2 and ESP32-C3 chips.

### HomeSpan Highlights

* Provides a natural, intuitive, and **very** easy-to-use framework
* Utilizes a unique *Service-Centric* approach to creating HomeKit devices
* Takes full advantage of the widely-popular Arduino IDE
* 100% HAP-R2 compliance
* 38 integrated HomeKit Services
* Operates in either Accessory or Bridge mode
* Supports pairing with Setup Codes or QR Codes

### For the HomeSpan Developer

* Extensive use of the Arduino Serial Monitor
  * Real-time, easy-to-understand diagnostics
  * Complete transparency to every underlying HomeKit action, data request, and data response
  * Command-line interface with a variety of info, debugging, and configuration commands
* Built-in database validation to ensure your configuration meets all HAP requirements
* Dedicated classes that utilize the ESP32's 16-channel PWM peripheral for easy control of:
  * LED Brightness
  * Servo Motors
* Integrated Push Button functionality supporting single, double, and long presses 
* Integrated access to the ESP32's on-chip Remote Control peripheral for easy generation of IR and RF signals
* Dedicated classes to control one- and two-wire addressable RGB and RGBW LEDs and LED strips
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

## ❗Latest Update - HomeSpan 1.5.1 (4/17/2022)

* **New Web Logging functionality**
  * HomeSpan can now host a Web Log page for message logging
  * New WEBLOG() macro makes is easy to create user-defined log messages
  * Provides for the optional use of an NTP Time Server to set the device clock so all messages can be properly timestamped
  * See [HomeSpan Message Logging](https://github.com/HomeSpan/HomeSpan/blob/master/docs/Logging.md) for full details

* **New *printf*-style formatting for LOG() macros**
  * Adds variadic forms of the LOG0(), LOG1(), and LOG2() macros so they can be used in the same manner as a standard C printf function
  * Greatly simplifies the creation of log messages 
  * See [HomeSpan Message Logging](https://github.com/HomeSpan/HomeSpan/blob/master/docs/Logging.md) for full details

* **New CUSTOM_SERV() macro**
  * Allows for the creation of Custom Services
  * Can be used in conjunction with the existing CUSTOM_CHAR() macro to produce Services beyond those provided in HAP-R2
  * Includes a fully worked example of a custom [Pressure Sensor Accessory](https://github.com/HomeSpan/HomeSpan/blob/master/Other%20Examples/CustomService) that is recognized by *Eve for HomeKit*
  * See [HomeSpan API Reference](https://github.com/HomeSpan/HomeSpan/blob/master/docs/Reference.md) for details

* **New "Safe-Load" mode for OTA updates**
  * HomeSpan can check to make sure the new sketch being uploaded via OTA is another HomeSpan sketch.  If not, the upload fails
  * Upon rebooting after an OTA update, HomeSpan checks to ensure that OTA is enabled in the updated sketch.  If not, HomeSpan rolls back to the previous version of the sketch
  * See [HomeSpan OTA](https://github.com/HomeSpan/HomeSpan/blob/master/docs/OTA.md) for full details
  
* **Additional updates include:**
  * a new (optional) argument to `SpanUserCommand()` that allows for passing a pointer to any arbitrary data structure
  * a new SPAN_ACCESSORY() macro that expands to a common snippet of code often used when creating Accessories 
  * refreshed and streamlined example Tutorials, and fully reworked Examples 7 and 11, to best conform with Home App behavior under iOS 15.4

See [Releases](https://github.com/HomeSpan/HomeSpan/releases) for details on all changes and bug fixes included in this update.

# HomeSpan Resources

HomeSpan includes the following documentation:

* [Getting Started with HomeSpan](https://github.com/HomeSpan/HomeSpan/blob/master/docs/GettingStarted.md) - setting up the software and the hardware needed to develop HomeSpan devices
* [HomeSpan API Overview](https://github.com/HomeSpan/HomeSpan/blob/master/docs/Overview.md) - an overview of the HomeSpan API, including a step-by-step guide to developing your first HomeSpan Sketch
* [HomeSpan Tutorials](https://github.com/HomeSpan/HomeSpan/blob/master/docs/Tutorials.md) - a guide to HomeSpan's tutorial-sketches
* [HomeSpan Services and Characteristics](https://github.com/HomeSpan/HomeSpan/blob/master/docs/ServiceList.md) - a list of all HAP Services and Characterstics supported by HomeSpan
* [HomeSpan Accessory Categories](https://github.com/HomeSpan/HomeSpan/blob/master/docs/Categories.md) - a list of all HAP Accessory Categories defined by HomeSpan
* [HomeSpan Command-Line Interface (CLI)](https://github.com/HomeSpan/HomeSpan/blob/master/docs/CLI.md) - configure a HomeSpan device's WiFi Credentials, modify its HomeKit Setup Code, monitor and update its status, and access detailed, real-time device diagnostics from the Arduino IDE Serial Monitor
* [HomeSpan User Guide](https://github.com/HomeSpan/HomeSpan/blob/master/docs/UserGuide.md) - turnkey instructions on how to configure an already-programmed HomeSpan device's WiFi Credentials, modify its HomeKit Setup Code, and pair the device to HomeKit.  No computer needed!
* [HomeSpan API Reference](https://github.com/HomeSpan/HomeSpan/blob/master/docs/Reference.md) - a complete guide to the HomeSpan Library API
* [HomeSpan QR Codes](https://github.com/HomeSpan/HomeSpan/blob/master/docs/QRCodes.md) - create and use QR Codes for pairing HomeSpan devices
* [HomeSpan OTA](https://github.com/HomeSpan/HomeSpan/blob/master/docs/OTA.md) - update your sketches Over-the-Air directly from the Arduino IDE without a serial connection
* [HomeSpan PWM](https://github.com/HomeSpan/HomeSpan/blob/master/docs/PWM.md) - integrated control of standard LEDs and Servo Motors using the ESP32's on-chip PWM peripheral
* [HomeSpan RFControl](https://github.com/HomeSpan/HomeSpan/blob/master/docs/RMT.md) - easy generation of RF and IR Remote Control signals using the ESP32's on-chip RMT peripheral
* [HomeSpan Pixels](https://github.com/HomeSpan/HomeSpan/blob/master/docs/Pixels.md) - integrated control of addressable one- and two-wire RGB and RGBW LEDs and LED strips
* [HomeSpan Television Services](https://github.com/HomeSpan/HomeSpan/blob/master/docs/TVServices.md) - how to use HomeKit's undocumented Television Services and Characteristics
* [HomeSpan Message Logging](https://github.com/HomeSpan/HomeSpan/blob/master/docs/Logging.md) - how to generate log messages for display on the Arduino Serial Monitor as well as optionally posted to an integrated Web Log page
* [HomeSpan Projects](https://github.com/topics/homespan) - real-world applications of the HomeSpan Library
* [HomeSpan FAQ](https://github.com/HomeSpan/HomeSpan/blob/master/docs/FAQ.md) - answers to frequently-asked questions

Note that all documentation is version-controlled and tied to each branch.  The *master* branch generally points to the latest release.  The *dev* branch, when available, will contain code under active development.

# External Resources

In addition to HomeSpan resources, developers who are new to HomeKit programming should download Apple's [HomeKit Accessory Protocol Specification, Release R2 (HAP-R2)](https://developer.apple.com/homekit/specification/). The download is free, but Apple requires you to register your Apple ID for access to the document.

You ***do not*** need to read the entire document.  The whole point of HomeSpan is that it implements all the required HAP operations under the hood so you can focus on just programming whatever logic is needed to control your real-world appliances (lights, fans, RF remote controls, etc.) with the device.  However, you will find Chapters 8 and 9 of the HAP guide to be an invaluable reference as it lists and describes all of the Services and Characteristics implemented in HomeSpan, many of which you will routinely utilize in your own HomeSpan sketches.

---

### Feedback or Questions?

Please consider adding to the [HomeSpan Discussion Board](https://github.com/HomeSpan/HomeSpan/discussions), or email me directly at [homespan@icloud.com](mailto:homespan@icloud.com).
