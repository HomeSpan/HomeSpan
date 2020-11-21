# HomeSpan Overview

*Note: This page references a lot of HomeKit HAP terminology.  If you are new to HomeKit development, you may want to start by first reviewing the [HomeKit Primer](HomeKitPrimer.md) page.*

HomeSpan is an Arduino-style C++ library designed for ESP32 devices to take on the role of a *HAP Accessory Server* (i.e. a HomeKit Device), which allows the device to be paired to, and communicate with, any *HAP Client*, such as Apple's Home App on an iPhone, iPad, or Mac.

There are three primary components of HomeSpan:

1. **The HomeSpan API** - a collection of objects, methods, and functions you implement inside an Arduino-style sketch to create your HomeKit device.

1. **The HomeSpan Command-Line Interface (CLI)** - a series of diagnostics that HomeSpan outputs to the Arduino Serial Monitor, and a series of commands you can enter into the Serial Monitor to request more diagnostic information, to perform some basic housekeeping functions (such as a Factory Reset), and to configure the device with WiFi Credentials and a HomeKit Setup Code.

1. **An End-User Environment** - allows an end-user to configure a standalone HomeSpan device using only a simple Control Button and the device's Status LED

### Overview of the HomeSpan API

The basic structure of a HomeSpan sketch is as follows:

```C++
#include "HomeSpan.h"         // include the HomeSpan library

void setup() {     
 
  Serial.begin(115200);       // start the Serial interface
  
  /// HOMESPAN SETUP CODE GOES HERE ///
  
} // end of setup()

void loop(){

  /// HOMESPAN LOOP CODE GOES HERE ///

} // end of loop()
```

Inclusion of HomeSpan.h creates a global object called `homeSpan` that implements a variety of methods.  The two most important are `begin()` and `poll()`.  The `begin()` method, which takes a number of optional parameters, initializes HomeSpan and is placed near the begining of the `setup()` section.  The `poll()` method, which take no arguments, is placed inside `loop()` and is what causes HomeSpan to run all its code.  This is generally the only function placed in the `loop()` section.  Our sketch now looks like this:

```C++
#include "HomeSpan.h"         // include the HomeSpan library

void setup() {     
 
  Serial.begin(115200);       // start the Serial interface
  
  homeSpan.begin();           // initialize HomeSpan
  
  /// CONFIGURATION OF ACCESSORY ATTRIBUTE DATABASE GOES HERE ///
  
} // end of setup()

void loop(){

 homeSpan.poll(); 

} // end of loop()
```


Need to discuss micro-controller usage to connect to real-world devices (not to connect to other web servers)

Need to discuss control button and status led

Need to discuss configuring using the CLI and the WiFi Access Point

Overview of programming paradigm

Point to the tutorials

Point to the HomeSpan Library Reference

