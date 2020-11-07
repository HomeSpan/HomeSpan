# HomeSpan
*HomeKit for the Arduino-ESP32!*

Welcome to HomeSpan - a robust and extremely easy-to-use Arduino library for creating your own [ESP32-based](https://www.espressif.com/en/products/modules/esp32) HomeKit devices entirely within the [Arduino IDE](http://www.arduino.cc).

HomeSpan provides a microcontroller-focused implementation of [Apple's HomeKit Accessory Protocol Specification (HAP) Release R2](https://developer.apple.com/support/homekit-accessory-protocol/) designed specifically for the Espressif ESP32 microcontroller running within the Arduino IDE.  HomeSpan pairs directly to HomeKit via your home WiFi network without the need for any external bridges or components.  With HomeSpan you can use the full power of the ESP32's I/O functionality to create custom control software and/or hardware to automatically operate external devices from the Home App on your iPhone, iPad, or Mac, or with Siri.

#### HomeSpan features:

* 100% HAP R2 Compliance
* 38 fully-implemented [HomeKit Services](docs/ServiceList.md)
* Operates in Accessory or Bridge mode

#### For the HomeSpan Developer:

* Extensive log outputs to the Arduino Serial Monitor
* Debugging and configuration control commands via the Arduino Serial Monitor
* Extensive error checking and HomeKit developer diagnostics
* Integrated PWM functionality
* Integrated Push Button functionailty supporting single, double, and long presses 
* Integrated access to ESP-32's Remote Control functionality used to generate IR and RF signals
* 16 detailed tutorial-examples wirth extensive comments, instructions, and HomeKit tips and tricks

#### For the HomeSpan End-User:

* Embedded WiFi Access Point and Web Interface to allow end-users (non-developers) to:
  * Setup Homespan with their own home WiFi Credentials
  * Create their own HomeKit Pairing Setup Code
* Status LED and Control Button to allow end-users to:
  * Unpair the device from HomeKit
  * Perform a Factory Reset
  * Launch the WiFi Access Point
* A 10-page User Guide

## Latest Updates ##

#### 11/3/2020

* HomeSpan 1.0.0 is the initial public release. It is 100% feature-complete and includes 16 very detailed example-tutorials providing step-by-step instructions on how to use HomeSpan to create your own HomeKit devices. A 10-page User Guide provides addtional details on how to configure your finished HomeKit device for use on your WiFi network, and how to pair it to HomeKit.

## Getting Started ##

_Complete documentation coming soon..._

[User Guide](docs/UserGuide.md)

