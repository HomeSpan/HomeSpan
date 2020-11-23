# HomeSpan Tutorials

*(editing in progress)*

### [Example 1 - SimpleLightBulb](../examples/01-SimpleLightBulb/01-SimpleLightBulb.ino)
This first example introduces the HomeSpan library and demonstrates how to implement a simple on/off light control using a combination of HomeSpan Accessory, Service, and Characteristic objects.  Once this sketch has been uploaded to your HomeSpan device and the device is paired to your home, a new "lightbulb" tile will appear in the Home App of your iPhone, iPad, or Mac. Though the tile will be fully operational (i.e. you can change the status of the lightbulb from "on" or "off"), we won't yet connect an actual light or LED to the HomeSpan device, so nothing real will light up.  Instead, in this and the next few examples, we'll focus on learning about the different ways HomeKit controls can be configured.  Starting in Example 5, we'll connect an LED to the device and introduce the methods that actually turn the LED on and off from your Home App.  HomeSpan API topics covered in this example include:

* the `homeSpan` global object, and it's `begin()` and `poll()` methods
* referencing HomeSpan categories defined in the `Categories::` namespace
* instantiating a new `SpanAccessory`
* instantiating HomeSpan Services and Characteristics defined in the `Service::` and `Characteristic::` namespaces

### [Example 2 - TwoSimpleLightBulbs](../examples/02-TwoSimpleLightBulbs/02-TwoSimpleLightBulbs.ino)
Example 2 expands on Example 1 by implementing two LightBulbs, each as their own Accessory.

### [Example 3 - CeilingFanWithLight](../examples/03-CeilingFanWithLight/03-CeilingFanWithLight.ino)
Example 3 shows how adding multiple Services to a single Accessory allows us to create a multi-featured Accessory, such as a ceiling fan wih a ceiling light.

### [Example 4 - AdvancedCeilingFan](../examples/04-AdvancedCeilingFan/04-AdvancedCeilingFan.ino)
Example 4 expands on Example 3 by adding Characteristics to set fan speed, fan rotation direction, and light brightness.  New HomeSpan API topics covered in this example include:

* using `SpanRange()` to set the allowable range and increment values for a Characteristic

### [Example 5 - WorkingLED](../examples/05-WorkingLED/05-WorkingLED.ino)
Example 5 expands on Example 2 by adding in the code needed to actually control LEDs connected to the ESP32 from HomeKit. In Example 2 we built out all the functionality to create a "Tile" Acessories inside HomeKit that displayed an on/off light, but these control did not actually operate anything on the ESP32.  To operate actual devices HomeSpan needs to be programmed to respond to "update" requests from HomeKit by performing some form of operation.  New HomeSpan API topics covered in this example include:

* creating derived device-specific Service structures (classes) from a base HomeSpan Service class
* placing derived Service classes in their own \*.h files for readability and portability
* implementing the virtual `update()` method for your derived Services
* saving references to Characteristic objects with a `SpanCharacteristic *` pointer
* retrieving new and updated Characteristic values with the `getVal()` and `getNewVal()` methods

### [Example 6 - DimmableLED](../examples/06-DimmableLED/06-DimmableLED.ino)
Example 6 changes Example 5 so that LED #2 is now dimmable, instead of just on/off.  New HomeSpan API topics covered in this example include:

* implementing pulse-width-modulation on any ESP32 pin by instantiating a `PwmPin()` object
* setting the PWM level to control the brightness of an LED using the PwmPin `set()` method
* storing similar derived Service classes in the same \*.h file for ease of use

### [Example 7 - IdentifyRoutines](../examples/07-IdentifyRoutines/07-IdentifyRoutines.ino)
Example 7 uses the encapsulation techniques illustrated in Examples 5 and 6 to derive an easier-to-use Identify Service from HomeSpan's AccessoryInformation Service.  The example includes the implementation of an `update()` method that responds to HomeKit requests writing to the Identify Characteristic.  New HomeSpan API topics covered in this example include:

* storing dissimilar derived Service classes in the different \*.h files for better portability

### [Example 8 - Bridges](../examples/08-Bridges/08-Bridges.ino)
Example 8 is functionally identical to Example 7, except that instead of defining two Accessories (one for the on/off LED and one for the dimmable LED), we define three Accessories, where the first acts as a HomeKit Bridge.

---

[↩️](README.md) Back to the Welcome page
