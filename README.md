# Welcome!

Welcome to HomeSpan - a robust and extremely easy-to-use Arduino library for creating your own [ESP32-based](https://www.espressif.com/en/products/modules/esp32) HomeKit devices entirely within the [Arduino IDE](http://www.arduino.cc).

HomeSpan provides a microcontroller-focused implementation of Apple's HomeKit Accessory Protocol Specification Release R2 (HAP-R2) designed specifically for the Espressif ESP32 microcontroller running within the Arduino IDE.  HomeSpan pairs directly to HomeKit via your home WiFi network without the need for any external bridges or components.  With HomeSpan you can use the full power of the ESP32's I/O functionality to create custom control software and/or hardware to automatically operate external devices from the Home App on your iPhone, iPad, or Mac, or with Siri.

HomeSpan requires version 2.0.0 or later of the [Arduino-ESP32 Board Manager](https://github.com/espressif/arduino-esp32), and has been tested up through version 2.0.14 (recommended).  HomeSpan can be run on the original ESP32 as well as Espressif's ESP32-S2, ESP32-C3, and ESP32-S3 chips.

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

## ❗Latest Update - HomeSpan 1.9.0 (2/XX/2024)

* **New ability to "chain" *homeSpan* methods**

  * converted various *homeSpan* methods that previously returned *void* to now return *Span &*
  * example: `homeSpan.setControlPin(21).setStatusPin(13);`
  * see [API Reference](Reference.md) for details

* **Additional Web Log functionality**

  * adds new homeSpan method `Span& setWebLogCallback(void (*func)(String &))` to allow user to add custom HTML to the Web Log
  * adds new homeSpan method `getWebLog(void (*f)(const char *, void *), void *args)` to retrieve Web Log HTML from within sketch
  * modified `enableWebLog()` so that it can be used to set the time from an NTP server without actually serving Web Log pages
  * see [Message Logging](Logging.md) for details

* **New WiFi reporting customizations**
 
  * adds new homeSpan method `Span& setVerboseWifiReconnect(bool verbose=true)` to suppress reconnection messages
  * adds new homeSpan method `Span& setWifiCallbackAll(void (*func)(int count))` to cal WiFI Callback function every time WiFi reconnects
  * see [API Reference](Reference.md) for details

* **New ability to trigger HomeSpan actions by power-cycling (or resetting) the device one or more times**

  * adds new homeSpan method `Span& setRebootCallback(void (*func)(uint8_t count), uint32_t upTime=5000)`
  * the parameter *count*, which is passed by HomeSpan to *func*, indicates the number of "short" reboots that have occurred prior to the current reboot, where a "short" reboot is any that occurs before *upTime* milliseconds have elapsed
  * see [API Reference](Reference.md) for details

* **New ability to disable SpanPoint encryption**

  * adds new class-level method `SpanPoint::setEncryption(boolean encrypt)`
  * enabling/disabling SpanPoint encryption changes the maximum number of connections that can be supported by a single device:
    * with encryption, max=7 remote devices
    * without encryption, max=20 remote devices
  * see [SpanPoint](NOW.md) for details

* **New ability to get handle of HomeSpan Auto Poll task**

  * adds homeSpan method `TaskHandle_t getAutoPollTask()`
  * see [API Reference](Reference.md) for details

* **New ability to use Inverted or Touch Buttons as a Control Button**

  * adds *triggerType* as a second, optional argument to `Span& setControlPin(uint8_t pin, triggerType_t triggerType)`
    support TRIGGER_ON_LOW, TRIGGER_ON_HIGH, TRIGGER_ON_TOUCH, or any user-defined function
  * see [API Reference](Reference.md) for details

* **New Services and Characteristics features**
  * HomeSpan constants can be used instead of integers when reading and writing values of applicable Characteristics
    * very helpful since Apple is no longer publishing its non-commercial HAP document that provided a list and description of the states for each Characteristic
    * example:  `if(target.getNewVal()==target.ARM_STAY) {...}`
  * new ability to name individual Services using new **Characteristic::ConfiguredName()**
  * updated Services and Characteristics page now provides functional descriptions and detailed specifications for every Service and Characteristic supported by HomeSpan
  * see [Services and Characteristics](ServiceList.md) for details

* **New Stepper Motor Drivers**
  * **Stepper_UNIPOLAR**: a generic driver for any 4-wire center-tapped unipolar motor
  * **Stepper_UNL2003**: support for the UNL2003 driver board
  * see [Stepper Motor Control Stepper](Stepper.md) for details

    
### Significant Refactoring to Optimize Memory Use

* HomeSpan can now support approximately TWICE the number of Accessories using the same amount of memory
* Minimized memory use also means much more room for users to add non-HomeSpan features to their sketch without running out of memory, especially if the non-HomeSpan code consumes a lot of stack space
* HomeSpan now automatically detects the presence of PSRAM (SPIRAM) and will utilize that extra memory to the largest extent possible to keep internal RAM free for certain HomeSpan functions and ESP32 functions (e.g. WiFi) that require internal RAM.  Also keeps internal RAM free for use by any non-HomeSpan code that does not (or cannot) use PSRAM
* Changed HomeSpan's 41-Accessory limit to a 150-Accessory Limit (as specified by HAP) since it is now possible to create a device with many more than 41 Accessories without running out of memory, especially if PSRAM is used (#619)
* HomeSpan will print a warning to the Serial Monitor at boot-up if free internal RAM seems to be getting too low based on memory use before WiFi is started
* Refactoring included:
  * converted all large fixed-size stack-based buffers to dynamically-sized heap-based buffers
  * ensure applicable heap-based buffers use PSRAM when available
  * added custom allocators to containers, and custom constructors to objects, to ensure they use PSRAM when available
  * converted the static array that stored all pairing info for 16 Controllers to a linked-list only storing data for Controllers as needed
    * HomeKit typically only assigns a few Controllers, so no need always reserve space for 16
    * also frees up a lot of NVS space (since Controller pairing info is stored there)
  * converted the containers holding the lists of required and optional Characteristics for each Service to a much more efficient storage class 
  * re-wrote the TLV8 class from being a large, fixed-size, globally-scoped, HAP-specific object to a completely generic TLV8 class supporting the simultaneous dynamic creation of multiple TLV8 objects, each with arbitrary tags sized dynamically as needed-sized
    * provides an opportunity to *potentially* add TLV-based Characteristics in the future
  * updated the SRP6A class to minimize memory use, as well as delete itself after pairing is completed and it is no longer needed
  * created new hapOut stream with a custom buffer that can simultaneously output text to the Serial Monitor, compute hash codes for any block of data, and transmit data to HAP clients, either in plain text, or encrypted on the fly
    * automatically chunks data on the fly into blocks of 1024 bytes, and, if needed, encrypts those blocks before transmitting  
    * dramatically reduces memory usage when transmitting or printing large blocks of text (e.g. the JSON response to a HAP getAccessories request)

### Refactoring to Optimize NVS Use

* NVS memory needed for the (optional) storage of Characteristic values has been reworked to take up a much smaller number of NVS records, allowing for the NVS storage of many more Characteristic values
  * added check at start-up informing user if NVS space is running low
* Added new CustomNVSPartition Example demonstrating how to create your own Partition Scheme to expand the size of the NVS partition and store a very large number of Characteristics (needed for sketches that create many Accessories, each with lots of stored Characteristics)
  
### Other Internal Updates

* **Added internal support for listing controllers in HAP /postPairing**
    * this HAP request was never used by iOS until recent updates, so is now needed 

* **Moved `vTaskDelay(5)` from `pollTask()` into `autoPoll()` (#668)**
  * this 5 ms delay is now called *only* when using `autoPoll()` (it is needed to allow the polling task to yield to other tasks)
  * the delay is no longer used when directly calling `poll()` from within the main `loop()` (it is not needed)

* **Updated *Pixel Example* to allow for strands of longer than 255 LEDs (#654)**
  * the Pixel library already allows for this - it was *only* the example that limited the number by inadvertently using *uint8_t* as the parameter for the number of pixels specified in a strand

* **Added support to properly display UTF-8 WiFi names when running HomeSpan Setup Access Point  (#631)**

* **Limit the internal storage of all string-based Characteristics to 64 characters to prevent buffer overflows (#659)**
  * this equates to the default limit specified by HAP

* **Updated *RemoteDevice* Example to show MAC Address for Remote Devices on Serial Monitor during boot-up (#660)**

* **Removed dependency on *core_version.h* (#744)**
  * the Arduino-ESP32 version is now sourced instead from *esp_arduino_version.h*












      
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

In addition to HomeSpan resources, developers who are new to HomeKit programming should download Apple's HomeKit Accessory Protocol Specification, Non-Commercial Version, Release R2 (HAP-R2). This document is unfortunately no longer available from Apple (perhaps because it was last updated July, 2019, and is now somewhat out-of-date).  However, you may be able find copies of this document elsewhere on the web.  Note Apple has not replaced the HAP-R2 document with any other versions for non-commercial use, and Apple's open-source [HomeKit ADK](https://github.com/apple/HomeKitADK) only reflects the original HAP-R2 specs (rather than all the latest Services and Characteristics available in HomeKit for commercial devices).

You ***do not*** need to read the entire HAP-R2 document. The whole point of HomeSpan is that it implements all the required HAP operations under the hood so you can focus on just programming whatever logic is needed to control your real-world appliances (lights, fans, RF remote controls, etc.) with the device.  However, you will find Chapters 8 and 9 of the HAP guide to be an invaluable reference as it lists and describes all of the Services and Characteristics implemented in HomeSpan, many of which you will routinely utilize in your own HomeSpan sketches.

---

### Feedback or Questions?

Please post bug reports or other problems with HomeSpan to the [Issues Board](https://github.com/HomeSpan/HomeSpan/issues).  Please post all other questions about HomeSpan (use, functionality, specs, examples, etc.) or any ideas or recommendations you may have for new functionality, or any general feedback about HomeSpan or HomeKit, to the [Discussion Board](https://github.com/HomeSpan/HomeSpan/discussions).  For more general questions or feedback not related to a specific HomeSpan issue or function, you can email me directly at [homespan@icloud.com](mailto:homespan@icloud.com).

### About the Author

HomeSpan was developed and continues to be maintained and supported by Gregg Berman.  It was originally conceived to solve the pesky problem of not being able to operate an RF-controlled kitchen vent hood with Siri.  I hope you find it useful as well as fun to use.

This is my second large-scale open-source project --- my first was the design of an open-source sytem for operating model railroads using nothing more than an Arduino Uno and Arduino Motor Shield to generate digital command and control (DCC) signals.  Though I have not been involved with the model railroading hobby for many years, videos showcasing my original system (dubbed DCC++), along with detailed tutorials of how it works, are still available on the [DCC++ YouTube Channel](https://www.youtube.com/@dcc2840/videos). 
