# HomeSpan Library Reference

The HomeSpan Library is invoked by including *HomeSpan.h* in your Arduino sketch as follows:

```C++
#include "HomeSpan.h";
```

At runtime this HomeSpan will create a global object named `homeSpan` that supports the following methods:

* `void begin(Category catID, char *displayName, char *hostNameBase, char *modelName);` 
  * Initializes HomeSpan
  * **Must** be called at the beginning of each sketch (typically in `setup()`) before any other HomeSpan functions
  * All arguments are **optional**
    * *catID* - the HAP Category HomeSpan broadcasts for pairing to HomeKit.  Default is [Category::Lighting](Categories.md)
    * *displayName* - the MDNS display name broadcast by HomeSpan.  Default is "HomeSpan Server"
    * *hostNameBase* - the full MDNS host name is broadcast by HomeSpan as *hostNameBase-DeviceID*.local, where DeviceID is a unique 6-byte code generated automatically by HomeSpan.  Default is "HomeSpan"
    * *modelName* - the HAP model name HomeSpan broadcasts for pairing to HomeKit.  Default is "HomeSpan-ESP32"
 
 * `void poll()`
   * Checks for HAP requests, local commands, and device activity
   * **Must** be called repeatedly in each sketch (typically placed at the top of `loop()`)
