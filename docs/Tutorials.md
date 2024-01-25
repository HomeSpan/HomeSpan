# HomeSpan Tutorials

The HomeSpan library includes many tutorial sketches of increasing complexity that take you through all the functions and features of HomeSpan.  The sketches are extensively annotated, and you'll even learn a lot about HomeKit itself by working through all the examples.  If you've already loaded HomeSpan into your Arduino IDE, the tutorials will be found under *File → Examples → HomeSpan*.  Each sketch is ready to be compiled and uploaded to your ESP32 device so you can see them in action.  Alternatively, you can explore just the code within GitHub by clicking on any of titles below.  Note: you may want to first read through the [HomeSpan API Overview](Overview.md) before exploring the tutorials.  They will probably make a lot more sense if you do!

> :heavy_check_mark: Each example is designed to be operated after pairing your ESP32 to HomeKit so you can control HomeSpan from the Home App on your iPhone, iPad, or Mac.  In principle, once you configure and pair your device to HomeKit, your Home App should automatically reflect all changes in your configuration whenever you upload a different tutorial.  However, in practice this is not always the case as it seems HomeKit sometimes caches information about devices, which means what you see in your Home App may not be fully in sync with your sketch.  If this occurs, unpairing and then re-pairing the ESP32 device usually fixes the issue.  If not, you may have to reset the ID on the ESP32 device so that HomeKit thinks it is a new device and will not use any cached data.  This is very easy to do - see the [HomeSpan Command-Line Interface (CLI)](CLI.md) page for details.

### [Example 1 - SimpleLightBulb](../examples/01-SimpleLightBulb)
This first example introduces the HomeSpan library and demonstrates how to implement a simple on/off light control using a combination of HomeSpan Accessory, Service, and Characteristic objects.  Once this sketch has been uploaded to your HomeSpan device and the device is paired to your home, a new "lightbulb" tile will appear in the Home App of your iPhone, iPad, or Mac. Though the tile will be fully operational (i.e. you can change the status of the lightbulb from "on" or "off"), we won't yet connect an actual light or LED to the HomeSpan device, so nothing real will light up.  Instead, in this and the next few examples, we'll focus on learning about the different ways HomeKit controls can be configured.  Starting in Example 5, we'll connect an LED to the device and introduce the methods that actually turn the LED on and off from your Home App.  HomeSpan API topics covered in this example include:

* the `homeSpan` global object, and it's `begin()` and `poll()` methods
* referencing HomeSpan categories defined in the `Categories::` namespace
* instantiating a new `SpanAccessory`
* instantiating HomeSpan Services and Characteristics defined in the `Service::` and `Characteristic::` namespaces

### [Example 2 - TwoSimpleLightBulbs](../examples/02-TwoSimpleLightBulbs)
Example 2 expands on Example 1 by implementing two LightBulbs, each as their own Accessory.

### [Example 3 - CeilingFanWithLight](../examples/03-CeilingFanWithLight)
Example 3 shows how adding multiple Services to a single Accessory allows us to create a multi-featured Accessory, such as a ceiling fan wih a ceiling light.

### [Example 4 - AdvancedCeilingFan](../examples/04-AdvancedCeilingFan)
Example 4 expands on Example 3 by adding Characteristics to set fan speed, fan rotation direction, and light brightness.  New HomeSpan API topics covered in this example include:

* using `setRange()` to set the allowable range and increment values for a Characteristic

### [Example 5 - WorkingLED](../examples/05-WorkingLED)
Example 5 expands on Example 2 by adding in the code needed to actually control LEDs connected to the ESP32 from HomeKit. In Example 2 we built out all the functionality to create a "Tile" Acessories inside HomeKit that displayed an on/off light, but these control did not actually operate anything on the ESP32.  To operate actual devices HomeSpan needs to be programmed to respond to "update" requests from HomeKit by performing some form of operation.  New HomeSpan API topics covered in this example include:

* creating derived device-specific Service structures (classes) from a base HomeSpan Service class
* placing derived Service classes in their own \*.h files for readability and portability
* implementing the virtual `update()` method for your derived Services
* saving references to Characteristic objects with a `SpanCharacteristic *` pointer
* retrieving new and updated Characteristic values with the `getVal()` and `getNewVal()` methods

### [Example 6 - DimmableLED](../examples/06-DimmableLED)
Example 6 changes Example 5 so that LED #2 is now dimmable, instead of just on/off.  New HomeSpan API topics covered in this example include:

* implementing pulse-width-modulation (PWM) to control an LED attached to any ESP32 pin by instantiating an `LedPin()` object
* setting the brightness of an LED using the LedPin `set()` method
* storing similar derived Service classes in the same \*.h file for ease of use

### [Example 7 - AccessoryNames](../examples/07-AccessoryNames)
Example 7 demonstrates how the names of two LED Accessories created in Example 6 can be changed from the defaults generated by the Home App to something more useful and customized.

### [Example 8 - Bridges](../examples/08-Bridges)
Example 8 is functionally identical to Example 7, except that instead of defining two Accessories (one for the on/off LED and one for the dimmable LED), we define three Accessories, where the first acts as a HomeKit Bridge.

### [Example 9 - MessageLogging](../examples/09-MessageLogging)
Example 9 illustrates how to add log messages to your HomeSpan sketch.  The code is identical to Example 8 except for the inclusion of new log messages.  New HomeSpan API topics covered in this example include:

* using the `LOG0()`, `LOG1()`, and `LOG2()` macros to create log messages for different log levels
* setting the initial log level for a sketch with the `homeSpan.setLogLevel()` method

### [Example 10 - RGB_LED](../examples/10-RGB_LED)
Example 10 illustrates how to control an RGB LED to set any color and brightness.  New HomeSpan API topics covered in this example include:

* converting HomeKit Hue/Saturation/Brightness levels to Red/Green/Blue levels using `PwmPin::HSVtoRGB()`
* using the optional template functionality of `getVal()`, such as `getVal<float>()`

### [Example 11 - ServiceNames](../examples/11-ServiceNames)
Example 11 demonstrates how the names of the different Services in a multi-Service Accessory can be changed from the defaults generated by the Home App to something more useful and customized.  The examples also explores how and when these names are displayed by the Home App, as well as how the Home App chooses an appropriate icon for an Accessory Tile when the device is configured as a Bridge.

### [Example 12 - ServiceLoops](../examples/12-ServiceLoops)
Example 12 introduces HomeKit *Event Notifications* to implement two new accessories - a Temperature Sensor and an Air Quality Sensor.  Of course we won't actually have these physical devices attached to the ESP32 for the purpose of this example, but we will simulate "reading" their properties on a periodic basis, and notify HomeKit of any changed values.  New HomeSpan API topics covered in this example include:

* implementing the virtual `loop()` method in a derived Service
* keeping track of elapsed time since the last update of a Characteristic with the `timeVal()` method
* setting the value of a Characteristic and triggering an Event Notification with the `setVal()` method

### [Example 13 - TargetStates](../examples/13-TargetStates)
Example 13 we demonstrate the simultaneous use of both the `update()` and `loop()` methods by implementing two new Services: a Garage Door Opener and a motorized Window Shade.  Both examples showcase HomeKit's Target-State/Current-State framework.

### [Example 14 - EmulatedPushButtons](../examples/14-EmulatedPushButtons)
Example 14 demonstrates how you can use the `setVal()` and `timeVal()` methods inside a Service's `loop()` method to create a tile in the Home App that emulates a pushbutton switch.  In this example pressing the tile in the Home App will cause it to turn on, blink an LED 3 times, and then turn off (just like a real pushbutton might do).

### [Example 15 - RealPushButtons](../examples/15-RealPushButtons)
This example introduces HomeSpan functionality that lets you easily connect real pushbuttons to any pin on your ESP32 device.  These pushbuttons can then be used to  manually control any appliance connected to the device, such as a lamp or fan.  In this example we implement 3 pushbuttons to control the power, brightness, and a "favorites" setting of an LED, using a combination of single, double, and long button presses.  Event Notifications are sent back to HomeKit using the `setVal()` method after each pushbutton press so that the Home App tiles immediately reflect your manual changes to the power and brightness of the LED.  New HomeSpan API topics covered in this example include:

* creating pushbutton objects on any ESP32 pin with `SpanButton()`
* implementing the virtual `button()` method in a derived Service
* parsing SINGLE, DOUBLE, and LONG button presses

### [Example 16 - ProgrammableSwitches](../examples/16-ProgrammableSwitches)
Example 16 does not introduce any new HomeSpan functionality, but instead showcases a unique feature of HomeKit that you can readily access with HomeSpan.  In all prior examples we used the ESP32 to control a local appliance - something connected directly to the ESP32 device.  We've then seen how you can control the device via HomeKit's iOS or MacOS Home App, or by the addition of local pushbuttons connected directly to the ESP32 device. In this example we do the opposite, and use pushbuttons connected to the ESP32 to control OTHER HomeKit devices of any type.  To do so, we use HomeKit's Stateless Programmable Switch Service.

### [Example 17 - LinkedServices](../examples/17-LinkedServices)
Example 17 introduces the HAP concept of Linked Services and demonstrates how they are used through the implementation of a multi-head Shower.  This example also illustrates some different coding styles that showcase the power and flexibility of HomeSpan's C++ *structure-based* design paradigm.  New HomeSpan API topics covered in this example include:

* creating Linked Services using the `addLink()` method

### [Example 18 - SavingStatus](../examples/18-SavingStatus)
Example 18 demonstrates, through the implementation of two Dimmable LEDs, how the values of Characteristics can be automatically saved in the device's non-volatile storage (NVS) for restoration upon start-up if the device is loses power.   New HomeSpan API topics covered in this example include:

* instructing HomeSpan to store the value of a Characteristic in NVS by setting the second parameter of the constuctor to `true`

### [Example 19 - WebLog](../examples/19-WebLog)
Example 19 illustrates, through the implementation of two On/Off LEDs, how to add a Web Log to your HomeSpan sketch, how to syncronize the internal clock of your device using an NTP time server, and how to create your own Web Log messages.  New HomeSpan API topics covered in this example include:

* enabling the HomeSpan Web Log and specifying an optional NTP time server with the `homeSpan.enableWebLog()` method
* using the `WEBLOG()` macro to create Web Log messages

### [Example 20 - AdvancedTechniques](../examples/20-AdvancedTechniques)
Example 20 illustrates a number of advanced techniques through the implementation of a "dynamic" bridge that allows Light Accessories to be *interactively* added and deleted at any time without the need to reboot the device.  New HomeSpan API topics covered in this example include:

* creating custom CLI commands using `SpanUserCommand()`
* dynamically deleting Accessories with `homeSpan.deleteAccessory()`
* refreshing the Accessory database (which automatically updates the Home App) using `homeSpan.updateDatabase()`
* using `homeSpan.autoPoll()` to implement HomeSpan Polling in the background (and on the second core, if available)
 
## Other Examples

The following examples showcase a variety of HomeSpan and HomeKit functionality as referenced in different sections of the HomeSpan documentation.  The sketches can be found in the Arduino IDE under *File → Examples → HomeSpan → Other Examples*

### [TableLamp](../examples/Other%20Examples/TableLamp)
A basic implementation of a Table Lamp Accessory. Used as the tutorial in [HomeSpan API Overview](Overview.md)

### [RemoteControl](../examples/Other%20Examples/RemoteControl)
A standalone example that shows how to use HomeSpan's *RFControl* class to produce a custom pulse train.  For illustrative purposes the pulse widths are very long and suitable for output to an LED so you can "see" the pulse train. See the [RF/IR Generation](RMT.md) page for full details

### [ServoControl](../examples/Other%20Examples/ServoControl)
An implementation of a Window Shade that uses HomeSpan's *ServoPin* class to control the horizontal tilt of the slats. See [ServoPin](PWM.md#servopinuint8_t-pin-double-initdegrees-uint16_t-minmicros-uint16_t-maxmicros-double-mindegrees-double-maxdegrees) for full details

### [Television](../examples/Other%20Examples/Television)
An example of HomeKit's *undocumented* Television Service showing how different Characteristics can be used to control a TV's power, input sources, and a few other functions.  See the [Television Services and Characteristics](TVServices.md) page for full details

### [Pixel](../examples/Other%20Examples/Pixel)
Demonstrates how to use HomeSpan's *Pixel* and *Dot* classes to control one- and two-wire Addressable RGB and RGBW LEDs.  See the [Addressable RGB LEDs](Pixels.md) page for full details

### [CustomService](../examples/Other%20Examples/CustomService)
Demonstrates how to create Custom Services and Custom Characteristics in HomeSpan to implement an Atmospheric Pressure Sensor recognized by the *Eve for HomeKit* app.  See [Custom Characteristics and Custom Services Macros](Reference.md#custom-characteristics-and-custom-services-macros) for full details
 
### [ProgrammableHub](../examples/Other%20Examples/ProgrammableHub)
Demonstrates how to implement a fully programmable Light Accessory Hub that allows the user to *dynamically* add/delete up to 12 Light Accessories directly through a device-hosted *web interface* or via HomeSpan's *command-line inteface*.  Each light can be configured as dimmable/non-dimmable with either no color control, full RGB color control, or color-temperature control.  Builds upon many of the techniques used in [Example 20](../examples/20-AdvancedTechniques)

### [RemoteSensors](../examples/Other%20Examples/RemoteSensors)
Demonstrates how *SpanPoint* can be used to transmit messages from battery-powered Remote Devices running light-weight sketches that measure the local temperature, to a wall-powered Main Device running a full HomeSpan sketch implementing Temperature Sensor Accessories.  See [SpanPoint: Point-to-Point Communication between ESP32 Devices](NOW.md) for full details regarding the *SpanPoint* class and all of its methods

### [FadingLED](../examples/Other%20Examples/FadingLED)
Demonstrates how the *LedPin* class can use the ESP32's built-in fading control to automatically fade an LED from from one level of brightness to another over a specified period of time. See the [LedPin](PWM.md#pulse-width-modulation-pwm) page for full details

### [MotorizedWindowShade](../examples/Other%20Examples/MotorizedWindowShade)
Demonstrates how to use the *StepperControl* class to operate a stepper motor.  Implements a motorized window shade based on [Example&nbsp;13](../examples/13-TargetStates) above. See the [Stepper Motor Control](Stepper.md) page for full details

### [CustomNVSPartition](../examples/Other%20Examples/CustomNVSPartition)
Demonstrates how to create a Custom Partition Scheme for your sketch by adding a *partitions.csv* file to your sketch folder.  Can be used to expand the size of the non-volatile-storage (NVS) partition, which may be needed when creating a HomeSpan device with many Accessories whose Characteristics you want to save in NVS

### [ExternalReference](../examples/Other%20Examples/ExternalReference)
Demonstrates how to access Characteristics of Services from outside those Services, such as from within the main Arduino `loop()`.  In this sketch we re-create the two LEDs in Example 5 with an added function in the main Arduino `loop()` that checks if both LEDs are on at the same time, and if so, they are automatically turned off

---

[↩️](../README.md) Back to the Welcome page
