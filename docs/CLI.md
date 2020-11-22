# The HomeSpan Command-Line Interface (CLI)

HomeSpan includes a light-weight Command-Line Interface (CLI) for developers that can be accessed via the Arduino Serial Monitor whenever your Homespan device is connected to a computer by selecting *Tools → Serial Monitor* from the top menu bar of the Arduino IDE.  The HomeSpan CLI allows you view real-time HomeSpan diagnostics, query the device's operating status, inspect its HAP database, and perform some basic functions, such as initiating a Factory Reset.  Most importantly, the CLI can be used to configure HomeSpan's network connectivity and its HomeKit Setup Code.

#### Startup Diagnostics

At startup, HomeSpan displays a welcome message, provides some general information about the device, and outputs information about the Accessories, Services, and Characteristics you've instantiated in the sketch to create your HAP Accessory Attribute Database.  If there are any errors with how you constructed the HAP Database, HomeSpan will report them and then halt the program.

If there are no errors, HomeSpan will next provide information about the Accessory ID of your device (or create one if this is a new device), the IDs of any controllers that have been paired to your device (if any), and the long-term public keys (LTPK) associated with each device.  Under normal operations, you will not need any of this information.  Note that this data is stored in a special Non-Volatile Storage (NVS) partition in the ESP32's flash memory and is therefore retained even if the device is re-programmed.  See the HomeSpan NVS section below for details.

Lastly, HomeSpan will report an Accessory configuration number.  This number is also stored in NVS and is incremented every time you make a change to the construction of your HAP Database in the HomeSpan sketch.  For example, adding a new Service or changing the initial value of a Characteristic will result in a new configuration number.  Changing any other portion of your sketch does not cause the configuration number to be incremented.  This value is broadcast by HomeSpan to all HomeKit controllers so that the controllers supposedly know when to ask for a full refresh of the the HAP Database from your device.  HomeKit controllers unfortunately do not always poll for this information, which can lead to a mismatch between the tiles that appear in your Home App, and what your HomeSpan device actually contains.  See the [Tutorials](Tutorials.md) page for how to resolve this problem if it arises.

#### WiFi Connectivity



#### WiFi Credentials and HomeKit Setup Codes

Though HomeSpan devices can be used on a standalone basis, to control the a HomeSpan device through Apple HomeKit (which is the whole point of HomeSpan), the device needs to be connected to a WiFi network.  This means HomeSpan needs to know your home WiFi network name and WiFi password. HomeSpan refers to these are your *WiFi Credentials*.  Rather then requiring you to hardcode your WiFi Credentials as part of every HomeSpan sketch (which is neither secure nor easy to update), HomeSpan stores your WiFi Credentials in a non-volatile storage (NVS) partition in the ESP32 that is reserved as part of the flash memory on the device, similar to how an EEPROM would operate.  You can set, change, and even erase, the WiFi Credentials stored on any HomeSpan device directly from the HomeSpan CLI without ever modifying the code, or even having access to the code.

Every HomeSpan device also requires an 8-digit Setup Code to be able to pair to Apple HomeKit.  This code is similarly stored in an NVS partition rather than hardcoded into a HomeSpan sketch.  When HomeSpan is run for the first time on a new device, it configures itself with a default code of **466-37-72**.  This can be changed any time from the HomeSpan CLI.
