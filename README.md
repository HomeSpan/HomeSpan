# Welcome!

Welcome to HomeSpan - a robust and extremely easy-to-use Arduino library for creating your own [ESP32-based](https://www.espressif.com/en/products/modules/esp32) HomeKit devices entirely within the [Arduino IDE](http://www.arduino.cc).

HomeSpan provides a microcontroller-focused implementation of Apple's HomeKit Accessory Protocol Specification Release R2 (HAP-R2) designed specifically for the Espressif ESP32 microcontroller running within the Arduino IDE.  HomeSpan pairs directly to HomeKit via your home WiFi network without the need for any external bridges or components.  With HomeSpan you can use the full power of the ESP32's I/O functionality to create custom control software and/or hardware to automatically operate external devices from the Home App on your iPhone, iPad, or Mac, or with Siri.

HomeSpan requires version 2 of the [Arduino-ESP32 Board Manager](https://github.com/espressif/arduino-esp32).  HomeSpan can be run on the original ESP32 as well as Espressif's ESP32-S2, ESP32-C3, and ESP32-S3 chips.

HomeSpan is currently NOT compatible with version 3.X of the Arduino-ESP32 Board Manager, since version 3 contains many breaking changes and is not backwards-compatible with version 2.X of the Arduino-ESP32 Board Manager.  At present, HomeSpan can only be compiled under version 2.X of the Board Manager. 

> [!NOTE]
> Apple's new HomeKit architecture [requires the use of a Home Hub](https://support.apple.com/en-us/HT207057) (either a HomePod or Apple TV) for full and proper operation of any HomeKit device, including those based on HomeSpan.  Without a Home Hub, HomeSpan cannot send notifications to the Home App - things like pushbuttons and temperature sensors will not be able to transmit updates to the Home App.  Use of HomeSpan without a Home Hub is NOT recommended.

### HomeSpan Highlights

* Provides a natural, intuitive, and **very** easy-to-use framework
* Utilizes a unique *Service-Centric* approach to creating HomeKit devices
* Takes full advantage of the widely-popular Arduino IDE
* 100% HAP-R2 compliance
* Dozens of integrated HomeKit Services
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

## ❗Latest Update - HomeSpan 1.9.1 (07/03/2024)

* **HomeSpan now supports *Tag-Length-Value ("TLV8")* Characteristics!**

  * adds new, fully-integrated `TLV8()` class library for the creation and management of TLV8 objects
  * includes methods to handle standard byte-stream VALUES as well as strings, numerical values, zero-length tags, and sub-TLVs
  * utilizes standard C++ iterators for easy access to reading and writing TLV8 records
  * adds new `Characteristic` methods `getTLV()`, `getNewTLV()`, and `setTLV()`
  * adds new `CUSTOM_CHAR_TLV8()` that allows for easy creation of custom TLV8 Characteristics
  * includes new [Tutorial Example 22 - TLV8 Characteristics](examples/22-TLV8_Characteristics) demonstrating use of the `TLV8()` class and TLV8 Characteristics
  * see the new [TLV8 Characteristics](docs/TLV8.md) page for complete details and documentation 
     
* **New *DisplayOrder* TLV8 Characteristic**
  
  * utlizes HomeSpan's new `TLV8()` library
  * allows you to specify the exact order in which the Input Sources for a Television Service are displayed in the Home App
  * see [Tutorial Example 22 - TLV8 Characteristics](examples/22-TLV8_Characteristics) for details
 
* **New *AccessoryIdentifier* Tutorial**

  * demonstrates how to trigger an Accessory's Identifier Characteristic, optionally used to help identify a device during initial pairing
  * see [Tutorial Example 21 - AccessoryIdentifier](examples/21-AccessoryIdentifier)

* **Added support for customizing Pixel chips**

  * new constructor `Pixel(uint8_t pin, [pixelType_t pixelType])` allows your to set the order in which colors are transmitted to the pixel chip, where *pixelType* is one of the following:
    * PixelType::RGB, PixelType::RBG, PixelType::BRG, PixelType::BGR, PixelType::GBR, PixelType::GRB
    * PixelType::RGBW, PixelType::RBGW, PixelType::BRGW, PixelType::BGRW, PixelType::GBRW, PixelType::GRBW* 
  * deprecated previous constructor `Pixel(uint8_t pin, boolean isRGBW)`
    * this constructor will continue to work, but you will receive a warning during compilation that it has been deprecated
    * users should switch to the new constructor to avoid potential compatibility issues with future versions of HomeSpan
  * added new method `boolean isRGBW()`
    * returns *true* if Pixel was constructed as RGBW, else *false* if constructed as RGB only (i.e. no white LED)
  * added new [PixelTester](examples/Other%20Examples/PixelTester) sketch (found under *Other Examples*) to aid in determining the *pixelType* for any LED Strip
  * see the [Adressable RGB LEDs](docs/Pixels.md) page for details
    
* **New ability to read and set the IIDs of Services and Characteristics**

  * adds new `SpanService` method `getIID()` that returns the IID of a Service
  * adds new `SpanCharacteristic` method `getIID()` that returns the IID of a Characteristic
  * adds new `homeSpan` method `resetIID(int newIID)` that resets the IID count for the current Accessory 
  * see the [API Reference](docs/Reference.md) for details
  
* **New ability to read Controller pairing data (for advanced use-cases only)**

  * adds new `homeSpan` methods `controllerListBegin()` and `controllerListEnd()` that returns iterators to HomeSpan's internal linked-list of Controller data records
  * adds new methods to read each Controller's pairing data:
    * `getID()` - returns a pointer to the 36-byte Device ID of the controller
    * `getLTPK()` - a pointer to the 32-byte Long-Term Public Key of the controller
    * `isAdmin()` - returns true if the controller has admin permission, else returns false
  * adds new `homeSpan` method `setControllerCallback()` to set optional callback function that HomeSpan calls whenever a controller is added, removed, or updated
  * see the [API Reference](docs/Reference.md) for details
 
* **HomeSpan now supports the *write-response ("WR")* protocol**
  * added automated handling of the HomeKits's *write-response ("WR")* protocol*
    * not needed for any Characteristics that are currently supported by HomeSpan, but useful for experimentation and work with Custom Characteristics
  * added extra checks when using `setVal()`
    * a warning message is output on the Serial Monitor if `setVal()` is called to change the value of a Characteristic from within the `update()` method at the same time the Home App is sending an update request for that value
    * does not apply if `setVal()` is called from within `update()` to change the value of a Characteristic in response to a *write-response* request from the Home App
   
* **Converted the `getLinks()` SpanService method to a template function**
  * allows user to automatically cast the elements of the returned vector into any specific Service type
  * also adds an optional parameter to restrict the elements of the returned vector to match a specified HomeSpan Service
  * see the [API Reference](docs/Reference.md) for details
 
* **New ability to halt the pulse generation for a ServoPin**
  * calling `set(NAN)` for a ServoPin halts the pulse generation, which (for most analog servos) allows the motor to be freely rotated
  * calling `set(position)`, where *position* equal the desired number of degrees, restarts the pulse generation and sets the servo position accordingly
   
* **Refactored client/slot management to save memory and prepare for future integration of Ethernet support**
  * fixed-array of Client/Socket connections replaced by dynamic linked-list
  * serial interface now only shows active client connections (rather than a fixed list of client slots)
  * **deprecated** `homeSpan.reserveSocketConnections()` since it is no longer needed
        
* **Fixed bug introduced in 1.9.0 that prevented `homeSpan.setPairingCode()` from saving (and subsequently using) the request Setup Pairing Code**
  * this method now operates silently, unless an invalid pairing code is provided, in which case an error is reported to the Serial Monitor and *the sketch is halted*
  * the process for setting the Pairing Code using the CLI 'S' command or via the Access Point are unchanged - confirmation messages are still output to the Serial Monitor and errors do *not* cause the sketch to halt

* **Fixed memory leak introduced in 1.9.0 that would fail to free a small temporary memory block created when verifying a new connection**
  * had no practical impact when using a Home Hub since Home Kit only creates a few permanent connections
  * had significant impact when not using a Home Hub in cases where the Home App repeatedly drops and re-establishes connections, resulting in slow erosion of heap memory and then out-of-memory failure of the device after a few days (note use of HomeSpan without a Home Hub is not formally supported)

* **Fixed latent bug in SpanPoint** 
  * HomeSpan would crash when printing **SpanPoint** configuration information to the Serial Monitor (the 'i' CLI command) if any of the instances of SpanPoint had *receiveSize=0*
  * this bug never surfaced before since all the **SpanPoint examples** were based on receiving data and therefore had a non-zero *receiveSize*
 
* **Deleted `homeSpan.setMaxConnections()`, which had been *deprecated* many versions ago**

* **Deleted stand-alone `SpanRange` structure, which had been *deprecated* many versions ago**
  * this has no impact on standard use of the Characteristic method `setRange()`
      
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

### Feedback or Questions?

Please post bug reports or other problems with HomeSpan to the [Issues Board](https://github.com/HomeSpan/HomeSpan/issues).  Please post all other questions about HomeSpan (use, functionality, specs, examples, etc.) or any ideas or recommendations you may have for new functionality, or any general feedback about HomeSpan or HomeKit, to the [Discussion Board](https://github.com/HomeSpan/HomeSpan/discussions).  For more general questions or feedback not related to a specific HomeSpan issue or function, you can email me directly at [homespan@icloud.com](mailto:homespan@icloud.com).

### About the Author

HomeSpan was developed and continues to be maintained and supported by Gregg Berman.  It was originally conceived to solve the pesky problem of not being able to operate an RF-controlled kitchen vent hood with Siri.  I hope you find it useful as well as fun to use.

This is my second large-scale open-source project --- my first was the design of an open-source sytem for operating model railroads using nothing more than an Arduino Uno and Arduino Motor Shield to generate digital command and control (DCC) signals.  Though I have not been involved with the model railroading hobby for many years, videos showcasing my original system (dubbed DCC++), along with detailed tutorials of how it works, are still available on the [DCC++ YouTube Channel](https://www.youtube.com/@dcc2840/videos). 
