# The HomeSpan Command-Line Interface (CLI)

HomeSpan includes a light-weight Command-Line Interface (CLI) for developers that can be accessed via the Arduino Serial Monitor whenever your Homespan device is connected to a computer by selecting *Tools → Serial Monitor* from the top menu bar of the Arduino IDE.  The HomeSpan CLI allows you view real-time HomeSpan diagnostics, query the device's operating status, inspect its HAP database, and perform some basic functions, such as initiating a Factory Reset.  Most importantly, the CLI can be used to configure HomeSpan's network connectivity and its HomeKit Setup Code.

> When using the Serial Monitor, please make sure you set the baud rate to match whatever you specified in the `Serial.begin()` function in your HomeSpan sketch.  In addition, you'll need to set the Serial Monitor to transmit a \<newline\> as the line ending.

### Startup Diagnostics

At startup, HomeSpan:

* displays a Welcome Message,
* reminds you to set the line ending to \<newline\>,
* performs various initialization routines,
* provides some general information about the device, and
* outputs information about the Accessories, Services, and Characteristics you've instantiated in the sketch to create your HAP Accessory Attribute Database.
  
If there are any errors with how you constructed your HAP Database, HomeSpan will report them and **halt** the program. 

Next, HomeSpan checks to see if the device has been configured with WiFi Credentials.  If none are found, HomeSpan will indicate this, and then complete its initialization routine by indicating is now READY.  If WiFi Credentials are found, HomeSpan will repeatedly try to connect to the specified network until it either succeeds (in which case it then completes its initialization routine by indicating it is now READY), or you cancel the process by typing `X <return>` (in which case HomeSpan erases its stored WiFi Credentials and restarts).

### Log Levels

In the READY state, if HomeSpan is connected to a WiFi network it will begin to listen for, and process, any incoming HomeKit HAP requests.  As each request is received and processed, HomeSpan provides diagnostic output messages with different levels of detail depending on the Log Level setting.  This setting ranges from  0 (minimal diagnostics) to 2 (hyper-detailed diganostics).  The default Log Level is 0, but this can be changed either in your HomeSpan sketch (see the [HomeSpan API Reference](Reference.md) for details), or during run time as described in the next section.

### HomeSpan Commands

In addition to listening for incoming HAP requests, HomeSpan also continuously polls the Serial Monitor for characters you may type.  Note that the Serial Monitor does not actually transmit the characters you type to the device until you hit <return>.  All HomeSpan commands are a single character, and HomeSpan will ignore all but the first character when parsing command requests, with the exception of those commands that also include a value.  HomeSpan supports the following commands:
  
* **s** - print connection status
  * HomeSpan supports connections from more than one HomeKit Controller (e.g. a HomePod, or the Home App on an iPhone) at the same time (the default is 8 simultaneous connection *slots*).  This command provides information on all of the Controllers that have open connections to HomeSpan at any given time, and indictes which slots are currently unconnected.  If a Controller tries to connect to HomeSpan when all connection slots are already occupied, HomeSpan will terminate an existing connection and re-assign the slot the requesting Controller.
  
* **i** - print summary information about the HAP Database
  * This provides an outline of the device's HAP Database showing all Accessories, Services, and Characteristics you instantiated in your HomeSpan sketch, followed by a table showing whether you have overridden any of the virtual methods for each Service.  Note this output is also provided at startup after the Welcome Message as HomeSpan check the database for errors.
  
* **d** - print the full HAP Accessory Attributes Database in JSON format
  * This outputs the full HAP Database in JSON format, exactly as it is transmitted to any HomeKit device that requests it (with the exception of the newlines and spaces that make it easier to read on the screen).  Note that the value tag for each Characteristic will reflect the *current* value on the device for that Characteristic.
  
* **W** - configure WiFi Credentials and restart
  * HomeSpan sketches *do not* contain WiFi network names or WiFi passwords.  Rather, this information is separately stored in a dedicated Non-Volatile Storage (NVS) partition in the ESP32's flash memory, where it is permanently retained until updated (with this command) or erased (see below).  When HomeSpan receives this command it first scans for any local WiFi networks.  If your network is found, you can specify it by number when prompted for the WiFi SSID.  Otherwise, you can directly type your WiFi network name.  After you then type your WiFi Password, HomeSpan updates the NVS with these new WiFi Credentials, and restarts the device.
  
* **X** - delete WiFi Credentials and restart
  * This command deletes whatever WiFi Credentials have been stored in the device NVS, and restarts.
 
* **S** \<code\> - change the HomeKit Pairing Setup Code to \<code\>
  * Every HomeKit device requires a unique 8-digit Setup code used for pairing.  When HomeSpan is run for the first time on a new device it sets the HomeKit Setup Code to a default value of **466-37-72**, and stores it in a dedicated NVS partition.  This command allows you to update the stored Setup Code to any other 8-digit code.  Note that in accordance with HAP specifications, HomeSpan actually stores a hashed version of the Setup Code, rather than the Setup Code itself.  This means the actual value is not recoverable, so if you forget your Setup Code you'll need to run this command and create a new one.
  
* **A** - start the HomeSpan Setup Access Point
  * This command starts HomeSpan's temporary Access Point, which provides users with an alternate methods for configuring a device's WiFi Credentials and HomeKit Setup Code.  Starting the Access Point with this command is identical to starting it via the Control Button.  See the [HomeSpan User Guide](UserGuide.md) for complete details.
  
* **U** - unpair device by deleting all Controller data
  * This deletes all data stored about Controllers that have been paired with the device, which forces HomeSpan to reset its internal state to unpaired.  Normally, unpairing is done by HomeKit at the direction of an end-user via the Home App on an iPhone.  However, HomeKit requests to unpair a device are not subject to any confirmation from that device.  HomeKit simply assumes that once it requests a device to unpair, the device has received the message and has reset its pairing state accordingly.  In the event that HomeKit unpairs a HomeSpan device, but the device does not receive or properly process the request, its pairing status will be out of sync with HomeKit.  Forcing HomeKit to reset its internal state to unpaired using this command resolves the issue and allows HomeSpan to be re-paired with HomeKit.
  * Note that if you run this command when HomeKit thinks it is still paired to the device, pairing status will be out of sync in the opposite direction.  HomeKit Controllers will continue to send HAP requests to the device, thinking it is paired, but HomeSpan will ignore all these requests since it no longer recognizes any of the Controllers as being paired.  To resolve this issue, you must instruct HomeKit to unpair the device via the Home App, after which you can re-pair the device if needed.
  
* **H** - delete HomeKit Device ID as well as all Controller data and restart
  * In addition to deleting all Controller data (as if the 'U' command was run), this command also deletes the device's HomeKit ID.  This unique ID is broadcast to all HomeKit Controllers so the device can be uniquely recognized.  When HomeSpan first runs on a new device, it creates this unique ID and stores it permanently in an NVS partition.  Normally, this ID should not changed once set.  However, if you are actively developing and testing a HomeSpan sketch, you may find that HomeKit is cacheing information about your device and the changes you have made to your HAP Accessory Database are not always reflected in the Home App.  Sometimes simply unpairing and re-pairing the device solves this HomeKit issue.  If not, deleting your device's HomeKit ID with this command forces HomeSpan to generate a new one after restarting, which means HomeKit will think this is a completely different device, thereby ignoring any prior data it had cached.
  
* **R** - restart the device
  * This command simply reboots HomeSpan.
  
* **F** - factory reset and restart
  * This deletes all data stored in the NVS, *except* for the HomeKit Pairing Setup Code, and restarts the device.  This is effectively the same as executing the 'X' command followed by the 'H' command.
  
* **E** - erase ALL stored data and restart
  * This completely erases the NVS, deleting all stored data, *including* the HomeKit Pairing Setup code.  The device is then restarted and initialized as if it were new.
  
* **L** \<level\> - change the Log Level setting to \<level\>
  * This command is used to set the Log Level, which controls the level of diagnostic messages output by HomeSpan.  Valid values are:
  
    * 0 (minimal diagnostics),
    * 1 (all diagnostics), and 
    * 2 (all diagnostics plus a real-time stream of all HAP communication between HomeSpan and any connected HomeKit Controllers).
  
  ---

[↩️](README.md) Back to the Welcome page  
  


