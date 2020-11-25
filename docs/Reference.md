# HomeSpan Library Reference

The HomeSpan Library is invoked by including *HomeSpan.h* in your Arduino sketch as follows:

```C++
#include "HomeSpan.h";
```

At runtime this HomeSpan will create a global object named `homeSpan` that supports the following methods:

* `void begin(Category catID, char *displayName, char *hostNameBase, char *modelName);` 
  * Initializes HomeSpan.
  * **Must** be called at the beginning of each sketch before any other HomeSpan functions and is typically placed near the top of the Arduino `setup()` method, but **after** `Serial.begin()` so that initialization diagnostics can be output to the Serial Monitor.
  * All arguments are **optional**.
    * *catID* - the HAP Category HomeSpan broadcasts for pairing to HomeKit.  Default is [Category::Lighting](Categories.md).
    * *displayName* - the MDNS display name broadcast by HomeSpan.  Default is "HomeSpan Server".
    * *hostNameBase* - the full MDNS host name is broadcast by HomeSpan as *hostNameBase-DeviceID*.local, where DeviceID is a unique 6-byte code generated automatically by HomeSpan.  Default is "HomeSpan".
    * *modelName* - the HAP model name HomeSpan broadcasts for pairing to HomeKit.  Default is "HomeSpan-ESP32".
 
 * `void poll()`
   * Checks for HAP requests, local commands, and device activity.
   * **Must** be called repeatedly in each sketch and typically placed at the top of the Arduino `loop()` method.
   
The following **optional** `homeSpan` methods override various HomeSpan initialization parameters used in `begin()`, and therefore **should** be called before `begin()` to take effect.  If not called, HomeSpan uses the default parameter indicated.
