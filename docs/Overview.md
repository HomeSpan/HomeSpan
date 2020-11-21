# HomeSpan Overview

*Note: This page references a lot of HomeKit HAP terminology.  If you are new to HomeKit development, you may want to start by first reviewing the [HomeKit Primer](HomeKitPrimer.md) page.*

HomeSpan is an Arduino-style C++ library designed for ESP32 devices to take on the role of a *HAP Accessory Server* (i.e. a HomeKit Device), which allows the device to be paired to, and communicate with, any *HAP Client*, such as Apple's Home App on an iPhone, iPad, or Mac.

There are three primary components of HomeSpan:

1. **The HomeSpan API** - a collection of objects, methods, and functions you implement inside an Arduino-style sketch to create your HomeKit device.

1. **The HomeSpan Command-Line Interface (CLI)** - a series of diagnostics that HomeSpan outputs to the Arduino Serial Monitor, and a series of commands you can enter into the Serial Monitor to request more diagnostic information, to perform some basic housekeeping functions (such as a Factory Reset), and to configure the device with WiFi Credentials and a HomeKit Setup Code.

1. **An End-User Environment** - allows an end-user to configure a standalone HomeSpan device using only a simple Control Button and the device's Status LED

# Overview of the HomeSpan API

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
  
  /// DEFINITION OF HAP ACCESSORY ATTRIBUTE DATABASE GOES HERE ///
  
} // end of setup()

void loop(){

 homeSpan.poll(); 

} // end of loop()
```

## Creating the HAP Accessory Attribute Database

The next step is to implement the code that defines the HAP Accessory Attribute Database, which is not really a database but simply a list of all HAP accessory objects, Service objects, and Characteristic objects implemented by this HomeSpan device.

You create a new HAP Accessory by instantiating a new `SpanAccessory` object as follows:

```C++
new SpanAccessory();
```

SpanAccessory takes no paramaters, and you do not need to save the object in a variable since HomeSpan automatically registers the Accessory inside the homespan object.

Once a HAP Accessory is created, you can then start adding HAP Services and HAP Characteristics by instantiating HomeSpan Service and Characteristic objects.  Every HAP Service supported by HomeSpan is defined in the `Service` namespace.  Every HAP Characteristic supported by HomeSpan is defined in the `Characteristic` namespace.  See [HomeSpan Services and Characteristics](ServiceList.md) for a complete list.  

For example, to add a HAP Carbon Dioxide Sensor Service to an Accessory, simply instantiate the corresponding HomeSpan Service object as follows:

```C++
new Service::CarbonDioxideSensor();
```

HomeSpan Service objects take no parameters, and you do not need to save the object in a variable.  HomeSpan will automatically register this Service and attach it to the last Accessory you defined.

HAP Characteristics are added to a Service in a similar fashion.  For example, to add a HAP Carbon Dioxide Detected Characteristic, simply instantiate the corresponding HomeSpan Chararacteristic object as follows:

```C++
new Characteristic::CarbonDioxideDetected();
```

HomeSpan will automatically register this Characteristic and attach it to the last Service you defined.  However, unlike Accessory and Service Objects, Characteristics do take a single optional argument (not shown above) that initializes the value of the Characteristic at start-up.  If you don't specify an argument, HomeSpan will use a sensible default.

A complete HAP Accessory Attribute Database is thus defined by simply instantiating one or more SpanAccessory objects, each with one or more Service objects, and that in turn each contain one of more Characteristic objects.  For example, a HomeSpan device supporting a variable-speed, reversible ceiling fan with a non-dimmable ceiling light, and a table lamp with a dimmable lightbulb, could be implemented as follows:

```C++
#include "HomeSpan.h"         // include the HomeSpan library

void setup() {     
 
  Serial.begin(115200);       // start the Serial interface
  
  homeSpan.begin();           // initialize HomeSpan

  new SpanAccessory();        // Reversible, Variable-Speed Ceiling Fan with Non-Dimmable Ceiling Light
   new Service::Fan();       
    new Characteristic::Active();          
    new Characteristic::RotationSpeed(100);   // initialize default speed to be 100%
    new Characteristic::RotationDirection();
   new Service::LightBulb();
    new Characteristic::On();
    
  new SpanAccessory()        // Table Lamp with Dimmable Light
   new Service::LightBulb();
    new Characteristic::On();
    new Characteristic::Brightness(50);     // initializes default brightness to be 50%
  
} // end of setup()

void loop(){

 homeSpan.poll(); 

} // end of loop()
```

As you can see, you do not need to name any objects, or specify any HAP parameters, such a format types, UUID codes, etc.  However, the *order* in which you instantiate objects is critical.  Characteristics are automatically associated with the last Service instantiated, and Services are automatically associated with the last Accessory instantiated.

> HomeSpan has extensive error checking.  At start-up, HomeSpan will validate the configuration of the HAP Accessory Attribute Database you instantiated to ensure that every Accessory has all the required Services, and that each Service has all its required Characteristics.  If HomeSpan finds an Accessory is missing a required Service, a Service is missing a required Characteristic, or a Characteristic that is neither required nor optional has been added to a Service that does not support that Characteristic, HomeSpan will report these errors and halt the program.

In fact, if you tried to run the above sketch you would find it failed to validate.  That's because each Accessory is missing two required Services - the HAP Accessory Information Service, and the HAP Protcol Information Service.  See the [Tutorials](Tutorials.md) for full completed and valid configurations that include these required HAP Services, such as this complete, working example for a simple table lamp:

```C++
#include "HomeSpan.h"         // include the HomeSpan library

void setup() {     
 
  Serial.begin(115200);       // start the Serial interface
  
  homeSpan.begin();           // initialize HomeSpan

  new SpanAccessory();           // Table Lamp Accessory
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service, with 6 *required* Characteristics
      new Characteristic::Name("My Table Lamp");      // Name of the Accessory, which shows up on the HomeKit "tiles", and should be unique across Accessories                                                    
      new Characteristic::Manufacturer("HomeSpan");   // Manufacturer of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::SerialNumber("123-ABC");    // Serial Number of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::Model("120-Volt Lamp");     // Model of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::FirmwareRevision("0.9");    // Firmware of the Accessory (arbitrary text string, and can be the same for every Accessory)  
      new Characteristic::Identify();                 // Provides a hook that allows a HomeKit Client to identify the device
  
    new Service::HAPProtocolInformation();          // HAP requires every Accessory (except those in a bridfge) to implement a Protcol Information Service  
      new Characteristic::Version("1.1.0");           // Set the Version Characteristic to "1.1.0," which is required by HAP
      
    new Service::LightBulb();                       // Create the Light Bulb Service
      new Characteristic::On();                       // Characteristic that stores that state of the light bulb: ON or OFF
  
} // end of setup()

void loop(){

 homeSpan.poll(); 

} // end of loop()
```

## Connecting HomeSpan to the Real World Appliances

A HomeSpan device that has been configured properly




 


Need to discuss micro-controller usage to connect to real-world devices (not to connect to other web servers)

Need to discuss control button and status led

Need to discuss configuring using the CLI and the WiFi Access Point

Overview of programming paradigm

Point to the tutorials

Point to the HomeSpan Library Reference

