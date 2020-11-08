# HomeSpan
*HomeKit for the Arduino-ESP32!*

Welcome to HomeSpan - a robust and extremely easy-to-use Arduino library for creating your own [ESP32-based](https://www.espressif.com/en/products/modules/esp32) HomeKit devices entirely within the [Arduino IDE](http://www.arduino.cc).

HomeSpan provides a microcontroller-focused implementation of [Apple's HomeKit Accessory Protocol Specification (HAP) Release R2](https://developer.apple.com/support/homekit-accessory-protocol/) designed specifically for the Espressif ESP32 microcontroller running within the Arduino IDE.  HomeSpan pairs directly to HomeKit via your home WiFi network without the need for any external bridges or components.  With HomeSpan you can use the full power of the ESP32's I/O functionality to create custom control software and/or hardware to automatically operate external devices from the Home App on your iPhone, iPad, or Mac, or with Siri.

### HomeSpan Highlights

* Provides a natural, intuitive, and **very** easy-to-use framework
* Utilizes a unique *Service-Centric* approach to creating HomeKit devices
* Takes full advantage of the widely-popular Arduino IDE
* 100% HAP R2 compliance
* 38 integrated [HomeKit Services](docs/ServiceList.md)
* Operates in either Accessory or Bridge mode

### For the HomeSpan Developer

* Extensive use of the Arduino Serial Monitor
  * Real-time, easy-to-understand diagnostics
  * Complete transparency to every underlying HomeKit action, data request, and data response
  * Command-line interface with a variety of info, debugging, and configuration commands
* Built-in database validation to ensure your configuration meets all HAP requirements
* Integrated PWM functionality supporting pulse-wave-modulation on any ESP32 pin
* Integrated Push Button functionality supporting single, double, and long presses 
* Integrated access to the ESP32's on-chip Remote Control peripheral for easy generation of IR and RF signals
* 16 detailed tutorial-examples with extensive comments, instructions, and HomeKit tips and tricks

### For the HomeSpan End-User

* Embedded WiFi Access Point and Web Interface to allow end-users (non-developers) to:
  * Setup Homespan with their own home WiFi Credentials
  * Create their own HomeKit Pairing Setup Code
* Status LED and Control Button to allow end-users to:
  * Unpair the device from HomeKit
  * Perform a Factory Reset
  * Launch the WiFi Access Point
* A 10-page [End-User Guide](docs/UserGuide.md)

## Latest Updates ##

#### 11/3/2020

* HomeSpan 1.0.0 is the initial public release. It is 100% feature-complete and includes 16 very detailed example-tutorials providing step-by-step instructions on how to use HomeSpan to create your own HomeKit devices. A 10-page User Guide provides addtional details on how to configure your finished HomeKit device for use on your WiFi network, and how to pair it to HomeKit.

## Getting Started ##

1. Since HomeSpan is a library designed for Arduino's Integrated Development Environment, the first step is to download and install the latest version of this IDE from the [Arduino Software page](https://www.arduino.cc/en/software).  If you have experience with programming Arduino sketches, you'll find Homespan's use of the standard Arduino setup() and loop() functions, as well as the ever-present *\*.ino* file, very familiar.  If you've never programmed an Arduino, you may want to begin by first reviewing [Arduino's Guide to the IDE](https://www.arduino.cc/en/Guide/Environment) (and maybe try out a few Arduino examples) before proceeding to program with HomeSpan.

1. The Arduino IDE comes with built-in compilers and support for a variety of Arduino boards, such as the Arduino Uno and Arduino Nano.  However, the IDE does not natively support programming for the ESP32.  Fortunately, the IDE allows you to download and install *cores* for other micro-controllers, including the ESP32, so  they can be programming from within the Arduino environment.  See the [Espressif Arduino-ESP32 GitHub page](https://github.com/espressif/arduino-esp32#arduino-core-for-esp32-wifi-chip) for complete instructions on how to install the Arduino-ESP32 core into the Arduino IDE using the [Arduino IDE Board Manager](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md).  



## HomeSpan Tutorials ##

*in progress*

## Resources and Documentation ##

*in progress*

[End-User Guide](docs/UserGuide.md)

