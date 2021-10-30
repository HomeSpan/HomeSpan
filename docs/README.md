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
* 18 detailed tutorial-sketches with extensive comments, HomeSpan documentation and tips and tricks

### For the HomeSpan End-User

* Embedded WiFi Access Point and Web Interface to allow end-users (non-developers) to:
  * Set up Homespan with their own home WiFi Credentials
  * Create their own HomeKit Pairing Setup Code
* Status LED and Control Button to allow end-users to:
  * Force-unpair the device from HomeKit
  * Perform a Factory Reset
  * Launch the WiFi Access Point
* A standalone, detailed End-User Guide

## ❗Latest Update - HomeSpan 1.4.0 (10/9/2021)
 
**HomeSpan is now fully compatible with Version 2.0.0 of the Arduino-ESP32 Board Manager and will run on the following Espressif chips:**
* **ESP32**
* **ESP32-S2**
* **ESP32-C3**

HomeSpan also maintains full backwards-compatability with Version 1.0.6 of the Arduino-ESP Board Manager should you need to revert to that version.  This has been a complicated update! Please report any bugs or issues found when using Version 2 of the Arduino-ESP32.

Also included in HomeSpan 1.4.0 are the following new features and enhancements:

* **The PWM library has been upgraded**
  * users can specify a custom PWM frequency for each instance of LedPin() (subject to chip-specific resource limitations)
  * users can set the duty cycle of an LedPin() as a decimal floating number (e.g., 34.56), instead of just an integer
  * the library automatically sets the duty resolution to the maximum allowable value for a chosen PWM frequency
  * the library optimzizes the distribution of multiple LedPin() and ServoPin() instances to ensure all available PWM channels are used
  
* **The RFControl library has been upgraded**
  * the library allows for the transmisison of arbitrary-length pulse trains
  * users can pre-load pulse trains into one or more arbitrary-length arrays of 32-bit words for on-demand transmission as needed

* Users can now **limit the selection choices** of certain Characteristics (such as the Target State for a Security System) in the Home App using a new method, `setValidValues()`

* **The Status LED and Control Button are now optional components that are ignored unless specifically enabled**
  * because HomeSpan now runs on chips with many different pin configurations, HomeSpan's use of preset pin numbers for the Status LED and Control Button is likely to conflict with many devices
  * the default behavior for HomeSpan has been changed to ignore all logic related to the Status LED and Control Button
  * to enable the Status LED you must specify the pin to which your LED is attached using the usual method `homeSpan.setStatusPin(pin)`
  * to enable the Control Button you must specify the pin to which your Control Button is attached using the usual method `homeSpan.setControlPin(pin)`
 
See [Releases](https://github.com/HomeSpan/HomeSpan/releases) for details on all changes included in this update.

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
* [HomeSpan Extras](https://github.com/HomeSpan/HomeSpan/blob/master/docs/Extras.md) - integrated access to the ESP32's on-chip LED, Servo Motor, and Remote Control peripherals!
* [HomeSpan Television Services](https://github.com/HomeSpan/HomeSpan/blob/master/docs/TVServices.md) - how to use HomeKit's undocumented Television Services and Characteristics
* [HomeSpan Projects](https://github.com/topics/homespan) - real-world applications of the HomeSpan Library
* [HomeSpan FAQ](https://github.com/HomeSpan/HomeSpan/blob/master/docs/FAQ.md) - answers to frequently-asked questions

Note that all documentation is version-controlled and tied to each branch.  The *master* branch generally points to the latest release.  The *dev* branch, when available, will contain code under active development.

# External Resources

In addition to HomeSpan resources, developers who are new to HomeKit programming should download Apple's [HomeKit Accessory Protocol Specification, Release R2 (HAP-R2)](https://developer.apple.com/homekit/specification/). The download is free, but Apple requires you to register your Apple ID for access to the document.

You ***do not*** need to read the entire document.  The whole point of HomeSpan is that it implements all the required HAP operations under the hood so you can focus on just programming whatever logic is needed to control your real-world appliances (lights, fans, RF remote controls, etc.) with the device.  However, you will find Chapters 8 and 9 of the HAP guide to be an invaluable reference as it lists and describes all of the Services and Characteristics implemented in HomeSpan, many of which you will routinely utilize in your own HomeSpan sketches.

---

### Feedback or Questions?

Please consider adding to the [HomeSpan Discussion Board](https://github.com/HomeSpan/HomeSpan/discussions), or email me directly at [homespan@icloud.com](mailto:homespan@icloud.com).
