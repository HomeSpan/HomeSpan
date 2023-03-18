# HomeSpan Accessory Categories

Every HomeSpan device must be assigned a HomeKit Accessory Category. HomeSpan implements these categories as C++ Classes with names that exactly match the spelling and capitalization specified by Apple in Section 13 of HAP-R2, but without any spaces.  HomeSpan Accessory Categories are defined in HomeSpan's `Category` namespace.  For example, HomeSpan defines the *Garage Door Openers* Category (HAP Category 4) as `Category::GarageDoorOpeners`, which could be used when initializing HomeSpan as follows:

```C++
homeSpan.begin(Category::GarageDoorOpeners,"Acme Garage Door Lifts");
```

The table below provides a list of all HomeSpan Categories.

### Accessory Categories

* AirConditioners
* AirPurifiers
* Bridges
* Dehumidifiers
* Doors
* Fans
* Faucets
* Lighting
* Locks
* GarageDoorOpeners
* Heaters
* Humidifiers
* IPCameras
* Other
* Outlets
* ProgrammableSwitches
* SecuritySystems
* Sensors
* ShowerSystems
* Sprinklers
* Switches
* Thermostats
* VideoDoorbells
* Windows
* WindowCoverings
* Television

Note that the HomeKit primarily uses the Accessory Category of a device for determining the icon to show in the Home App when a device is being paired.  Apart from this purely cosmetic function, the Category assigned to a device does not in any way limit which Services or Characteristics can be implemented on that device.

---

[↩️](../README.md) Back to the Welcome page

