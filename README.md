# Welcome!

Welcome to HomeSpan - a robust and extremely easy-to-use Arduino library for creating your own [ESP32-based](https://www.espressif.com/en/products/modules/esp32) HomeKit devices entirely within the [Arduino IDE](http://www.arduino.cc).

HomeSpan provides a microcontroller-focused implementation of Apple's HomeKit Accessory Protocol Specification Release R2 (HAP-R2) designed specifically for the Espressif ESP32 microcontroller running within the Arduino IDE.  HomeSpan pairs directly to HomeKit via your home WiFi network without the need for any external bridges or components.  With HomeSpan you can use the full power of the ESP32's I/O functionality to create custom control software and/or hardware to automatically operate external devices from the Home App on your iPhone, iPad, or Mac, or with Siri.

Requirements to run HomeSpan depend on which version you choose:

|HomeSpan Version | Arduino-ESP32 Board Manager | Partition Scheme | Supported Chips|
|:---:|:---:|:---:|---|
|1.9.1 or earlier | v2.0.0 - v2.0.17 | *Default* (1.3MB APP) | ESP32, S2, S3, C3 |
|2.0.0 or later | v3.0.2 - **v3.1.0**<sup>*</sup> | *Minimal SPIFFS* (1.9MB APP) | ESP32, S2, S3, C3, *and C6* |

<sup>*</sup>HomeSpan has been tested through **version 3.1.0** of the Arduino-ESP32 Board Manager (built on IDF 5.3.2).  Later releases may work fine, but have not (yet) been tested.  Note HomeSpan does not support the use of alpha, beta, or pre-release candidates of the Arduino-ESP32 Board Manager - testing is only done on production releases of the Board Manager.

**ADDITIONAL REQUIREMENTS**:  Apple's HomeKit architecture [requires the use of a Home Hub](https://support.apple.com/en-us/HT207057) (either a HomePod or Apple TV) for full and proper operation of any HomeKit device, including those based on HomeSpan.  ***Use of HomeSpan without a Home Hub is NOT supported.***

### HomeSpan Highlights

* Provides a natural, intuitive, and **very** easy-to-use framework
* Utilizes a unique *Service-Centric* approach to creating HomeKit devices
* Takes full advantage of the widely-popular Arduino IDE
* 100% HAP-R2 compliance
* Dozens of integrated HomeKit Services
* Operates in either Accessory or Bridge mode
* Supports pairing with Setup Codes or QR Codes
* Supports both WiFi and Ethernet connectivity to your home network

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

## ❗Latest Update - HomeSpan 2.1.0 (12/27/2024)

* **Integrated Support for Ethernet Connectivity!**

  * no new homeSpan methods are required.  Instead, during start-up HomeSpan checks if you've instructed the ESP32 to establish an Ethernet connection, and if so it will switch into "Ethernet mode" and not attempt to connect to your network via WiFi
  * once in Ethernet mode, HomeSpan customizes some of the output to the Serial Monitor and Web Log so it is clear Ethernet, and not WiFi, connectivity is being used
  * HomeSpan handles all reporting of connects/disconnects/reconnects just as it normally does for WiFi connections
  * to establish Ethernet connectivity, simply use the Arduino-ESP32's ETH library by calling `ETH.begin()` in your sketch with the appropriate parameters for your Ethernet board (assuming the Arduino-ESP32 library supports your board)
    * you must call `ETH.begin()` before calling `homeSpan.begin()`
    * you do **not** need to include `ETH.h` in your sketch
    * note the Arduino-ESP32 ETH library supports both direct-connect PHY as well as standalone SPI-based Ethernet boards
  * adds new homeSpan method `setConnectionCallback()`, which is a renamed version of the `setWifiCallbackAll()` method (now deprecated, see below) to reflect the fact that this method can be used with both Ethernet and WiFi connections

* **WiFi Enhancements and New WiFi Management Methods**

  * when connecting to a WiFi mesh network with multiple access points, HomeSpan now **automatically connects to the access point with the strongest WiFi signal** (i.e. the greatest RSSI)
    * previously HomeSpan would simply connect to the first access point it found that matched the SSID specified by the user, even if other access points with the same SSID had stronger signals
    * the BSSID (6-byte MAC address) of the access point to which HomeSpan is currently connected is provided in the Web Log as well as in the Serial Monitor in response to the 's' CLI command

  * added new homeSpan method `setConnectionTimes()` that allows users to fine-tune how long HomeSpan waits for each connection attempt when trying to connect to a WiFi network
  
  * added new homeSpan method `setWifiBegin()` that allows users to create an alternative function HomeSpan calls **instead of** `WiFi.begin()` when attempting to connect to a WiFi network

    * provides ability to create customizations, such as connecting to an enterprise network, or changing the WiFi power while connectivity is being established (required for some ESP32 boards with a misconfigured WiFi radio)
    
  * added new homeSpan method `enableWiFiRescan()` that causes HomeSpan to periodically re-scan for all access points matching the configured SSID and automatically switches to the access point with the strongest signal
     * useful after a mesh network is rebooted and HomeSpan initially reconnects to a more distance access point because a closer one with a stronger signal has not yet fully rebooted
       
  * added new homeSpan method `addBssidName()` that allows users to create optional display names for each access point in a WiFi mesh network according to their 6-byte BSSID addresses
     * when defined, HomeSpan will display both this name and the BSSID of an access point whenever presenting info on the Serial Monitor or writing to the Web Log
       
  * see the [API Reference](docs/Reference.md) page for full details, as well as the new [HomeSpan WiFi and Ethernet Connectivity](docs/Networks.md) page for a high-level discussion of HomeSpan's connectivity options

* **DEPRECATIONS**
  * `setWifiCallbackAll()` has been deprecated and renamed to `setConnectionCallback()` to reflect the fact this callback can be used for both WiFi and Ethernet connections
  * `setWifiCallback()` has been deprecated --- the more generic `setConnectionCallback()` should be used instead
    * requires any existing callbacks to be upgraded to add a single integer argument representing the number of connection attempts, similar to how `setWifiCallbackAll()`, and now `setConnectionCallback()`, work
  * both `setWifiCallbackAll()` and `setWifiCallback()` will be removed in a future version of HomeSpan.  Please update your sketches to avoid incompatibility with these future versions 

* **New CLI Commands**
  
  * 'D' - forces HomeSpan to disconnect and then automatically re-connect to the configured WiFi network
  * 'Z' - scans a user's WiFi network environment and displays information about each SSID (including each BSSID for mesh networks with multiple access points broadcasting the same SSID) on the Serial Monitor
  * see the [Command Line Interface (CLI)](docs/CLI.md) page for full details

* **New Multi-Threading Management**

  * made Web Log writing/reading thread-safe
    * fixes a latent bug related to a race condition between displaying the web log and writing a log record when the separate thread HomeSpan creates at start-up to handle initial contact with an NTP server records the time found

  * made HomeSpan autopolling thread-safe
    * adds two new macros, `homeSpanPAUSE` and `homeSpanRESUME`, that allow users to temporarily suspend the HomeSpan polling task once it completes its current run
    * allows users to make modifications to HomeSpan Characteristics and perform any other HomeSpan functions from a separate thread without worrying about inconsistencies if HomeSpan polling was being run at the same time
    * typically used when your sketch calls `homeSpan.autoPoll()` to run HomeSpan polling in a separate background task *and* you also want to make separate modifications to existing HomeSpan Characteristics by using `getVal()` and `setVal()` from within the main Arduino `loop()` (instead of, or in addition to, modifying these Characteristics from within their Service loops)
      
  * see the [API Reference](docs/Reference.md) page for full details, as well as a new [MultiThreading](examples/Other%20Examples/MultiThreading) Example that demonstrates the use of multi-threading using these macros

* **Web Log Enhancements**

  * Web Log can now auto-refresh from browser
    * if a Web Log request from a browser includes the query string, *refresh=N*, HomeSpan will add an HTTP Refresh response header to the HTML it serves back to the browser to cause it to auto-refresh the Web Log request every *N* seconds
    * example: *http<nolink>://homespan-4e8eb8504e59.local/status?refresh=10* 

  * Web Log requests are now case-insensitive
    * example: *http<nolink>://homespan-4e8eb8504e59.local/STATUS?REFRESH=10* is equivalent to above
 
* **Bug Fixes**
  * Fixes a latent bug that prevented compilation when the homeSpan methods `controllerListBegin()` and `controllerListEnd()` were used in a sketch
 
* **Known Issues**
  * Under v3.1.0 of the Arduino-ESP32 Board Manager, it is not possible to call an external NTP Server to set the time on C6 chips (all other chips unaffected).  See https://github.com/espressif/arduino-esp32/issues/10754 for details.
      
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

# External Resources

In addition to HomeSpan resources, developers who are new to HomeKit programming may find useful Chapters 8 and 9 of Apple's HomeKit Accessory Protocol Specification, Non-Commercial Version, Release R2 (HAP-R2). This document is unfortunately no longer available from Apple (perhaps because it was last updated July, 2019, and is now somewhat out-of-date).  However, you may be able find copies of this document elsewhere on the web.  Note Apple has not replaced the HAP-R2 document with any other versions for non-commercial use, and Apple's open-source [HomeKit ADK](https://github.com/apple/HomeKitADK) only reflects the original HAP-R2 specs (rather than all the latest Services and Characteristics available in HomeKit for commercial devices).

---
### Matter and Thread

There are no plans to make HomeSpan compatible with Matter since HomeSpan was structured entirely around HAP R2.  In addition, both Apple and Espressif have released Matter SDKs for public use, reducing the need for yet another Matter SDK.

Connecting HomeSpan directly to HomeKit via Thread is not planned (and might not even be possible).  However, Thread may be useful for inter-device communication similar to how HomeSpan uses ESP-NOW to implement remote, battery-operated devices.  This may be added at some point in a future release.

### Feedback or Questions?

Please post bug reports or other problems with HomeSpan to the [Issues Board](https://github.com/HomeSpan/HomeSpan/issues).  Please post all other questions about HomeSpan (use, functionality, specs, examples, etc.) or any ideas or recommendations you may have for new functionality, or any general feedback about HomeSpan or HomeKit, to the [Discussion Board](https://github.com/HomeSpan/HomeSpan/discussions).  For more general questions or feedback not related to a specific HomeSpan issue or function, you can email me directly at [homespan@icloud.com](mailto:homespan@icloud.com).

### About the Author

HomeSpan was developed and continues to be maintained and supported by Gregg Berman.  It was originally conceived to solve the pesky problem of not being able to operate an RF-controlled kitchen vent hood with Siri.  I hope you find it useful as well as fun to use.

This is my second large-scale open-source project --- my first was the design of an open-source sytem for operating model railroads using nothing more than an Arduino Uno and Arduino Motor Shield to generate digital command and control (DCC) signals.  Though I have not been involved with the model railroading hobby for many years, videos showcasing my original system (dubbed DCC++), along with detailed tutorials of how it works, are still available on the [DCC++ YouTube Channel](https://www.youtube.com/@dcc2840/videos). 
