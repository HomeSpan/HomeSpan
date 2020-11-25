# HomeSpan Library Reference

The HomeSpan Library is invoked by including *HomeSpan.h* in your Arduino sketch as follows:

```C++
#include "HomeSpan.h";
```

At runtime this HomeSpan will create a global object named `homeSpan` that supports the following methods:

* `void begin(Category catID, char *displayName, char *hostNameBase, char *modelName);` 
  * begin() initializes homeSpan and **must** be called at the beginning of each sketch before any other HomeSpan functions.
  * all arguments are **optional**
  * *catID* - the HAP Category HomeSpan broadcasts for pairing to HomeKit.  Default is [Category::Lighting](Categories.md)
  * *displayName* - the MDNS display name broadcast by HomeSpan.  Default is "HomeSpan Server"
  * *hostNameBase* - the full MDNS host name is broadcast by HomeSpan as *hostNameBase-DeviceID*.local, where DeviceID is a unique 6-byte code generated automatically by HomeSpan.  Default is "HomeSpan"
  * modelName - the HAP model name HomeSpan broadcasts for pairing to HomeKit.  Default is "HomeSpan-ESP32"
  
