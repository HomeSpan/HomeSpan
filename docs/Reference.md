# HomeSpan Library Reference

The HomeSpan Library is invoked by including *HomeSpan.h* in your Arduino sketch as follows:

```C++
#include "HomeSpan.h"
```

At runtime this HomeSpan will create a global object named `homeSpan` that supports the following methods:

* `void begin(Category catID, char *displayName, char *hostNameBase, char *modelName)` 
  * Initializes HomeSpan.
  * **Must** be called at the beginning of each sketch before any other HomeSpan functions and is typically placed near the top of the Arduino `setup()` method, but **after** `Serial.begin()` so that initialization diagnostics can be output to the Serial Monitor.
  * All arguments are **optional**.
    * *catID* - the HAP Category HomeSpan broadcasts for pairing to HomeKit.  Default is Category::Lighting.  See [HomeSpan Categories](Categories.md) for a complete list.
    * *displayName* - the MDNS display name broadcast by HomeSpan.  Default is "HomeSpan Server".
    * *hostNameBase* - the full MDNS host name is broadcast by HomeSpan as *hostNameBase-DeviceID*.local, where DeviceID is a unique 6-byte code generated automatically by HomeSpan.  Default is "HomeSpan".
    * *modelName* - the HAP model name HomeSpan broadcasts for pairing to HomeKit.  Default is "HomeSpan-ESP32".
 
 * `void poll()`
   * Checks for HAP requests, local commands, and device activity.
   * **Must** be called repeatedly in each sketch and is typically placed at the top of the Arduino `loop()` method.
   
The following **optional** `homeSpan` methods override various HomeSpan initialization parameters used in `begin()`, and therefore **should** be called before `begin()` to take effect.  If a method is *not* called, HomeSpan uses the default parameter indicated below:

* `void setControlPin(uint8_t pin)`
  * Sets the ESP32 pin to use for the HomeSpan Control Button (default=21).
  
* `void setStatusPin(uint8_t pin)`
  * Sets the ESP32 pin to use for the HomeSpan Status LED (default=LED_BUILTIN).
  
* `void setApSSID(char *ssid)`
  * Sets the SSID (network name) of the HomeSpan Setup Access Point (default="HomeSpan-Setup").
  
* `void setApPassword(char *pwd)`
  * Sets the password of the HomeSpan Setup Access Point (default="homespan").
  
* `void setApTimeout(uint16_t nSec)`
  * Sets the duration (in seconds) that the HomeSpan Setup Access Point, once activated, stays alive before timing out (default=300 seconds).
  
* `void setCommandTimeout(uint16_t nSec)`
  * Sets the duration (in seconds) that the HomeSpan End-User Command Mode, once activated, stays alive before timing out (default=120 seconds).
  
* `void setLogLevel(uint8_t level)`
  * Sets the logging level for diagnostic messages, where:
    * 0 = top-level status messages only (default),
    * 1 = all status messages, and
    * 2 = all status messages plus all HAP communication packets to and from the HomeSpan device.
  * This parameter can also be changed at runtime via the [HomeSpan CLI](CLI.md).
  
* `void setMaxConnections(uint8_t nCon)`
  * Sets the maximum number of HAP Controllers that be simultaneously connected to HomeSpan (default=8).
