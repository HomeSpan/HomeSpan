# HomeSpan Library Reference

The HomeSpan Library is invoked by including *HomeSpan.h* in your Arduino sketch as follows:

```C++
#include "HomeSpan.h"
```

## *homeSpan*

At runtime this HomeSpan will create a global **object** named `homeSpan` that supports the following methods:

* `void begin(Category catID, char *displayName, char *hostNameBase, char *modelName)` 
  * initializes HomeSpan
  * **must** be called at the beginning of each sketch before any other HomeSpan functions and is typically placed near the top of the Arduino `setup()` method, but **after** `Serial.begin()` so that initialization diagnostics can be output to the Serial Monitor
  * all arguments are **optional**
    * *catID* - the HAP Category HomeSpan broadcasts for pairing to HomeKit.  Default is Category::Lighting.  See [HomeSpan Accessory Categories](Categories.md) for a complete list
    * *displayName* - the MDNS display name broadcast by HomeSpan.  Default is "HomeSpan Server"
    * *hostNameBase* - the full MDNS host name is broadcast by HomeSpan as *hostNameBase-DeviceID*.local, where DeviceID is a unique 6-byte code generated automatically by HomeSpan.  Default is "HomeSpan"
    * *modelName* - the HAP model name HomeSpan broadcasts for pairing to HomeKit.  Default is "HomeSpan-ESP32"
  * example: `homeSpan.begin(Category::Fans, "Living Room Ceiling Fan");`
 
 * `void poll()`
   * checks for HAP requests, local commands, and device activity
   * **must** be called repeatedly in each sketch and is typically placed at the top of the Arduino `loop()` method
   
The following **optional** `homeSpan` methods override various HomeSpan initialization parameters used in `begin()`, and therefore **should** be called before `begin()` to take effect.  If a method is *not* called, HomeSpan uses the default parameter indicated below:

* `void setControlPin(uint8_t pin)`
  * sets the ESP32 pin to use for the HomeSpan Control Button (default=21)
  
* `void setStatusPin(uint8_t pin)`
  * sets the ESP32 pin to use for the HomeSpan Status LED (default=LED_BUILTIN)
  
* `void setApSSID(char *ssid)`
  * sets the SSID (network name) of the HomeSpan Setup Access Point (default="HomeSpan-Setup")
  
* `void setApPassword(char *pwd)`
  * sets the password of the HomeSpan Setup Access Point (default="homespan")
  
* `void setApTimeout(uint16_t nSec)`
  * sets the duration (in seconds) that the HomeSpan Setup Access Point, once activated, stays alive before timing out (default=300 seconds)
  
* `void setCommandTimeout(uint16_t nSec)`
  * sets the duration (in seconds) that the HomeSpan End-User Command Mode, once activated, stays alive before timing out (default=120 seconds)
  
* `void setLogLevel(uint8_t level)`
  * sets the logging level for diagnostic messages, where:
    * 0 = top-level status messages only (default),
    * 1 = all status messages, and
    * 2 = all status messages plus all HAP communication packets to and from the HomeSpan device
  * this parameter can also be changed at runtime via the [HomeSpan CLI](CLI.md)
  
* `void setMaxConnections(uint8_t nCon)`
  * sets the maximum number of HAP Controllers that be simultaneously connected to HomeSpan (default=8)
  
## *SpanAccessory()*

Creating an instance of this **class** adds a new HAP Accessory to the HomeSpan HAP Database.

  * every HomeSpan sketch requires at least one Accessory
  * there are no arguments or associated methods
  * you must call `homeSpan.begin()` before instantiating any Accessories
  * example: `new SpanAccessory();`

## *SpanService()*

This is a **base class** from which all HomeSpan Services are derived, and should **not** be directly instantiated.  Rather, to create a new Service instantiate one of the HomeSpan Services defined in the [Service](ServiceList.md) namespace.  No arguments are needed.

* instantiated Services are added to the HomeSpan HAP Database and associated with the last Accessory instantiated
* instantiating a Service without first instantiating an Accessory throws an error during initialization
* example: `new Service::MotionSensor();`

The following methods are supported:

* `SpanService *setPrimary()`
  * specifies that this is the primary Service for the Accessory.  Returns a pointer to the Service itself so that the method can be chained during instantiation.  Example: `new Service::Fan->setPrimary();`
* `SpanService *setHidden()`
  * specifies that this is hidden Service for the Accessory.  Returns a pointer to the Service itself so that the method can be chained during instantiation.
* `virtual boolean update()`
  * HomeSpan calls this method upon receiving a request from a HomeKit Controller to update one or more Characteristics associated with the Service.  Users should override this method with code that implements that requested updates and returns *true* or *false* if the update succeeds or fails
* `virtual void loop()`
  * HomeSpan calls this method every time `homeSpan.poll()` is executed.  Users should override this method with code that monitors for state changes in Characteristics that require HomeKit Controllers to be notified.
* `virtual void button(int pin, int pressType)`
  * HomeSpan calls this method whenever a SpanButton() object associated with the Service is triggered.  Users should override this method with code that implements any actions to be taken in response to the SpanButton() trigger
    * *pin* - the ESP32 pin associated with the SpanButton() object
    * *pressType* - 
      * 0=single press (SpanButton::SINGLE)
      * 1=double press (SpanButton::DOUBLE)
      * 2=long press (SpanButton::LONG)
    
## *SpanCharacteristic()*
  
This is a **base class** from which all HomeSpan Characteristics are derived, and should **not** be directly instantiated.  Rather, to create a new Characteristic instantiate one of the HomeSpan Characteristics defined in the [Characteristic](ServiceList.md) namespace.

* instantiated Characteristics are added to the HomeSpan HAP Database and associated with the last Service instantiated
* instantiating a Characteristic without first instantiating a Service throws an error during initialization
* a single, optional argument is used to set the initial value of the Characteristic at startup.
* example: `new Characteristic::Brightness(50);`

