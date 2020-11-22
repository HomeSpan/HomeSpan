# The HomeSpan Command-Line Interface (CLI)

HomeSpan includes a light-weight Command-Line Interface (CLI) for developers that can be accessed via the Arduino Serial Monitor whenever your Homespan device is connected to a computer by selecting *Tools → Serial Monitor* from the top menu bar of the Arduino IDE.  The HomeSpan CLI allows you view real-time HomeSpan diagnostics, query the device's operating status, inspect its HAP database, and perform some basic functions, such as initiating a Factory Reset.  Most importantly, the CLI can be used to configure HomeSpan's network connectivity and its HomeKit Setup Code.

> When using the Serial Monitor, please make sure you set the baud rate to match whatever you specified in the `Serial.begin()` function in your HomeSpan sketch.  In addition, you'll need to set the Serial Monitor to transmit <newlines> as the line ending.

#### Startup Diagnostics

At startup, HomeSpan displays a welcome message, reminds you to set the line ending to <newline>, performs various initialization routines, provides some general information about the device, and outputs information about the Accessories, Services, and Characteristics you've instantiated in the sketch to create your HAP Accessory Attribute Database.  If there are any errors with how you constructed your HAP Database, HomeSpan will report them and halt the program. 

Next, HomeSpan checks to see if the device has been configured with WiFi Credentials.  If none are found, HomeSpan will indicate this, and then complete its initialization routine by indicating is now READY.  If WiFi Credentials are found, HomeSpan will repeatedly try to connect to the specified network until it either succeeds (in which case it then completes its initialization routine by indicating it is now READY), or you cancel the process by typing `X <return>` (in which case HomeSpan erases its stored WiFi Credentials and restarts).

#### Log Levels

In the READY state, if HomeSpan is connected to a WiFi network it will begin to listen for, and process, any incoming HomeKit HAP requests.  As each request is received and processed, HomeSpan provides diagnostic output depending on the Message Log Level, which ranges from 0 (minimal diagnostics) to 2 (hyper-detailed diganostics).  The default Message Log Level is 0, but this can be changed either in your HomeSpan sketch (see the [HomeSpan API Reference](Reference.md) for details), or during run time as described in the next section

#### HomeSpan Commands

In addition to listening for incoming HAP requests, HomeSpan also continuously polls the Serial Monitor for characters you may type.  Note that the Serial Monitor does not actually transmit the characters you type to the device until you hit <return>.  All HomeSpan commands are a single character, and HomeSpan will ignore all but the first charcacter when parsing command requests, with the exception of those commands that also include a value.  HomeSpan supports the following commands:
  
* **s** - print connection status
  * HomeSpan supports connections from more than one HomeKit Controller at the same time (the default is 8 simultaneous connection "slots").  This command provides information on all of the Controllers that have open connections to HomeSpan at any given time, and indictes which slots are currently unconnected.  If a Controller tries to connect to HomeSpan when all connection slots are already occupied, HomeSpan will terminate an existing connection and re-assign the slot the requesting Controller.
  


1. If yocharacters have been received from the Serial Monitor, process the requested command (see below) into the Serial Monitor

#### WiFi Credentials and HomeKit Setup Codes

Though HomeSpan devices can be used on a standalone basis, to control the a HomeSpan device through Apple HomeKit (which is the whole point of HomeSpan), the device needs to be connected to a WiFi network.  This means HomeSpan needs to know your home WiFi network name and WiFi password. HomeSpan refers to these are your *WiFi Credentials*.  Rather then requiring you to hardcode your WiFi Credentials as part of every HomeSpan sketch (which is neither secure nor easy to update), HomeSpan stores your WiFi Credentials in a non-volatile storage (NVS) partition in the ESP32 that is reserved as part of the flash memory on the device, similar to how an EEPROM would operate.  You can set, change, and even erase, the WiFi Credentials stored on any HomeSpan device directly from the HomeSpan CLI without ever modifying the code, or even having access to the code.

Every HomeSpan device also requires an 8-digit Setup Code to be able to pair to Apple HomeKit.  This code is similarly stored in an NVS partition rather than hardcoded into a HomeSpan sketch.  When HomeSpan is run for the first time on a new device, it configures itself with a default code of **466-37-72**.  This can be changed any time from the HomeSpan CLI.
