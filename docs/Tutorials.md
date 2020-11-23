# HomeSpan Tutorials

*(editing in progress)*

### [Example 1 - SimpleLightBulb](../examples/01-SimpleLightBulb/01-SimpleLightBulb.ino)
This first example introduces the HomeSpan library and demonstrates how to implement a simple on/off light control using a combination of HomeSpan Accessory, Service, and Characteristic objects.  Once this sketch has been uploaded to your HomeSpan device and the device is paired to your home, a new "lightbulb" tile will appear in the Home App of your iPhone, iPad, or Mac. Though the tile will be fully operational (i.e. you can change the status of the lightbulb from "on" or "off"), we won't yet connect an actual light or LED to the HomeSpan device, so nothing real will light up.  Instead, in this and the next few examples, we'll focus on learning about the different ways HomeKit controls can be configured.  Starting in Example 5, we'll connect an LED to the device and introduce the methods that actually turn the LED on and off from your Home App.  HomeSpan API topics covered in this example include:

* the `homeSpan` global object, and it's `begin()` and `poll()` methods
* referencing HomeSpan categories defined in the `Categories::` namespace
* instantiating a new `SpanAccessory`
* instantiating HomeSpan Services and Characteristics defined in the `Service::` and 'Characteristic::` namespaces

### [Example 2 - TwoSimpleLightBulbs](../examples/02-TwoSimpleLightBulbs/02-TwoSimpleLightBulbs.ino)
Example 2 expands on Example 1 by implementing two LightBulbs, each as their own Accessory.

### [Example 3 - CeilingFanWithLight](../examples/03-CeilingFanWithLight/03-CeilingFanWithLight.ino)
Example 3 shows how adding multiple Services to a single Accessory allows us to create a multi-featured Accessory, such as a ceiling fan wih a ceiling light.

### [Example 4 - AdvancedCeilingFan](../examples/04-AdvancedCeilingFan/04-AdvancedCeilingFan.ino)
Example 4 expands on Example 3 by adding Characteristics to set fan speed, fan rotation direction, and light brightness.  New HomeSpan API topics covered in this example include:

* Using `SpanRange()` to set the allowable range and increment values for a Characteristic

### [Example 5 - AdvancedCeilingFan](../examples/04-AdvancedCeilingFan/04-AdvancedCeilingFan.ino)

---

[↩️](README.md) Back to the Welcome page
