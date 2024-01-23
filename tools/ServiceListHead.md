# HomeSpan Services and Characteristics

Below is a list of all Services supported by HomeSpan, along with a brief description of each Service.  Click on the arrow next to the description of any Service to see a list of all Characteristics supported by the Service, as well as a description of each Characteristic.[^1]

A blue diamond (🔹) next to a Characteristic means it is **required** for that Service, otherwise it is optional.  Information included for each Characteristic is as follows:

* **Format** - the native format of the Characteristic's value.  Note that *string* means a standard C-string (i.e. char \*)
* **Perms**, where
  * PR = Paired Read.  This means HomeKit can read the value from HomeSpan[^2]
  * PW = Paired Write.  This means HomeKit can write the value to HomeSpan, which triggers a call to `update()`
  * EV = Event Notification.  This means HomeSpan can push notifications of value changes to HomeKit using `setVal()`
* **Min** / **Max** - the default minimum and maximum values allowed in HomeKit.  For applicable numerical Characteristics, you can change the allowed minimum and maximum values using `setRange()`
* **Constants/Defaults**
  * for enumerated Characteristics, a list of all allowed values in the form of pre-defined constant expressions and their equivalent numeric values.  A check mark next to a constant indicates that it is the default value.  If your Accessory does not support certain states of a Characteristic, you can change the allowed values using `setValidValues()`
  * for all other Characteristics, the actual default value that is used if you do not specify one when instantiating the Characteristic

 [^1]: The hexidecimal numbers in parentheses next to each Service and Characteristic represent the short-form of the Apple's UUID for that Service or Characteristic.  These are provided for informational purposes only (you do not need to use them, or even know about them, to create HomeSpan sketches)

 [^2]: Though rarely needed, you can change the permissions of a Characteristic using `setPerms()`, `addPerms()`, and `removePerms()`
 
HomeSpan Services and Characteristics are implemented as C++ Classes with names that exactly match the spelling and capitalization specified by Apple in Sections 8  and 9 of HAP-R2, but without any spaces.  HomeSpan Services are defined in HomeSpan's `Service` namespace.  HomeSpan Characteristics are defined in HomeSpan's `Characteristic` namespace.  For example, HomeSpan defines the *Carbon Dioxide Sensor* Service (HAP Service 8.7) as `Service::CarbonDioxideSensor`, and the *Carbon Dioxide Detected* Characteristic (HAP Characteristic 9.16) as `Characteristic::CarbonDioxideDetected`.

The pre-defined constant expressions for enumerated Characteristics are in namespaces that match the name of the Characteristic.  For example, to set the *Air Quality* Characteristic of an *Air Quality Sensor* Service, you could use `setVal(AirQuality::GOOD)` or, equivalently, `setVal(2)`[^3]

---

[↩️](../README.md) Back to the Welcome page
