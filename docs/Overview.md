# HomeSpan API Overview

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

Note that as an *alternative*, you can intruct HomeSpan to create separate task that repeatedly calls `homeSpan.poll()` in the background.  To do so, **replace** the call to `homeSpan.poll()` in the main `loop()` with a call to  `homeSpan.autoPoll()` at the end of the `setup()` function:

```C++
#include "HomeSpan.h"         // include the HomeSpan library

void setup() {     
 
  Serial.begin(115200);       // start the Serial interface
  
  homeSpan.begin();           // initialize HomeSpan
  
  /// DEFINITION OF HAP ACCESSORY ATTRIBUTE DATABASE GOES HERE ///
  
 homeSpan.autoPoll();         // start a task that repeatedly calls `homeSpan.poll()` in the background

} // end of setup()

void loop(){

} // end of loop()
```

This is particularly efficient when using dual-core processors since HomeSpan will run the polling task on the "free" processor that is otherwise not performing any other Arduino functions.

## Creating the HAP Accessory Attribute Database

The next step is to implement the code that defines the HAP Accessory Attribute Database, which is not really a database but simply a list of all HAP accessory objects, Service objects, and Characteristic objects implemented by this HomeSpan device.

You create a new HAP Accessory by instantiating a new `SpanAccessory` object as follows:

```C++
new SpanAccessory();
```

SpanAccessory can be instantiated without any parameters, and you do not need to save the object in a variable since HomeSpan automatically registers the Accessory inside the `homespan` object.

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

> :heavy_check_mark: HomeSpan has extensive error checking.  At start-up, HomeSpan will validate the configuration of the HAP Accessory Attribute Database you instantiated to ensure that every Accessory has all the required Services, and that each Service has all its required Characteristics.  If HomeSpan finds an Accessory is missing a required Service, a Service is missing a required Characteristic, or a Characteristic that is neither required nor optional has been added to a Service that does not support that Characteristic, HomeSpan will report these errors and halt the program.

In fact, if you tried to run the above sketch you would find it failed to validate.  That's because each Accessory is missing a required Service and Characteristic - the HAP Accessory Information Service and the Identify Characteristic.  See the [Tutorials](Tutorials.md) for a variety of complete and valid examples that include all required HAP elements, such as this sketch for a simple table lamp:

```C++
/* HomeSpan Table Lamp Example */

#include "HomeSpan.h"         // include the HomeSpan library

void setup() {     
 
  Serial.begin(115200);       // start the Serial interface
  
  homeSpan.begin();           // initialize HomeSpan

  new SpanAccessory();           // Table Lamp Accessory
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service  
      new Characteristic::Identify();               // HAP requires the Accessory Information Service to include the Identify Characteristic
        
    new Service::LightBulb();                       // Create the Light Bulb Service
      new Characteristic::On();                       // Characteristic that stores that state of the light bulb: ON or OFF
  
} // end of setup()

void loop(){

 homeSpan.poll(); 

} // end of loop()
```

## Connecting HomeSpan to Real World Appliances

The above Table Lamp example is a fully functioning HomeSpan sketch.  If you upload it to your ESP32 device and pair the device to HomeKit, your will find a new tile appears in your Home App labeled "My Table Lamp."  The tile is operational.  Press once and it shows the light turning on.  Press a second time and it shows the light turning off.  Of course nothing happens in the real world - HomeSpan has not yet been programmed to turn on an actual lamp.

When you press a tile in the Home App, HomeKit sends a request to HomeSpan to *update* one or more Characteristics.  In the example above, pressing the tile causes HomeKit to specifically request that the On Characteristic of the LightBulb Service be updated to either true (on) or false (off).  When HomeSpan receives a request to update one or more Characteristics, rather than sequentially call code to separately update each Characteristic, HomeSpan instead groups the requests according to their Service, and calls the `update()` method for that Service.  This is because many Services support multiple Characteristic that are physcially related.  For example, turning on a light and setting its brightness to 50% requires the updating of two Characteristic (On and Brightness).  In systems where these are handled sequentially, the user is required to write complicated code to save intermediate states and determine what to do when it receives a request to turn on a light if does not yet know whether or not a request to set the brightness will shortly follow.

The *Service-Centric* approach used by HomeSpan makes it much easier to implement the code to handle updates.  Instead of writing code to respond to updates of individual Characteristics, you write code to respond to an update of an entire Service, with methods that inform you of which Characteristics were requested to be updated, and their new corresponding values.

Every HomeSpan Service implements a virtual `update()` method that by default does nothing.  To implement your own logic, you override the `update()` with your own code.  The easiest way to do this is by creating a new Service that is derived from the Service you want to customize with your own `update()` method.  For example, we can derive a new service called TableLamp from LightBulb as follows:

```C++
struct TableLamp : Service::LightBulb {};
```

Within this new structure we can create our own constructor, potentially with one or more arguments.  The constructor can store some device-specific parameters, such as the number of the ESP32 pin that will be used to drive the relay that turns on and off the table lamp.  The constructor should also define all the Characteristics required by the Service.  You will also want to save at least some Characteristic objects as named variables so they can be referenced later in the `update()` method.  The base class for all HomeSpan Characteristics is `SpanCharacteristic`, so creating a variable to store a HomeSpan Characteristic requires you to define it as ``SpanCharacteristic *``.  Putting it all together, we have, so far:

```C++
struct TableLamp : Service::LightBulb{

  int lampPin;                               // store the pin number connected to a hypothetical relay that turns the Table Lamp on/off
  SpanCharacteristic *lampPower;             // store a reference to the On Characteristic
  
  TableLamp(int lampPin) : Service::LightBulb(){       // constructor() method for TableLamp defined with one parameter.  Note we also call the constructor() method for the LightBulb Service.

    lampPower=new Characteristic::On();      // instantiate the On Characteristic and save it as lampPower
    this->lampPin=lampPin;                   // save the pin number for the hypothetical relay
    pinMode(lampPin,OUTPUT);                 // configure the pin as an output using the standard Arduino pinMode function                       
    
  } // end constructor
};
```

Now we are ready to implement our `update()` method.  Though `update()` takes no arguments, it does have a boolean return value, which lets HomeKit know whether its request to update the Table Lamp was successful (true) or unsuccessful (false).  Within the `update()` method you'll have access to all the Characteristics in the Service, which is this case is just the On Characterisrtic we named lampPower.  HomeSpan Characteristics implement a variety of methods that provide information on the status of the Characteristics, whether there is a request that they be updated, and what the update value should be.  Since our TableLamp Service only defines a single Characteristic that means whenever `update()` is called there must have been a request by HomeKit to update the On Characteristic and we do not have to check to see if it is being updated.  However, we do need to know the value that HomeKit is requesting.  To retrieve this value we use the getNewVal() method as such: `lampPower->getNewVal()`.  This returns the new value for the On Characteristic requested by HomeKit.  This will either be true (1) or false (0) depending on whether HomeKit would like the light turned on or off.  We can thus use this value to update the lampPin accordingly.  Our complete TableLamp Service now looks like this:

```C++
struct TableLamp : Service::LightBulb{

  int lampPin;                               // store the pin number connected to a hypothetical relay that turns the Table Lamp on/off
  SpanCharacteristic *lampPower;             // store a reference to the On Characteristic
  
  TableLamp(int lampPin) : Service::LightBulb(){       // constructor() method for TableLamp defined with one parameter.  Note we also call the constructor() method for the LightBulb Service.

    lampPower=new Characteristic::On();      // instantiate the On Characteristic and save it as lampPower
    this->lampPin=lampPin;                   // save the pin number for the hypothetical relay
    pinMode(lampPin,OUTPUT);                 // configure the pin as an output using the standard Arduino pinMode function                       
    
  } // end constructor()
  
  boolean update(){                          // update() method

    digitalWrite(lampPin,lampPower->getNewVal());      // use standard Arduino digitalWrite function to change the ledPin to either high or low based on the value requested by HomeKit
   
    return(true);                            // return true to let HomeKit (and the Home App Client) know the update was successful
  
  } // end update()
  
};
```

Finally, we prepend our derived TableLamp Service to the original sketch and replace the instantiation of our generic LightBulb Service and On Characteristic:

```C++
new Service::LightBulb();                       // Create the Light Bulb Service
  new Characteristic::On();                       // Characteristic that stores that state of the light bulb: ON or OFF
```

with the instantiation of the TableLamp Service as such:

```C++
new TableLamp(17);
```

where 17 specifies the ESP32 pin number that is connected to the hypothetical relay used to turn the Table Lamp on and off.

Our completed HomeSpan sketch to operate a Table Lamp is presented in full below.  Note it is also available in the Arduino IDE under *File → Examples → HomeSpan → Other Examples → TableLamp*.

```C++
/* HomeSpan Table Lamp Example */

#include "HomeSpan.h"         // include the HomeSpan library

//// Define the TableLamp Service ////

struct TableLamp : Service::LightBulb{

  int lampPin;                               // store the pin number connected to a hypothetical relay that turns the Table Lamp on/off
  SpanCharacteristic *lampPower;             // store a reference to the On Characteristic
  
  TableLamp(int lampPin) : Service::LightBulb(){       // constructor() method for TableLamp defined with one parameter.  Note we also call the constructor() method for the LightBulb Service.

    lampPower=new Characteristic::On();      // instantiate the On Characteristic and save it as lampPower
    this->lampPin=lampPin;                   // save the pin number for the hypothetical relay
    pinMode(lampPin,OUTPUT);                 // configure the pin as an output using the standard Arduino pinMode function                       
    
  } // end constructor()
  
  boolean update(){                          // update() method

    digitalWrite(lampPin,lampPower->getNewVal());      // use standard Arduino digitalWrite function to change the ledPin to either high or low based on the value requested by HomeKit
   
    return(true);                            // return true to let HomeKit (and the Home App Client) know the update was successful
  
  } // end update()
  
};

//// INITIALIZE HOMESPAN AND CREATE THE HAP ACCESSORY ATTRIBUTE DATABASE ////

void setup() {     
 
  Serial.begin(115200);       // start the Serial interface
  
  homeSpan.begin();           // initialize HomeSpan

  new SpanAccessory();           // Table Lamp Accessory
  
    new Service::AccessoryInformation();            // HAP requires every Accessory to implement an AccessoryInformation Service  
      new Characteristic::Identify();               // HAP requires the Accessory Information Service to include the Identify Characteristic
       
   new TableLamp(17);                               // instantiate the TableLamp Service (defined below) with lampPin set to 17
  
} // end of setup()

//// RUN HOMESPAN ////

void loop(){

 homeSpan.poll(); 

} // end of loop()
```

This fully working example is ready to be uploaded to your ESP32 device where is can be used to operate a hypothetical table lamp connected by a relay to pin 17.  Alternatively, you can simply connect an LED to pin 17 and see how it works!  Congratulations, you've created your first HomeSpan Sketch.

## Multi-File Sketches

The encapsulation of all TableLamp functions into a single structure suggests that the TableLamp code could instead be stored in a separate file.  Since TableLamp represents a *device-specific* implementation of HAP's generic LightBulb Service, the recommended convention is to add DEV_ as a prefix to all such structures and place them in a \*.h file with a similar DEV_ prefix.  For example, we would change `struct TableLamp` to `struct DEV_TableLamp` and place its code in a separate file name *DEV_TableLamp.h*. To use we simply add `#include "DEV_TableLamp.h"` to our \*.ino sketch file, typically right after `#include "HomeSpan.h"`.

In this fashion, we can create a "library" of device-specific structures for real-world lamps, ceiling fans, window shades, garage door openers, or even just a series of switches that control a unique appliance.  By storing each structure (or a set of related structures) in its own DEV_\*.h file with a well-documented constructor, and by including only the device-specific structures needed for a specific HomeSpan sketch, we increase overall readability, portability and re-useability.  You'll see this convention is used throughout HomeSpan's tutorial sketches. 

# Exploring More

Though the example above is a good illustration of a basic HomeSpan sketch, it only scratches the surface of HomeSpan features.  The best way to learn about the full suite of HomeSpan functions is by exploring the [HomeSpan Tutorials](Tutorials.md), either online in GitHub, or by opening each tutorial-sketch from within the Arduino IDE.  The later method is preferred since you can also compile and upload the tutorial sketches to your device to see them in action.

In addition, you should read through the [HomeSpan API Reference](Reference.md) pages for complete details on every HomeSpan object, structure, method, and function, including some less-used functions that are not covered in any of the tutorials.

Finally, don't forget to visit the [HomeSpan Command-Line Interface (CLI)](CLI.md) page.  The HomeSpan CLI is where you'll find real-time diagnostics and be able to monitor the status of your device.  Most importantly, the HomeSpan CLI is used to configure your device with your home network's WiFi Credentials and a HomeKit Setup Code so you can pair your device to your Home.


---

[↩️](../README.md) Back to the Welcome page

