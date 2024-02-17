# Welcome!

Welcome to HomeSpan - a robust and extremely easy-to-use Arduino library for creating your own [ESP32-based](https://www.espressif.com/en/products/modules/esp32) HomeKit devices entirely within the [Arduino IDE](http://www.arduino.cc).

HomeSpan provides a microcontroller-focused implementation of Apple's HomeKit Accessory Protocol Specification Release R2 (HAP-R2) designed specifically for the Espressif ESP32 microcontroller running within the Arduino IDE.  HomeSpan pairs directly to HomeKit via your home WiFi network without the need for any external bridges or components.  With HomeSpan you can use the full power of the ESP32's I/O functionality to create custom control software and/or hardware to automatically operate external devices from the Home App on your iPhone, iPad, or Mac, or with Siri.

HomeSpan requires version 2.0.0 or later of the [Arduino-ESP32 Board Manager](https://github.com/espressif/arduino-esp32), and has been tested up through version 2.0.14 (recommended).  HomeSpan can be run on the original ESP32 as well as Espressif's ESP32-S2, ESP32-C3, and ESP32-S3 chips.

> [!NOTE]
> Apple's new HomeKit architecture [requires the use of a Home Hub](https://support.apple.com/en-us/HT207057) (either a HomePod or Apple TV) for full and proper operation of any HomeKit device, include those based on HomeSpan.  Without a Home Hub, HomeSpan cannot send notifications to the Home App - things like pushbuttons and temperature sensors will not be able to transmit updates to the Home App.

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
  * LED Brightness (including auto-fading)
  * Servo Motors
* Integrated Push Button and Toggle Switch functionality supporting single, double, and long presses of:
  * Physical pushbuttons that connect an ESP32 pin to either ground or VCC
  * Touch pads/sensors connected to an ESP32 pin (for ESP32 devices that support touch pads)
* Integrated access to the ESP32's on-chip Remote Control peripheral for easy generation of IR and RF signals
* Dedicated classes to control one- and two-wire addressable RGB and RGBW LEDs and LED strips
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

## ❗Latest Update - HomeSpan 1.9.0 (2/17/2024)

* **HomeSpan has been optimized to use significantly less RAM!**

  * supports approximately **TWICE** the number of Accessories using the same amount of memory
  * minimized memory use also means much more room for users to add non-HomeSpan features to their sketch without running out of memory, especially if the non-HomeSpan code consumes a lot of stack space
  * HomeSpan now automatically detects the presence of **PSRAM** (SPIRAM) and will utilize that extra memory to the largest extent possible to keep internal RAM free for certain HomeSpan functions and ESP32 functions (e.g. WiFi) that require internal RAM.  Also keeps internal RAM free for use by any non-HomeSpan code that does not (or cannot) use PSRAM
  * increased HomeSpan's 41-Accessory limit to a 150-Accessory Limit (as specified by HAP) since it is now possible to create a device with many more than 41 Accessories without running out of memory, especially if PSRAM is used

* **HomeSpan has been optimized to use significantly less Non-Volatile Storage (NVS)**

  * allows you to use NVS to save the values of a many more Characteristics 
  * see the newly-added [CustomNVSPartition](docs/Tutorials.md#customnvspartition) example that demonstrates how to create your own Partition Scheme to further expand the size of the NVS partition beyond the ESP32 default to support sketches with a large number of Accessories each configured to use NVS to save the values of many Characteristics

* **New features and documentation for Services and Characteristics**
  * created "enumerated constants" (e.g. *SWING_ENABLED*, *HUMIDIFYING*, etc.) for every applicable Characteristic that can be used instead of integers when reading and writing values
    * very helpful since Apple is no longer publishing its non-commercial HAP document that provided a list and description of the states for each Characteristic
    * example:  `if(target.getNewVal()==target.ARM_STAY) {...}`
  * added ability to properly name individual Services within a single Accessory using new **Characteristic::ConfiguredName()**
    * see revised [Example 11 - ServiceNames](docs/Tutorials.md#example-11---servicenames) for details
  * new [Services and Characteristics](docs/ServiceList.md) page now provides functional descriptions and detailed specifications for every Service and Characteristic supported by HomeSpan, including a list of the enumerated constants available for every Characteristic
   
* **New ability to use *Inverted Buttons* and *Touch Sensors* as a Control Button**

  * adds *triggerType* as a second, optional argument to `Span& setControlPin(uint8_t pin, triggerType_t triggerType)`
  * supports TRIGGER_ON_LOW, TRIGGER_ON_HIGH, TRIGGER_ON_TOUCH, or any user-defined function
  * see [API Reference](docs/Reference.md) for details

* **New ability to "remotely" trigger user-defined actions by repeatedly power-cycling the device**

  * adds new homeSpan method `Span& setRebootCallback(void (*func)(uint8_t count), uint32_t upTime=5000)`
  * the parameter *count*, which is passed by HomeSpan to *func*, indicates the number of "short" reboots that have occurred prior to the current reboot, where a "short" reboot is any that occurs before *upTime* milliseconds have elapsed
  * can be use to remotely restore a device that is not easily accessible to a pre-defined state
  * see [API Reference](docs/Reference.md) for details

* **Added two new Stepper Motor Drivers**
  * **Stepper_UNIPOLAR**: a generic driver for any 4-wire center-tapped unipolar motor
  * **Stepper_ULN2003A**: support for the ULN2003A driver board
  * see [Stepper Motor Control Stepper](docs/Stepper.md) for details

* **Additional Web Log functionality**

  * adds new homeSpan method `Span& setWebLogCallback(void (*func)(String &))`
    * allows users to include additional data and custom HTML in the Web Log
  * adds new homeSpan method `getWebLog(void (*f)(const char *, void *), void *args)`
    * allows users to retrieve the underlying Web Log HTML from within sketch
  * modified `enableWebLog()` so that it can be used to set the time from an NTP server without actually serving Web Log pages
  * see [Message Logging](docs/Logging.md) for details

* **Added ability to "chain" *homeSpan* methods**

  * converted various *homeSpan* methods that previously returned *void* to now return *Span &*
  * example: `homeSpan.setControlPin(21).setStatusPin(13);`
  * see [API Reference](docs/Reference.md) for details

* **Added ability to disable SpanPoint encryption**

  * without encryption increases the maximum number of allowed SpanPoint devices from 7 to 20
  * see [SpanPoint](docs/NOW.md) for details
 
* **Other new *homeSpan* methods included in this release:**

  * `Span& setVerboseWifiReconnect()` - optionally suppresses "Trying to connect to..." messages
  * `Span& setWifiCallbackAll()` - provides an optional callback every time WiFi is connected *or re-connected*
  * `TaskHandle_t getAutoPollTask()` - returns the Task Handle for the HomeSpan Auto Poll Task
 
* **Removed dependencies on various "extra" `#include` files**
  * the following \#include files are now embedded in *HomeSpan.h* and **should not be specified in any sketch:**
    * *extras/Pixel.h*
    * *extras/RFControl.h*
    * *extras/PwmPin.h*
    * *extras/StepperControl.h*

> [!IMPORTANT]
> At present it is okay to include the above `#include` files in your sketch (they have no effect on the compiled code), but they will be deleted at some point in the future so please remove them from your sketches now to ensure forward compatibility with subsequent releases. 
      
See [Releases](https://github.com/HomeSpan/HomeSpan/releases) for details on all changes and bug fixes included in this update.

# HomeSpan Resources

HomeSpan includes the following documentation:

* [Getting Started with HomeSpan](docs/GettingStarted.md) - setting up the software and the hardware needed to develop HomeSpan devices
* [HomeSpan API Overview](docs/Overview.md) - an overview of the HomeSpan API, including a step-by-step guide to developing your first HomeSpan Sketch
* [HomeSpan Tutorials](docs/Tutorials.md) - a guide to HomeSpan's tutorial-sketches
* [HomeSpan Services and Characteristics](docs/ServiceList.md) - a list of all HAP Services and Characterstics supported by HomeSpan
* [HomeSpan Accessory Categories](docs/Categories.md) - a list of all HAP Accessory Categories defined by HomeSpan
* [HomeSpan Command-Line Interface (CLI)](docs/CLI.md) - configure a HomeSpan device's WiFi Credentials, modify its HomeKit Setup Code, monitor and update its status, and access detailed, real-time device diagnostics from the Arduino IDE Serial Monitor
* [HomeSpan User Guide](docs/UserGuide.md) - turnkey instructions on how to configure an already-programmed HomeSpan device's WiFi Credentials, modify its HomeKit Setup Code, and pair the device to HomeKit.  No computer needed!
* [HomeSpan API Reference](docs/Reference.md) - a complete guide to the HomeSpan Library API
* [HomeSpan QR Codes](docs/QRCodes.md) - create and use QR Codes for pairing HomeSpan devices
* [HomeSpan OTA](docs/OTA.md) - update your sketches Over-the-Air directly from the Arduino IDE without a serial connection
* [HomeSpan PWM](docs/PWM.md) - integrated control of standard LEDs and Servo Motors using the ESP32's on-chip PWM peripheral
* [HomeSpan RFControl](docs/RMT.md) - easy generation of RF and IR Remote Control signals using the ESP32's on-chip RMT peripheral
* [HomeSpan Pixels](docs/Pixels.md) - integrated control of addressable one- and two-wire RGB and RGBW LEDs and LED strips
* [HomeSpan Stepper Motor Control](docs/Stepper.md) - integrated control of stepper motors, including PWM microstepping
* [HomeSpan SpanPoint](docs/NOW.md) - facilitates point-to-point, bi-directional communication between ESP32 Devices using ESP-NOW
* [HomeSpan Television Services](docs/TVServices.md) - how to use HomeKit's undocumented Television Services and Characteristics
* [HomeSpan Message Logging](docs/Logging.md) - how to generate log messages for display on the Arduino Serial Monitor as well as optionally posted to an integrated Web Log page
* [HomeSpan Device Cloning](docs/Cloning.md) - seamlessly swap a broken device for a new one without needing to re-pair and lose HomeKit automations
* [HomeSpan Projects](https://github.com/topics/homespan) - real-world applications of the HomeSpan Library
* [HomeSpan FAQ](docs/FAQ.md) - answers to frequently-asked questions
* [Solutions to Common Problems](docs/Solutions.md) - resolutions to some common problems when using/compiling HomeSpan
* [HomeSpan Reference Sketches](https://github.com/HomeSpan/HomeSpanReferenceSketches) - a collection of self-contained Reference Sketches showcasing some of the more complex HomeKit Services, such as Thermostats and Irrigation Systems

Note that all documentation is version-controlled and tied to each branch.  The *master* branch generally points to the latest release.  The *dev* branch, when available, will contain code under active development.

# External Resources

In addition to HomeSpan resources, developers who are new to HomeKit programming may find useful Chapters 8 and 9 of Apple's HomeKit Accessory Protocol Specification, Non-Commercial Version, Release R2 (HAP-R2). This document is unfortunately no longer available from Apple (perhaps because it was last updated July, 2019, and is now somewhat out-of-date).  However, you may be able find copies of this document elsewhere on the web.  Note Apple has not replaced the HAP-R2 document with any other versions for non-commercial use, and Apple's open-source [HomeKit ADK](https://github.com/apple/HomeKitADK) only reflects the original HAP-R2 specs (rather than all the latest Services and Characteristics available in HomeKit for commercial devices).

---

### Feedback or Questions?

Please post bug reports or other problems with HomeSpan to the [Issues Board](https://github.com/HomeSpan/HomeSpan/issues).  Please post all other questions about HomeSpan (use, functionality, specs, examples, etc.) or any ideas or recommendations you may have for new functionality, or any general feedback about HomeSpan or HomeKit, to the [Discussion Board](https://github.com/HomeSpan/HomeSpan/discussions).  For more general questions or feedback not related to a specific HomeSpan issue or function, you can email me directly at [homespan@icloud.com](mailto:homespan@icloud.com).

### About the Author

HomeSpan was developed and continues to be maintained and supported by Gregg Berman.  It was originally conceived to solve the pesky problem of not being able to operate an RF-controlled kitchen vent hood with Siri.  I hope you find it useful as well as fun to use.

This is my second large-scale open-source project --- my first was the design of an open-source sytem for operating model railroads using nothing more than an Arduino Uno and Arduino Motor Shield to generate digital command and control (DCC) signals.  Though I have not been involved with the model railroading hobby for many years, videos showcasing my original system (dubbed DCC++), along with detailed tutorials of how it works, are still available on the [DCC++ YouTube Channel](https://www.youtube.com/@dcc2840/videos). 
