# HomeSpan Accessory Categories

Every HomeSpan device must be assigned a HomeKit Accessory Category. HomeSpan implements these categories as C++ Classes with names that exactly match the spelling and capitalization specified by Apple in Section 13 of [HAP-R2](https://developer.apple.com/support/homekit-accessory-protocol/), but without any spaces.  HomeSpan Accessory Categories are defined in HomeSpan's `Category` namespace.  For example, HomeSpan defines the *Garage Door Openers* Category (HAP Category 4) as `Category::GarageDoorOpeners`, which could be used when initializing HomeSpan as follows:

```C++
homeSpan.begin(Category::GarageDoorOpeners,"Acme Garage Door Lifts");
```

The table below provides a list of all HomeSpan Categories.

### Category List

| Category |
|---|
| AirConditioners |
| AirPurifiers |
| Bridges |
| Dehumidifiers |
| Doors |
| Fans |
| Faucets |
| Lighting |
| Locks |
| GarageDoorOpeners |
| Heaters |
| Humidifiers |
| IPCameras |
| Other |
| Outlets |
| ProgrammableSwitches |
| SecuritySystems |
| Sensors |
| ShowerSystems |
| Sprinklers |
| Switches |
| Thermostats |
| VideoDoorbells |
| Windows |
| WindowCoverings |

