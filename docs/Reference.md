# HomeSpan API Reference

The HomeSpan Library is invoked by including *HomeSpan.h* in your Arduino sketch as follows:

```C++
#include "HomeSpan.h"
```

## *homeSpan*

At runtime HomeSpan will create a global **object** named `homeSpan` (of type *class Span*) that supports the following methods:

* `void begin(Category catID, const char *displayName, const char *hostNameBase, const char *modelName)` 
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
   * **must** be called repeatedly in each sketch and is typically placed at the top of the Arduino `loop()` method (*unless* `autoPoll()`, described further below, is used instead)

---

The following **optional** `homeSpan` methods override various HomeSpan initialization parameters used in `begin()`, and therefore **should** be called before `begin()` to take effect.
Methods with a return type of `Span&` return a reference to `homeSpan` itself and can thus be chained together (e.g. `homeSpan.setControlPin(21).setStatusPin(13);`).  If a method is *not* called, HomeSpan uses the default parameter indicated below:

* `Span& setControlPin(uint8_t pin, triggerType=PushButton::TRIGGER_ON_LOW)`
  * sets the ESP32 *pin* to use for the HomeSpan Control Button
  * if this method is not called, HomeSpan will assume there is no Control Button
  * the optional second argument, *triggerType*, configures the Control Button as follows:
    * `PushButton::TRIGGER_ON_LOW` - triggers when *pin* is driven LOW
      * suitable for buttons that connect *pin* to GROUND (this is the default when *triggerType* is not specified)
    * `PushButton::TRIGGER_ON_HIGH` - triggers when *pin* is driven HIGH
      * suitable for buttons that connect *pin* to VCC (typically 3.3V)
    * `PushButton::TRIGGER_ON_TOUCH` - uses the device's touch-sensor peripheral to trigger when *pin* has been touched
      * not available on ESP32-C3
  * as an alternative, you can set *triggerType* to any user-defined function of the form `boolean(int arg)` to utilize any device as a Control Button.  See **SpanButton** below for details

* `int getControlPin()`
   * returns the pin number of the HomeSpan Control Button as set by `setControlPin(pin)`, or -1 if no pin has been set
  
* `Span& setStatusPin(uint8_t pin)`
  * sets the ESP32 *pin* to use for the HomeSpan Status LED
  * assumes a standard LED will be connected to *pin*
  * if neither this method nor any equivalent method is called, HomeSpan will assume there is no Status LED
  
* `Span& setStatusPixel(uint8_t pin, float h=0, float s=100, float v=100)`
  * sets the ESP32 *pin* to use for the HomeSpan Status LED
  * this method is an *alternative* to using `setStatusPin()` above
  * assumes an RGB NeoPixel (or equivalent) will be connected to *pin*
  * works well with ESP32 boards that have a built-in NeoPixel LED, though adding an external NeoPixel is fine
  * users can optionally specify the color HomeSpan will use with the NeoPixel by providing the following HSV values:
    * h = Hue from 0-360
    * s = Saturation percentage from 0-100
    * v = Brightness percentage from 0-100
  * color defaults to *red* if unspecified
  * example: `homeSpan.setStatusPixel(8,120,100,20)` sets the Status LED to light green using a NeoPixel attached to pin 8 
  * if neither this method nor any equivalent method is called, HomeSpan will assume there is no Status LED

* `Span& setStatusDevice(Blinkable *sDev)`
  * sets the Status LED to a user-specified Blinkable device, *sDev*
  * this method is an *alternative* to using either `setStatusPin()` or `setStatusPixel()` above
  * see [Blinkable](Blinkable.md) for details on how to create generic Blinkable devices
  * useful when using an LED connected to a pin expander, or other specialized driver, as the Status LED
  * if neither this method nor any equivalent method is called, HomeSpan will assume there is no Status LED

* `Span& setStatusAutoOff(uint16_t duration)`
  * sets Status LED to automatically turn off after *duration* seconds
  * Status LED will automatically turn on, and duration timer will be reset, whenever HomeSpan activates a new blinking pattern
  * if *duration* is set to zero, auto-off is disabled (Status LED will remain on indefinitely)
  
* `int getStatusPin()`
   * returns the pin number of the Status LED as set by `setStatusPin(pin)`, or -1 if no pin has been set

* `Span& setApSSID(const char *ssid)`
  * sets the SSID (network name) of the HomeSpan Setup Access Point (default="HomeSpan-Setup")
  
* `Span& setApPassword(const char *pwd)`
  * sets the password of the HomeSpan Setup Access Point (default="homespan")
  
* `Span& setApTimeout(uint16_t nSec)`
  * sets the duration (in seconds) that the HomeSpan Setup Access Point, once activated, stays alive before timing out (default=300 seconds)
  
* `Span& setCommandTimeout(uint16_t nSec)`
  * sets the duration (in seconds) that the HomeSpan End-User Command Mode, once activated, stays alive before timing out (default=120 seconds)
  
* `Span& setLogLevel(int level)`
  * sets the logging level for diagnostic messages, where:
    * 0 = top-level HomeSpan status messages, and any `LOG0()` messages specified in the sketch by the user (default)
    * 1 = all HomeSpan status messages, and any `LOG1()` messages specified in the sketch by the user
    * 2 = all HomeSpan status messages plus all HAP communication packets to and from the HomeSpan device, as well as all `LOG1()` and `LOG2()` messages specified in the sketch by the user
    * -1 = supresses ALL HomeSpan status messages, including all `LOG0()`, `LOG1()`, and `LOG2()` messages specified in the sketch by the user, freeing up the Serial port for other uses
  * the log level setting has no impact on any `Serial.print()` or `Serial.printf()` statements that may be used in a sketch.  Use one of the `LOG()` macros instead of `Serial.print()` or `Serial.printf()` if you want to control the output by setting the HomeSpan log level
  * the log level setting has no impact on any ESP32 diagnostic messages output by the ESP32 operating system itself.  To suppress these mesaages make sure to compile your sketch with the *Core Debug Level* set to "None" in the Tools menu of the Arduino IDE
  * note the log level can also be changed at runtime with the 'L' command via the [HomeSpan CLI](CLI.md)
  * see [Message Logging](Logging.md) for complete details

* `int getLogLevel()`
  * returns the current Log Level as set by `setLogLevel(level)`
  
* `Span& setPortNum(uint16_t port)`
  * sets the TCP port number used for communication between HomeKit and HomeSpan (default=80)
  
* `Span& setHostNameSuffix(const char *suffix)`
  * sets the suffix HomeSpan appends to *hostNameBase* to create the full hostName
  * if not specified, the default is for HomeSpan to append a dash "-" followed the 6-byte Accessory ID of the HomeSpan device
  * setting *suffix* to a null string "" is permitted
  * example: `homeSpan.begin(Category::Fans, "Living Room Ceiling Fan", "LivingRoomFan");` will yield a default *hostName* of the form *LivingRoomFan-A1B2C3D4E5F6.local*.  Calling `homeSpan.setHostNameSuffix("v2")` prior to `homeSpan.begin()` will instead yield a *hostName* of *LivingRoomFanv2.local*
  
* `Span& setQRID(const char *id)`
  * changes the Setup ID, which is used for pairing a device with a [QR Code](QRCodes.md), from the HomeSpan default to *id*
  * the HomeSpan default is "HSPN" unless permanently changed for the device via the [HomeSpan CLI](CLI.md) using the 'Q' command
  * *id* must be exactly 4 alphanumeric characters (0-9, A-Z, and a-z).  If not, the request to change the Setup ID is silently ignored and the default is used instead
  
---

The following **optional** `homeSpan` methods enable additional features and provide for further customization of the HomeSpan environment.  Unless otherwise noted, calls **should** be made before `begin()` to take effect:

* `int enableOTA(boolean auth=true, boolean safeLoad=true)`
  * enables [Over-the-Air (OTA) Updating](OTA.md) of a HomeSpan device, which is otherwise disabled
  * HomeSpan OTA requires an authorizing password unless *auth* is specified and set to *false*
  * the default OTA password for new HomeSpan devices is "homespan-ota"
  * this can be changed via the [HomeSpan CLI](CLI.md) using the 'O' command
  * note enabling OTA reduces the number of HAP Controller Connections by 1
  * OTA Safe Load will be enabled by default unless the second argument is specified and set to *false*.  HomeSpan OTA Safe Load checks to ensure that sketches uploaded to an existing HomeSpan device are themselves HomeSpan sketches, and that they also have OTA enabled.  See [HomeSpan OTA Safe Load](OTA.md#ota-safe-load) for details
  * returns 0 if enabling OTA was successful, or -1 and reports an error to the Serial Monitor if not

* `int enableOTA(const char *pwd, boolean safeLoad=true)`
  * an alternative form of `enableOTA()` that allows you to programmatically change the OTA password to the specified plain-text *pwd*, which must contain between 1 and 32 characters
  * the plain-text password you specify is automatically converted into a SHA256 hash (default) or MD5 hash (for legacy systems running on Arduino-ESP32 Cores *prior* to version 3.3.2) for use by the sketch for OTA uploads 
  * this command causes HomeSpan to ignore, but does not otherwise alter, any hashed password previously stored using the 'O' command 
  * as an alternative to setting *pwd* to your desired plain-text OTA password, you may instead specify your password in pre-hashed format as follows:
    * if pwd begins with "0x" followed by exactly 64 hexidecimal digits HomeSpan interprets pwd as a SHA256 hash
    * if pwd begins with "0x" followed by exactly 32 hexidecimal digits HomeSpan interprets pwd as an MD5 hash
  * use SHA256 for devices running Arduino-ESP32 Core version 3.3.2 or later, else use MD5 for earlier versions 
  * this command returns 0 if enabling OTA was successful, or -1 and reports an error to the Serial Monitor if not

* `void markSketchOK()`
  * marks the OTA State of the currently-running partition as *VALID*
  * must be called from within a sketch when the HomeSpan OTA Rollback mechanism has been enabled to avoid the bootloader from automatically rolling back to a prior version of your sketch upon the next reboot of the device
  * to enable the HomeSpan OTA Rollback mechanism add `#include "SpanRollback.h"` to the top of your sketch
  * see [HomeSpan OTA Rollback](OTA.md#ota-rollback) for details 

* `Span& enableAutoStartAP()`
  * enables automatic start-up of WiFi Access Point if WiFi Credentials are **not** found at boot time
  * methods to alter the behavior of HomeSpan's Access Point, such as `setApTimeout()`, must be called prior to `enableAutoStartAP()` to have an effect  
  
* `Span& setApFunction(void (*func)())`
  * replaces HomeSpan's built-in WiFi Access Point with user-defined function *func*
  * *func* must be of type *void* and have no arguments
  * *func* will be called instead of HomeSpan's built-in WiFi Access Point whenever the Access Point is launched:
    * via the CLI by typing 'A', or
    * via the Control Button using option 3 of the Command Mode, or
    * automatically upon start-up if `enableAutoStartAP()` is set and there are no stored WiFi Credentials
  * after identifying the SSID and password of the desired network, *func* must call `setWifiCredentials()` to save and use these values
  * it is recommended that *func* terminates by restarting the device using `ESP.restart()`. Upon restart HomeSpan will use the SSID and password just saved
  
* `Span& setWifiCredentials(const char *ssid, const char *pwd)`
  * sets the SSID (*ssid*) and password (*pwd*) of the WiFi network to which HomeSpan will connect
  * *ssid* and *pwd* are automatically saved in HomeSpan's non-volatile storage (NVS) for retrieval when the device restarts
  * note that the saved values are truncated if they exceed the maximum allowable characters (ssid=32; pwd=64)
  * :warning: SECURITY WARNING: The purpose of this function is to allow advanced users to *dynamically* set the device's WiFi Credentials using a customized Access Point function specified by `setApFunction(func)`. It it NOT recommended to use this function to hardcode your WiFi SSID and password directly into your sketch.  Instead, use one of the more secure methods provided by HomeSpan, such as typing 'W' from the CLI, or launching HomeSpan's Access Point, to set your WiFi credentials without hardcoding them into your sketch
 
* `Span& setConnectionTimes(uint32_t minTime, uint32_t maxTime, uint8_t nSteps)`
  * overrides HomeSpan's default repeating pattern of increasing wait times when trying to connect to a WiFi network, where
    * *minTime* - the minimum time (in seconds) that HomeSpan initially waits when first trying to connect to a WiFi network
    * *maxTime* - the maximum time (in seconds) that HomeSpan will wait on subequent attempts to connect if first attempt fails
    * *nSteps* - the number of steps HomeSpan uses to set the increasing intermediate times between *minTime* and *maxTime* as each re-attempt to connect is made
  * example: `homeSpan.setConnectionTimes(5,60,3);` causes HomeSpan to initially wait 5 seconds when first attempting to connect to a WiFi network, and then wait 11, 26, and finally 60 seconds on the next 3 subsequent attempts. If HomeSpan has still not connected, the pattern repeats indefinitely
  * if either *minTime* or *nSteps* is set to zero, or if *maxTime* is not strictly greater than *minTime*, HomeSpan ignores the request and reports a warning message to the Serial Monitor
  * note this is an optional method. If not called HomeSpan uses default parameters of {5,60,5} which yields a wait pattern of 5, 8, 14, 22, 36, and 60 seconds between connection attempts

* `Span& setWifiBegin(void (*func)(const char *ssid, const char *pwd))`
  * sets an **alternative** user-defined function, *func*, to be called by HomeSpan when it tries to connect to a WiFi network with specified credentials SSID=*ssid* and password=*pwd*, **instead** of HomeSpan's default behavior of simply calling `WiFi.begin(ssid, pwd)`
  * this ability to define an alternative *func* is provided for users that either need to use a different type of call to establish WiFi connectivity (e.g. connectivity to an enterprise network), or that require additional functionality to be called when connectivity is established (e.g. changing the WiFi power)
  * note *func* is called every time HomeSpan tries to connect to WiFi network, including during repeated wait periods as well as reconnects after a disconnect
  * the function *func* must be of type *void* and accept two argument into which HomeSpan will pass whatever SSID and Password you previously saved as HomeSpan's WiFi Credentials, or that you explicitly set in the sketch using `setWifiCredentials()` above
    * *func* does not necessarily need to use this information, but it must must be able to accept the data
  * example: `homeSpan.setWifiBegin(myWifi)` where *myWifi* is defined below would address the issue on some ESP32 boards that will not connect to a WiFi network unless the WiFi radio power is changed to a lower value immediately after calling `WiFi.begin()`:

```C++
 void myWifi(const char *ssid, const char *pwd){
   WiFi.begin(ssid,pwd);                 // don't forget to call WiFi.begin(), if still needed, in your alternative function
   WiFi.setTxPower(WIFI_POWER_8_5dBm);   // set power immediately after as required for some ESP32 boards
}
```

* `Span& enableWiFiRescan(uint32_t iTime=1, uint32_t pTime=0, int thresh=3)`
  * when you configure HomeSpan to connect to a WiFi SSID that broadcasts from more than one access point (e.g. a mesh network), HomeSpan connects to the access point with the strongest RSSI signal for that SSID
  * once connected, HomeSpan remains "attached" to that specific access point unless it looses overall connectivity to the network, or is otherwise purposely disconnected, at at which point it will attempt to reconnect once again the strongest access point broadcasting the original SSID
  * calling `enableWiFiRescan()` enables HomeSpan to further optimize WiFi connectivity in the background during normal operation by periodically rescanning all access points with the original SSID, and automatically performing a disconnect/reconnect if it finds an access point with a stronger RSSI signal, where
    * *iTime* - the time HomeSpan waits (in minutes) between first making a connection to an access point and rescanning to check for other stronger access points
    * *pTime* -  the time HomeSpan waits after its first scan before performing any subsequent rescans
    * *thresh* -  the threshhold difference (in RSSI units) by which the signal strength of a newly-scanned access point must be stronger than that of the access point to which HomeSpan is current connected for HomeSpan to trigger a connect/disconnect.  This prevents the HomeSpan from switching back and forth between two access points with very similar RSSI strengths
  * example: `homeSpan.enableWiFiRescan(2, 5, 3)` causes HomeSpan to rescan all access points 2 minutes after initially connecting, and then every 5 minutes thereafter; if after any rescan HomeSpan finds a different access point with an RSSI signal that is 3 or more units stronger than the currently-connect access point, it will disconnect from the existing access point and connect to the stronger one
  * the default is for HomeSpan to NOT perform any background rescans unless you specifically enable this by calling `homeSpan.enableWiFiRescan()` from your sketch
    * if *iTime* is set to zero, rescanning is disabled
    * if *iTime* is greater than zero but *pTime* is set to zero, rescanning occurs only once at *iTime* minutes after connecting, but not thereafter

* `Span& addBssidName(String bssid, string name)`
  * creates a friendly display name for any given WiFi access point that HomeSpan can display (in both the Serial Monitor and the Web Log) alongside the BSSID whenever needed, where
    * *bssid* - the 6-byte BSSID MAC address of an access point, in the form "XX:XX:XX:XX:XX:XX"
    * *name* - a friendly display name for the access point
  * most useful when HomeSpan is connected to mesh WiFi network containing multiple access points sharing the same SSID, and you want to keep track of which access point is being used withouth having to memorize the BSSIDs of each access point
  * example: `homeSpan.addBssidName("3A:98:B5:EF:BF:69","Kitchen").addBssidName("3A:98:B5:DB:54:86","Basement");` creates display names "Kitchen" and "Basement" for access points with BSSIDs "3A:98:B5:EF:BF:69" and "3A:98:B5:DB:54:86", respectively
 
* `Span& setVerboseWifiReconnect(bool verbose)`
  * when trying connecting to WiFi, HomeSpan normally logs "Trying to connect to..." messages to the Serial Monitor and the Web Log
  * calling this method with *verbose* set to *false* supresses these messages
  * calling this method a second time with *verbose* set to *true* re-activates these messages (default behavior)

* `Span& setConnectionCallback(void (*func)(int count))`
  * sets an optional user-defined callback function, *func*, to be called by HomeSpan every time WiFi or Ethernet connectivity has been established or re-established after a disconnect.  The function *func* must be of type *void* and accept a single *int* argument, *count*, into which HomeSpan passes the number of times WiFi or Ethernet connectivity has been established or re-established (i.e. *count*=1 on initial WiFi or Ethernet connection; *count*=2 if re-established after the first disconnect, etc.)
  * note HomeSpan considers a connection to be established as soon as it acquires an IP address from the router.  If IPv6 has been enabled, HomeSpan will typically receive multiple IP addresses from the router over the course of a few seconds.  HomeSpan calls *func* only once upon receipt of the *first* IP address (regardless of whether is it IPv4 or IPv6).  It does *not* call *func* upon receipt of any subsequent IP addresses it may receive from the router
  * see [WiFi and Ethernet Connectivity](Networks.md) for more details about HomeSpan's use of IPv4 and IPv6 addresses
 
* `Span& useEthernet()`
  * forces HomeSpan to use Ethernet instead of WiFi, even if ETH has not yet been called or an Ethernet card has not been found prior to `homeSpan.begin()` being called
    
* `Span& setPairCallback(void (*func)(boolean status))`
  * sets an optional user-defined callback function, *func*, to be called by HomeSpan upon completion of pairing to a controller (*status=true*) or unpairing from a controller (*status=false*)
  *   this one-time call to *func* is provided for users that would like to trigger additional actions when the device is first paired, or the device is later unpaired
  *   note this *func* is **not** called upon start-up and should not be used to simply check whether a device is paired or unpaired.  It is only called when pairing status changes
  *   the function *func* must be of type *void* and accept one *boolean* argument
 
* `Span& setControllerCallback(void (*func)())`
  * sets an optional user-defined callback function, *func*, to be called by HomeSpan every time a new controller is added, removed, or updated, even if the pairing status does not change
  * note this method differs from `setPairCallback()`, which is only called if the device's pairing status changes, such as when the first controller is added during initial pairing, or the last controller is removed when unpairing
  * the function *func* must be of type *void* and have no arguments
  * see the `controllerListBegin()` and `controllerListEnd()` methods for details on how to read the pairing data for each paired controller (*only needed to support certain advanced use cases*)
 
* `Span& setStatusCallback(void (*func)(HS_STATUS status))`
  * sets an optional user-defined callback function, *func*, to be called by HomeSpan whenever its running state (e.g. WiFi Connecting, Pairing Needed...) changes in way that would alter the blinking pattern of the (optional) Status LED
  * if *func* is set, it will be called regardless of whether or not a Status LED has actually been defined
  * this allows users to reflect changes to the current state of HomeSpan using alternative methods, such as outputting messages to an embedded LCD or E-Ink display
  * the function *func* must be of type *void* and accept one argument of enum type [HS_STATUS](HS_STATUS.md)

* `char* statusString(HS_STATUS s)`
  * returns a pre-defined character string message representing *s*, which must be of enum type [HS_STATUS](HS_STATUS.md)
  * typically used in conjunction with `setStatusCallback()` above

* `Span& setPollingCallback(void (*func)())`
  * sets an optional user-defined callback function, *func*, to be called by HomeSpan *one time* upon completing its first pass through the HomeSpan `poll()` function
  * the function *func* must be of type *void* and have no arguments

* `Span& setGetCharacteristicsCallback(void (*func)(const char *getCharList))`
  * sets an optional user-defined callback function, *func*, to be called by HomeSpan whenever it receives a *GET /characteristics* request from HomeKit
    * HomeKit generally sends this request to every paired device each time the Home App is opened on an iPhone or Mac
  * note *func* is called **prior** to HomeSpan reading the requested Characteristics and sending their values back to HomeKit
    * this callback is useful in circumstances where the current state of a sensor-style Characteristic must be read by HomeSpan using a separate "expensive" process that should be called only when needed as opposed to being continuously updated in a Services `loop()` method
  * the function *func* must be of type *void* and accept one argument of type *const char \** into which HomeSpan passes the list of Characteristic AID/IID pairs that HomeKit provided in its HTTP GET request
    * *getCharList* can be used to determine if the HTTP GET request includes the AID/IID pair for any specific Characteristic
    * this allows the user to act on the callback based on which specific Characteristics were requested by HomeKit
    * see `SpanCharacteristic::foundIn(const char *getCharList)`

* `Span& setPairingCode(const char *s)`
  * sets the Setup Pairing Code to *s*, which **must** be exactly eight numerical digits (no dashes)
  * example: `homeSpan.setPairingCode("46637726");`
  * a hashed version of the Pairing Code will be saved to the device's non-volatile storage, overwriting any currently-stored Pairing Code
  * this method operated silently unless *s* contains an invalid code, in which case a fatal error will be reported to the Serial Monitor, the code will *not* be saved, and the sketch will be HALTED
  * :warning: SECURTY WARNING: Hardcoding a device's Pairing Code into your sketch is considered a security risk and is **not** recommended.  Instead, use one of the more secure methods provided by HomeSpan, such as typing 'S \<code\>' from the CLI, or launching HomeSpan's Access Point, to set your Pairing Code without hardcoding it into your sketch

* `Span& setSketchVersion(const char *sVer)`
  * sets the version of a HomeSpan sketch to *sVer*, which can be any arbitrary character string
  * if unspecified, HomeSpan uses "n/a" as the default version text
  * HomeSpan displays the version of the sketch in the Arduino IDE Serial Monitor upon start-up
  * HomeSpan also includes both the version of the sketch, as well as the version of the HomeSpan library used to compile the sketch, as part of its HAP MDNS broadcast.  This data is *not* used by HAP.  Rather, it is for informational purposes and allows you to identify the version of a sketch for a device that is updated via [OTA](OTA.md), rather than connected to a computer

* `const char *getSketchVersion()`
  * returns the version of a HomeSpan sketch, as set using `setSketchVersion(const char *sVer)` above, or "n/a" if not set
  * can by called from anywhere in a sketch

* `Span& setCompileTime(const char *compTime)`
  * sets the compilation time of a HomeSpan sketch to *compTime*, which can be any arbitrary character string
  * if unspecified, HomeSpan derives a compTime string from the `__DATE__` and `__TIME__` macros provided by the compiler when the sketch is compiled
  * HomeSpan displays the compilation time of the sketch in the Arduino IDE Serial Monitor upon start-up

* `Span& enableWebLog(uint16_t maxEntries, const char *timeServerURL, const char *timeZone, const char *logURL)`
  * enables a rolling Web Log that displays the most recent *maxEntries* entries created by the user with the `WEBLOG()` macro.  Parameters, and their default values if unspecified, are as follows:
    * *maxEntries* - maximum number of (most recent) entries to save.  If unspecified, defaults to 0, in which case the Web Log will only display status without any log entries
    * *timeServerURL* - the URL of a time server that HomeSpan will use to set its clock upon startup after a WiFi connection has been established. HomeSpan will reserve one extra socket connection when a time server is specified. If unspecified, defaults to NULL, in which case HomeSpan skips setting the device clock
    * *timeZone* - specifies the time zone to use for setting the clock.  Uses POSIX.1 format only and does not support the *Time Zone Database*, or *tzdata*. As per [GNU libc documentation for TZ](https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html), *the offset specifies the time value you must **add to the local time** to get a Coordinated Universal Time value*. "UTC+5:00" means that local time + 5 hours give UTC time. See the GNU libc documentation for some examples, including how to specify North American Eastern Standard Time (EST) and Eastern Daylight Time (EDT) with start and end dates of EDT. If *serverURL=NULL* this field is ignored; if *serverURL!=NULL* this field is required
    * *logURL* - the URL of the Web Log page for this device.  If unspecified, defaults to "status".  If *logURL* is set to NULL HomeSpan will use the *timeServerURL* and *timeZone* parameters to set the clock, but it will *not* serve any Web Log pages in response to any HTTP requests.  However, Web Log data is still accumulated internally and the resulting HTML can be accessed anytime by calling the `homeSpan.getWebLog()` method (see below)  
  * example: `homeSpan.enableWebLog(50,"pool.ntp.org","UTC-1:00","myLog");` creates a web log at the URL *http<nolink>://HomeSpan-\[DEVICE-ID\].local:\[TCP-PORT\]/myLog* that will display the 50 most-recent log messages produced with the WEBLOG() macro.  Upon start-up (after a WiFi connection has been established) HomeSpan will attempt to set the device clock by calling the server "pool.ntp.org" and adjusting the time to be 1 hour ahead of UTC.
  * when attemping to connect to *timeServerURL*, HomeSpan waits 120 seconds for a response.  This is done in the background and does not block HomeSpan from running as usual while it tries to set the time.  If no response is received after the 120-second timeout period, HomeSpan assumes the server is unreachable and skips the clock-setting procedure.  Use `setTimeServerTimeout()` to re-configure the 120-second timeout period to another value
  * see [Message Logging](Logging.md) for complete details

* `Span& setTimeServerTimeout(uint32_t tSec)`
  * changes the default 120-second timeout period HomeSpan uses when `enableWebLog()` tries set the device clock from an internet time server to *tSec* seconds
 
* `Span& setWebLogCSS(const char *css)`
  * sets the format of the HomeSpan Web Log to the custom style sheet specified by *css*
  * see [Message Logging](Logging.md) for details on how to construct *css*
 
* `Span& setWebLogFavicon(const char *faviconURL)`
  * adds a favicon to the HomeSpan Web Log, where *faviconURL* points to a hosted **PNG** image file containing the favicon
  * if left unspecified, *faviconURL* defaults to [docs/images/HomeSpanLogo.png](images/HomeSpanLogo.png) hosted in the master branch of this repository
  * see [Message Logging](Logging.md) for further details

* `Span& setWebLogCallback(void (*func)(String &htmlText))`
  * sets an optional user-defined callback function, *func*, to be called by HomeSpan whenever the Web Log is produced
  * allows user to add additional custom data to the initial table of the Web Log by setting the String *htmlText*, which is passed as a reference to *func*
  * the function *func* must be of type *void* and accept one argument of type *String \&*
  * see [Message Logging](Logging.md) for details on how to construct *htmlText*

* `void getWebLog(void (*f)(const char *htmlBuf, void *args), void *userData)`
   * when called, HomeSpan *streams* the current Web Log HTML text, and any optionally-specified *userData*, directly to the user-defined function, *f()*, which should return a *void* and accept the following two arguments:
     *  *htmlBuf* - pointer to part of the HTML text for the Web Log page
     *  *args* - a pass-through of the *userData* argument
   *  if user-defined data is not needed, set *userData* to NULL
   * to avoid creating a single large text buffer, HomeSpan splits the HTML for the Web Log into chunks of 1024 bytes and repeatedly calls *f()* until all the HTML has been streamed; HomeSpan then makes a final call to *f()* with *htmlBuf* set to NULL indicating to the user that the end of the HTML text has been reached
   * this command is primarily used to redirect Web Log pages to a user-defined process for alternative handling, display, or transmission
   * see [Message Logging](Logging.md) for more details

* `void assumeTimeAcquired()`
  * calling this method tells HomeSpan to assume that you have acquired the time using your own code
  * useful if you don't want to specify a *timeServerURL* when enabling the Web Log, but would rather acquire it manually
    * note if a *timeServerURL* is not specified when enabling the Web Log, the Web Log records will show the time as "Unknown" unless and until you call this method   
  
* `void processSerialCommand(const char *CLIcommand)`
  * processes the *CLIcommand* just as if were typed into the Serial Monitor
  * allows for programmatic access to all CLI commands, included any custom commands defined by the user
  * will work whether or not device is connected to a computer
  * example: `homeSpan.processSerialCommand("A");` starts the HomeSpan Setup Access Point
  * example: `homeSpan.processSerialCommand("Q HUB3");` changes the HomeKit Setup ID for QR Codes to "HUB3"

* `Span& setRebootCallback(void (*func)(uint8_t count), uint32_t upTime)`
  * sets an optional user-defined callback function, *func*, that is called (just once) when *upTime* milliseconds after rebooting have elapsed (default *upTime*=5000 ms if not specified)
  * the function *func* must be of type *void* and accept one argument of type *uint8_t*
  * the parameter *count*, which HomeSpan passes to *func*, indicates the number of "short" reboots that have occured prior to the current reboot, where a "short" reboot is any that occurs **before** *upTime* milliseconds have elapsed
  * this allows the user to provide a generic form of input to a sketch by rapidly turning on/off power to the device a specified number of times, typically to provide a method of resetting some aspect of a remote device
  * example using a lamba function:
    * `homeSpan.setRebootCallback( [](uint8_t c) {if(c==3) homeSpan.processSerialCommand("X");} );`
    * causes HomeSpan to run the 'X' Serial Command, which erases WiFi data, if the device is "short" rebooted exactly 3 times, where each reboot is for less than 5 seconds
    * note that creating 3 short reboots means you actually cycle the power (or press the reset button) a total of 4 times, since the last time you allow the sketch to run without rebooting
 
* `Span& setSerialInputDisable(boolean val)`
   * if *val* is true, disables HomeSpan from reading input from the Serial port
   * if *val* is false, re-enables HomeSpan reading input from the Serial port
   * useful when the main USB Serial port is needed for reading data from an external Serial peripheral, rather than being used to read input from the Arduino Serial Monitor

 * `boolean getSerialInputDisable()`
   * returns *true* if HomeSpan reading from the Serial port is currently disabled
   * returns *false* if HomeSpan is operating normally and will read any CLI commands input into the Arduino Serial Monitor
 
---

The following **optional** `homeSpan` methods provide additional run-time functionality for more advanced use cases: 
 
* `void deleteStoredValues()`
  * deletes the value settings of all stored Characteristics from the NVS
  * performs the same function as typing 'V' into the CLI
 
* `boolean deleteAccessory(uint32_t aid)`
  * deletes Accessory with Accessory ID of *aid*, if found
  * returns true if successful (match found), or false if the specified *aid* does not match any current Accessories
  * allows for dynamically changing the Accessory database during run-time (i.e. changing the configuration *after* the Arduino `setup()` has finished)
  * deleting an Accessory automatically deletes all Services, Characteristics, and any other resources it contains
  * outputs Level-1 Log Messages listing all deleted components
  * note: though deletions take effect immediately, HomeKit Controllers, such as the Home App, will not be aware of these changes until the database configuration number is updated and rebroadcast - see `updateDatabase()` below
 
* `boolean updateDatabase()`
  * recomputes the database configuration number and, if changed, rebroadcasts the new number via MDNS so all connected HomeKit Controllers, such as the Home App, can request a full refresh to accurately reflect the new configuration
  * returns true if configuration number has changed, false otherwise
  * *only* needed if you want to make run-time (i.e. after the Arduino `setup()` function has completed) changes to the device's Accessory database 
  * use anytime after dynamically adding one or more Accessories (with `new SpanAccessory(aid)`) or deleting one or more Accessories (with `homeSpan.deleteAccessory(aid)`)
  * **important**: once you delete an Accessory, you cannot re-use the same *aid* when adding a new Accessory (on the same device) unless the new Accessory is configured with the exact same Services and Characteristics as the deleted Accessory
  * note: this method is **not** needed if you have a static Accessory database that is fully defined in the Arduino `setup()` function of a sketch

* `Span& forceNewConfigNumber()`
  * forces HomeSpan to update the database configuration number at start-up, as well as anytime `updateDatabase()` is called from a sketch, regardless of whether there has been any change to the database configuration  

* `Span& resetIID(uint32_t newIID)`
  * resets the IID count for the current Accessory to *newIID*, which must be greater than 0
  * throws an error and halts program if called before at least one Accessory is created
  * example: `homeSpan.resetIID(100)` causes HomeSpan to set the IID to 100 for the very next Service or Characteristic defined within the current Accessory, and then increment the IID count going forward so that any Services or Characteristics subsequently defined (within the same Accessory) have IID=101, 102, etc.
  * note: calling this function only affects the IID generation for the current Accessory (the count will be reset to IID=1 upon instantiation of a new Accessory)

* `const_iterator controllerListBegin()` and `const_iterator controllerListEnd()`
  * returns a *constant iterator* pointing to either the *beginning*, or the *end*, of an opaque linked list that stores all controller data
  * iterators should be defined using the `auto` keyword as follows: `auto myIt=homeSpan.controllerListBegin();`
  * controller data can be read from a de-referenced iterator using the following methods:    
    * `const uint8_t *getID()` returns pointer to the 36-byte ID of the controller
    * `const uint8_t *getLTPK()` returns pointer to the 32-byte Long Term Public Key of the controller
    * `boolean isAdmin()` returns true if controller has admin permissions, else returns false
  * <details><summary>click here for example code</summary><br>

    ```C++
    // Extract and print the same data about each controller that HomeSpan prints to the Serial Monitor when using the 's' CLI command
    
    Serial.printf("\nController Data\n");
    
    for(auto it=homeSpan.controllerListBegin(); it!=homeSpan.controllerListEnd(); ++it){  // loop over each controller
    
      Serial.printf("Admin=%d",it->isAdmin());    // indicate if controller has admin permissions

      Serial.printf("  ID=");                     // print the 36-byte Device ID of the controller
      for(int i=0;i<36;i++)
        Serial.printf("%02X",it->getID()[i]);
    
      Serial.printf("  LTPK=");                   // print the 32-byte Long-Term Public Key of the controller)
      for(int i=0;i<32;i++)
        Serial.printf("%02X",it->getLTPK()[i]);
    
      Serial.printf("\n");
    }
    ```
    </details>

* `Span& enableWatchdog(uint16_t nSeconds)`
  * creates a HomeSpan *task watchdog* that triggers a reboot of the device if the HomeSpan `poll()` function is not run at least once every *nSeconds*
    * *nSeconds* must be equal to, or greater than, the ESP32 default task watchdog timeout period specified in the IDF macro `CONFIG_ESP_TASK_WDT_TIMEOUT_S` (typically 5 seconds)
    * if *nSeconds* is set to a duration less than `CONFIG_ESP_TASK_WDT_TIMEOUT_S`, or if it is left blank, the timeout duration will be set to `CONFIG_ESP_TASK_WDT_TIMEOUT_S`
  * enabling the HomeSpan task watchdog timer does not alter whether any other tasks, including the ESP32's IDLE tasks, are, or are not, also subscribed to the task watchdog timer
  * calling `enableWatchdog(nSeconds)` when the HomeSpan watchdog has already been enabled with a different value of *nSeconds* changes the timeout duration to the new value of *nSeconds* specified 
  * note the ESP32 task watchdog timer only supports a single timeout duration for all tasks subscribing to the task watchdog.  When the HomeSpan watchdog is enabled, *nSeconds* will therefore be used as the new timeout duration for any and all other tasks (including any of the ESP32's IDLE tasks) that are also subscribed to the tasks watchdog timer
  * see the [HomeSpan Watchdog Timer](WDT.md) page for details

* `void disableWatchdog()`
  * disables the HomeSpan *task watchdog*, if it has been enabled, else does nothing
  * has no impact any other tasks that may also be subscribed to the task watchdog timer
  * has no impact on the timeout duration
  * see the [HomeSpan Watchdog Timer](WDT.md) page for details

* `void resetWatchdog()`
  * resets the HomeSpan watchdog timer (if it has been enabled) and then pauses for 1 ms by calling `vTaskDelay(1)` to yield and allow other tasks of an equal or lower priority acquire processing time
  * users generally **do not** need to call this method since HomeSpan already does so whenever the `poll()` function runs
  * this is needed **only** if you enable the HomeSpan watchdog **and** you add code to a HomeSpan sketch that blocks the `poll()` function or prevents it from running for extended periods of time
  * see the [HomeSpan Watchdog Timer](WDT.md) page for details
 
---

The following **optional** `homeSpan` methods are for creating and managing multi-threaded sketches:
 
* `void autoPoll(uint32_t stackSize, uint32_t priority, uint32_t cpu)`
 
  * an *optional* method to create a separate task that repeatedly calls `poll()` in the background.  This frees up the Ardino `loop()` method for any user-defined code to run in parallel that would otherwise block, or be blocked by, calling `poll()` in the `loop()` method.  Parameters, and their default values if unspecified, are as follows:
 
    * *stackSize* - size of stack, in bytes, used by the polling task.  Default=8192 if unspecified
    * *priority* - priority at which task runs.  Minimum is 1.  Maximum is typically 24, but it depends on how the ESP32 operating system is configured. If you set it to an arbitrarily high value (e.g. 999), it will be set to the maximum priority allowed.  Default=1 if unspecified
    * *cpu* - specifies the CPU on which the polling task will run.  Valid values are 0 and 1.  This parameter is ignored on single-cpu boards.  Default=0 if unspecified
  * if used, **must** be placed in a sketch as the last line in the Arduino `setup()` method
  * HomeSpan will throw and error and halt if both `poll()`and `autoPoll()` are used in the same sketch - either place `poll()` in the Arduino `loop()` method **or** place `autoPoll()` at the the end of the Arduino `setup()` method
  * if this method is used, and you have no need to add your own code to the main Arduino `loop()`, you can safely skip defining a blank `void loop(){}` function in your sketch
 
* `TaskHandle_t getAutoPollTask()`
  * returns the task handle for the Auto Poll Task, or NULL if Auto Polling has not been used
   
* `homeSpanPAUSE`
  * when called, this **MACRO** waits for the current iteration of HomeSpan's polling task to complete and then pauses that process so you can separately call HomeSpan functions from your own thread, typically the main Arduino `loop`
  * allows you to safely read and write values of Characteristics using `setVal` and `getVal` without worrying about race conditions that would have occured if HomeSpan's polling function were running while you were trying to change Characteristics
  * pausing lasts until the end of the scope of the code block in which the `homeSpanPAUSE` macro was called, after which HomeSpan's polling process automatically resumes normal operations
  * **warning:** this macro should only be used from a thread that is distinct from HomeSpan's polling process.  **DO NOT** use this macro from within any code that is managed by the HomeSpan polling process, which is basically all the `update`, `loop` and other methods you created inside your SpanService structures.  However, you **CAN** call these methods directly from a separate thread, provided that you first call the `homeSpanPAUSE` macro
 
* `homeSpanRESUME`
  * call this *optional* **MACRO** if you want to prematurely resume HomeSpan polling after it has been paused via the `homeSpanPAUSE` macro but before reaching the end of the scope of the code block (at which point HomeSpan polling automatically resumes, as noted above)
  * this macro can only be called **AFTER** the `homeSpanPAUSE` macro has already been called, and it must be from within the same scope of the code block (or sub-block)
  * you will get a compilation error if you try to use this macro before calling `homeSpanPAUSE` or if it is not called within the same scope of the code block
  * it is okay to call `homeSpanRESUME` multiple times after calling `homeSpanPAUSE`.  The first instance restarts the HomeSpan polling process; subsequent instances are ignored
 
* `std::shared_mutex& getMutex()`
  * returns a reference to the *shared_mutex* used by the `homeSpanPAUSE` and `homeSpanRESUME` macros to lock and unlock the HomeSpan polling thread
  * only needed for advanced users who want to manually lock and unlock the HomeSpan polling thread using their own logic instead of simply calling these macros
 
A fully worked example showing how to use `autoPoll()` and `homeSpanPAUSE` to automaticlly flip the power of a Simple LightBulb Accessory from a separate thread can be found in the Arduino IDE under [*File → Examples → HomeSpan → Other Examples → MultiThreading*](../examples/Other%20Examples/MultiThreading).
 
## *SpanAccessory(uint32_t aid)*

Creating an instance of this **class** adds a new HAP Accessory to the HomeSpan HAP Database.

* every HomeSpan sketch requires at least one Accessory
* a sketch can contain a maximum of 150 Accessories per sketch (if exceeded, a runtime error will the thrown and the sketch will halt)
* there are no associated methods
* the argument *aid* is optional:
  * if specified and *not* zero, the Accessory ID is set to *aid*
  * if unspecified, or equal to zero, the Accessory ID will be set to one more than the ID of the previously-instantiated Accessory, or to 1 if this is the first Accessory
  * the first Accessory instantiated must always have an ID=1 (which is the default if *aid* is unspecified)
  * setting the *aid* of the first Accessory to anything but 1 throws an error during initialization
* you must call `homeSpan.begin()` before instantiating any Accessories
* example: `new SpanAccessory();`

The following methods are supported:

* `uint32_t getAID()`
  * returns the Accessory ID (AID)
  
## *SpanService()*

This is a **base class** from which all HomeSpan Services are derived, and should **not** be directly instantiated.  Rather, to create a new Service instantiate one of the HomeSpan Services defined in the [Service](ServiceList.md) namespace.  No arguments are needed.

* instantiated Services are added to the HomeSpan HAP Database and associated with the last Accessory instantiated
* instantiating a Service without first instantiating an Accessory throws an error during initialization
* example: `new Service::MotionSensor();`

The following methods are supported:

* `SpanService *setPrimary()`
  * specifies that this is the primary Service for the Accessory.  Returns a pointer to the Service itself so that the method can be chained during instantiation 
  * example: `(new Service::Fan)->setPrimary();`
  * note though this functionality is defined by Apple in HAP-R2, it seems to have been deprecated and no longer serves any purpose or has any affect on the Home App
  
* `SpanService *setHidden()`
  * specifies that this is hidden Service for the Accessory.  Returns a pointer to the Service itself so that the method can be chained during instantiation.
  * example: `(new Service::Fan)->setHidden();`
  * note though this functionality is defined by Apple in HAP-R2, it seems to have been deprecated and no longer serves any purpose or has any affect on the Home App
  
* `SpanService *addLink(SpanService *svc)`
  * adds *svc* as a Linked Service.  Returns a pointer to the calling Service itself so that the method can be chained during instantiation
  * note that Linked Services are only applicable for select HAP Services.  See Apple's HAP-R2 documentation for full details
  * example: `(new Service::Faucet)->addLink(new Service::Valve)->addLink(new Service::Valve);` (links two Valves to a Faucet)

* `vector<T> getLinks<T=SpanService *>(const char *serviceName=NULL)`
  * template function that returns a vector of pointers to Services that were added using `addLink()`
    * if template parameter, *T*, is left blank, the elements of the returned vector will be of type *SpanService \**
    * if template parameter, *T*, is specified, the elements of the returned vector will be cast into type *T*
  * if *serviceName* is specified, only those services matching *serviceName* will be included in the return vector
    * *serviceName* must be one of HomeSpan's built-in Services (e.g. "Valve")
    * if *serviceName* is left blank or set to NULL, all services will be included in the return vector
  * this function is useful for creating loops that iterate over all linked Services
  * example: from within a Faucet Service containing linked Valves defined in *MyValveService*, use `for(auto valve : getLinks<MyValveService *>()) { if(valve->active->getVal()) ... }` to check which Valves are active
  
* `virtual boolean update()`
  * HomeSpan calls this method upon receiving a request from a HomeKit Controller to update one or more Characteristics associated with the Service.  Users should override this method with code that implements that requested updates using one or more of the SpanCharacteristic methods below.  Method **must** return *true* if update succeeds, or *false* if not.
  
* `virtual void loop()`
  * HomeSpan calls this method every time `homeSpan.poll()` is executed.  Users should override this method with code that monitors for state changes in Characteristics that require HomeKit Controllers to be notified using one or more of the SpanCharacteristic methods below.
  
* `virtual void button(int pin, int pressType)`
  * HomeSpan calls this method whenever a SpanButton() object associated with the Service is triggered.  Users should override this method with code that implements any actions to be taken in response to the SpanButton() trigger using one or more of the SpanCharacteristic methods below.
    * *pin* - the ESP32 pin associated with the SpanButton() object
    * *pressType* - 
      * 0=single press (SpanButton::SINGLE)
      * 1=double press (SpanButton::DOUBLE)
      * 2=long press (SpanButton::LONG)
     
* `uint32_t getIID()`
  * returns the IID of the Service

* `uint32_t getAID()`
  * returns the AID of the Accessory to which the Service belongs
      
## *SpanCharacteristic(value [,boolean nvsStore])*
  
This is a **base class** from which all HomeSpan Characteristics are derived, and should **not** be directly instantiated.  Rather, to create a new Characteristic instantiate one of the HomeSpan Characteristics defined in the [Characteristic](ServiceList.md) namespace.

* instantiated Characteristics are added to the HomeSpan HAP Database and associated with the last Service instantiated
* instantiating a Characteristic without first instantiating a Service throws an error during initialization
* the first argument optionally allows you to set the initial *value* of the Characteristic at startup using the following formats:
  * for NUMERIC Characteristics, *value* can be any integer or decimal numeric type, such as `boolean`, `int`, `uint64_t`, `double`, etc.  HomeSpan will automatically cast *value* into a variable with the correct numerical precision for the Characteristic specified
  * for STRING Characteristics, *value* must be either of the type `char *`, or a literal quote-enclosed UTF-8 string
  * for TLV8 Characteristics, *value* must be of the type `TLV8`
  * for DATA Characteristics, *value* must be a brace-enclosed *pair* of the form `{uint8_t *data, size_t len}`, where *len* specifies the length of the size of the byte-array *data*
* if *value* is not specified, HomeSpan will supply a reasonable default for the Characteristic
* for numerical Characteristics, throws a runtime warning if *value* is outside of the min/max range for the Characteristic, where min/max is either the HAP default, or any new values set via a call to `setRange()`
* the second optional argument, if set to `true`, instructs HomeSpan to save updates to this Characteristic's value in the device's non-volative storage (NVS) for restoration at startup if the device should lose power.  If not specified, *nvsStore* will default to `false` (no storage)
* examples:
  * `new Characteristic::Brightness();`           Brightness initialized to default value
  * `new Characteristic::Brightness(50);`         Brightness initialized to 50
  * `new Characteristic::Brightness(50,true);`    Brightness initialized to 50; updates to the value are saved to, and restored from, NVS

#### The following methods are supported for numerical-based Characteristics (e.g. *int*, *float*...):

* `type T getVal<T>()`
  * a template method that returns the **current** value of a numerical-based Characteristic, after casting into the type *T* specified (e.g. *int*, *double*, etc.).  If template parameter is excluded, value will be cast to *int*.
  * example with template specified: `double temp = Characteristic::CurrentTemperature->getVal<double>();`
  * example with template excluded : `int tilt = Characteristic::CurrentTiltAngle->getVal();`

* `type T getNewVal<T>()`
  * a template method that returns the desired **new** value to which a HomeKit Controller has requested the Characteristic be updated.  Same casting rules as for `getVal<>()`
  * only applicable when called from within the `update()` loop of a **SpanService** (if called outside of the `update()` loop, the return value is that same as calling `getVal<>()`)
    
* `void setVal(value [,boolean notify])`
  * sets the value of a numerical-based Characteristic to *value*, and, if *notify* is set to true, notifies all HomeKit Controllers of the change.  The *notify* flag is optional and will be set to true if not specified.  Setting the *notify* flag to false allows you to update a Characateristic without notifying any HomeKit Controllers, which is useful for Characteristics that HomeKit automatically adjusts (such as a countdown timer) but will be requested from the Accessory if the Home App closes and is then re-opened
  * works with any integer, boolean, or floating-based numerical *value*, though HomeSpan will convert *value* into the appropriate type for each Characteristic (e.g. calling `setValue(5.5)` on an integer-based Characteristic results in *value*=5)
  * throws a runtime warning if *value* is outside of the min/max range for the Characteristic, where min/max is either the HAP default, or any new min/max range set via a prior call to `setRange()`
  * note that *value* is **not** restricted to being an increment of the step size; for example it is perfectly valid to call `setVal(43.5)` after calling `setRange(0,100,5)` on a floating-based Characteristic even though 43.5 does does not align with the step size specified.  The Home App will properly retain the value as 43.5, though it will round to the nearest step size increment (in this case 45) when used in a slider graphic (such as setting the temperature of a thermostat)
  * throws a runtime warning if called from within the `update()` routine of a **SpanService** *and* `isUpdated()` is *true* for the Characteristic (i.e. it is being updated at the same time via the Home App), *unless* you are changing the value of a Characteristic in response to a *write-response* request from HomeKit (typically used only for certain TLV-based Characteristics)
  * note this method can be used to update the value of a Characteristic even if the Characteristic is not permissioned for event notifications (EV), in which case the value stored by HomeSpan will be updated but the Home App will *not* be notified of the change


* `SpanCharacteristic *setRange(min, max, step)`
  * overrides the default HAP range for a Characteristic with the *min*, *max*, and *step* parameters specified
  * *step* is optional; if unspecified (or set to a non-positive number), the default HAP step size remains unchanged
  * works with any integer or floating-based parameters, though HomeSpan will recast the parameters into the appropriate type for each Characteristic (e.g. calling `setRange(50.5,70.3,0.5)` on an integer-based Characteristic results in *min*=50, *max*=70, and *step*=0)
  * an error is thrown if:
    * called on a Characteristic that does not suport range changes, or
    * called more than once on the same Characteristic
  * returns a pointer to the Characteristic itself so that the method can be chained during instantiation
  * example: `(new Characteristic::Brightness(50))->setRange(10,100,5);`
  
* `SpanCharacteristic *setValidValues(int n, [int v1, int v2 ...])`
  * overrides the default HAP Valid Values for Characteristics that have specific enumerated Valid Values with a variable-length list of *n* values *v1*, *v2*, etc.
  * works on Characteristics with UINT8, UINT16, UINT32, and INT formats only
    * a warning message is thrown, and the request is ignored, if this method is called on a Characteristic with any other format
  * returns a pointer to the Characteristic itself so that the method can be chained during instantiation
  * example: `(new Characteristic::SecuritySystemTargetState())->setValidValues(3,0,1,3);` creates a new Valid Value list of length=3 containing the values 0, 1, and 3.  This has the effect of informing HomeKit that a SecuritySystemTargetState value of 2 (Night Arm) is not valid and should not be shown as a choice in the Home App

#### The following methods are supported for string-based Characteristics (i.e. a null-terminated C-style array of characters):

* `char *getString()`
  * equivalent to `getVal()`, but used exclusively for string-characteristics (i.e. a null-terminated array of characters)
  
* `char *getNewString()`
  * equivalent to `getNewVal()`, but used exclusively for string-characteristics (i.e. a null-terminated array of characters)

* `void setString(const char *value [,boolean notify])`
  * equivalent to `setVal(value)`, but used exclusively for string-characteristics (i.e. a null-terminated array of characters)

* `SpanCharacteristic *setMaxStringLength(uint8_t n)`
  * changes the maximum allowed length of a string-characteristic from the HAP default (64) to *n*
  * note the Home App seems to properly process strings that exceed 64 characters without needing to reset the maximum length
  * this method is included in HomeSpan only for completeness with HAP - it is likely not needed
  * returns a pointer to the Characteristic itself so that the method can be chained during instantiation
 
#### The following methods are supported for DATA (i.e. byte-array) Characteristics:

* `size_t getData(uint8_t *data, size_t len)`
  * similar to `getVal()`, but exclusively used for byte-array Characteristics
  * fills byte array *data*, of specified size *len*, with all bytes "encoded" as the current value of the Characteristic
  * returns the total number of bytes encoded in the Characteristic
  * if *len* is less than the total number of bytes encoded, no data is extracted (i.e. *data* is unmodified) and a warning message is thrown indicating that the size of the *data* array is insufficient to extract all the bytes encoded in the Characteristic
  * setting *data* to NULL returns the total number of bytes encoded without extracting any data.  This can be used to help create a *data* array of sufficient size in advance of extracting the data
  * note: byte-array Characteristics are encoded and transmitted as base-64 strings.  HomeSpan automatically peforms all encoding and decoding between this format and the specified byte arrays.  But when output to the Serial Monitor using the 'i' CLI command, the value of byte-array Characteristics are displayed in their base-64 string format (only the first 32 characters are shown), since base-64 is the representation that is actually transmitted to and from HomeKit
  * a warning message is thrown if the value stored in the Characteristic is not in base-64 format
  
* `size_t getNewData(uint8_t *data, size_t len)`
  * similar to `getData()`, but fills byte array *data*, of specified size *len*, with bytes based on the desired **new** value to which a HomeKit Controller has requested the Characteristic be updated

* `void setData(uint8_t *data, size_t len [,boolean notify])`
  * similar to `setVal()`, but exclusively used for byte-array Characteristics
  * updates the Characteristic by "filling" it with *len* bytes from bytes array *data*

#### The following methods are supported for TLV8 (structured-data) Characteristics:

* `size_t getTLV(TLV8 &tlv)`
  * similar to `getVal()`, but exclusively used for TLV8 Characteristics
  * fills TLV8 structure *tlv* with TLV8 records from the current value of the Characteristic
  * returns the total number of bytes encoded in the Characteristic
  * if *tlv8* is not empty, TLV8 records from the Characteristic will be appended to any existing records
  * similar to DATA Characteristics, TLV8 Characteristics are stored and transmitted as base-64 strings
  * a warning message is thrown if the value stored in the Characteristic is not in base-64 format, or does not appear to contain TLV8 records 
  
* `size_t getNewTLV(TLV8 &tlv)`
  * similar to `getTLV()`, but fills TLV8 structure *tlv* with TLV8 records based on the desired **new** value to which a HomeKit Controller has requested the Characteristic be updated

* `void setTLV(TLV8 &tlv [,boolean notify])`
  * similar to `setVal()`, but exclusively used for TLV8 Characteristics
  * updates the Characteristic by packing the TLV8 structure *tlv* into a byte array and then encoding it in base-64 for storage as a string
    
* `NULL_TLV`
  * this is not a method, but rather a static HomeSpan constant defined as an empty TLV8 object.  It may used a placeholder wherever an empty TLV8 object is needed, such as when you want to instantiate a TLV8 Characteristic with *nvsStore=true*, but you don't yet want set the TLV8 value
  * example: `new Characteristic::DisplayOrder(NULL_TLV,true);`

* see the [TLV8 Characteristics](TLV8.md) page for complete details on how to read/process/create TLV8 Objects using HomeSpan's TLV8 Library.

#### The following methods are supported for all Characteristics:

* `boolean updated()`
  * returns *true* if a HomeKit Controller has requested an update to the value of the Characteristic, otherwise *false*.  The requested value itself can retrieved with `getNewVal<>()` or `getNewString()`

* `int timeVal()`
  * returns time elapsed (in millis) since value of the Characteristic was last updated (whether by `setVal()`, `setString()` or as the result of a successful update request from a HomeKit Controller)

* `SpanCharacteristic *setPerms(uint8_t perms)`
  * changes the default permissions for a Characteristic to *perms*, where *perms* is an additive list of permissions as described in HAP-R2 Table 6-4.  Valid values are PR, PW, EV, AA, TW, HD, and WR
  * returns a pointer to the Characteristic itself so that the method can be chained during instantiation
  * example: `(new Characteristic::IsConfigured(1))->setPerms(PW+PR+EV);`
   
* `SpanCharacteristic *addPerms(uint8_t perms)`
  * adds new permissions, *perms*, to the default permissions for a Characteristic, where *perms* is an additive list of permissions as described in HAP-R2 Table 6-4.  Valid values are PR, PW, EV, AA, TW, HD, and WR
  * returns a pointer to the Characteristic itself so that the method can be chained during instantiation
  * example: `(new Characteristic::IsConfigured(1))->addPerms(PW);`

* `SpanCharacteristic *removePerms(uint8_t perms)`
  * removes permissions, *perms*, from the default permissions of a Characteristic, where *perms* is an additive list of permissions as described in HAP-R2 Table 6-4.  Valid values are PR, PW, EV, AA, TW, HD, and WR
  * returns a pointer to the Characteristic itself so that the method can be chained during instantiation
  * example: `(new Characteristic::ConfiguredName("HDMI 1"))->removePerms(PW);`

* `SpanCharacteristic *setDescription(const char *desc)`
  * adds an optional description, *desc*, to a Characteristic, as described in HAP-R2 Table 6-3
  * this field is generally used to provide information about custom Characteristics, but does not appear to be used in any way by the Home App
  * returns a pointer to the Characteristic itself so that the method can be chained during instantiation
  * example: `(new Characteristic::MyCustomChar())->setDescription("Tuner Frequency");`

* `SpanCharacteristic *setUnit(const char *unit)`
  * adds or overrides the *unit* for a Characteristic, as described in HAP-R2 Table 6-6
  * returns a pointer to the Characteristic itself so that the method can be chained during instantiation
  * example: `(new Characteristic::RotationSpeed())->setUnit("percentage");`

* `uint32_t getIID()`
  * returns the IID of the Characteristic

* `uint32_t getAID()`
  * returns the AID of the Accessory to which the Characteristic belongs

* `boolean foundIn(const char *getCharList)`
  * returns *true* if the AID/IID pair for the Characteristic is found in *getCharList*, else returns *false*
  * *getCharList* is typically passed by HomeSpan to an optional user-defined callback function as specified in *homeSpan.setGetCharacteristicsCallback()*
  
### *SpanButton(int pin, uint16_t longTime, uint16_t singleTime, uint16_t doubleTime, boolean (\*triggerType)(int))*

Creating an instance of this **class** attaches a pushbutton handler to the ESP32 *pin* specified.

* instantiated Buttons are associated with the last Service instantiated
* instantiating a Button without first instantiating a Service throws an error during initialization

The first argument is required; the rest are optional:
 
* *pin* - the ESP32 pin to which the button is connected 
* *longTime* - the minimum time (in millis) required for the button to be pressed and held to trigger a Long Press (default=2000 ms)
* *singleTime* - the minimum time (in millis) required for the button to be pressed to trigger a Single Press (default=5 ms)
* *doubleTime* -  the maximum time (in millis) allowed between two Single Presses to qualify as a Double Press (default=200 ms)
* *triggerType* - pointer to a boolean function that accepts a single *int* argument and returns `true` or `false` depending on whether or not a "press" has been triggered on the *pin* number passed to the *int* argument.  For ease of use, you may simply choose from the following built-in functions:
  * `SpanButton::TRIGGER_ON_LOW` - triggers when *pin* is driven LOW.  Suitable for buttons that connect *pin* to GROUND (this is the default when *triggerType* is not specified)
 
  * `SpanButton::TRIGGER_ON_HIGH` - triggers when *pin* is driven HIGH.  Suitable for buttons that connect *pin* to VCC (typically 3.3V)
  * `SpanButton::TRIGGER_ON_TOUCH`- uses the device's touch-sensor peripheral to trigger when a sensor attached to *pin* has been touched (not available on ESP32-C3 devices)
 
When any of these built-in functions are selected (or *triggerType* is left unspecified and the default is used), SpanButton will automatically configure the *pin* as needed upon instantiation.
 
Alternatively, you can set *triggerType* to any user-defined function of the form `boolean(int arg)` and provide your own logic for determining whether a trigger has occured on the specified *pin*, which is passed through to your function as *arg*. In this case *arg* can either represent an actual device pin, or simply be an arbitrary *int* your function utilizes, such as the virtual pin number on a multiplexer.  Note: if you specify your own function for *triggerType* you also must include in your sketch any code needed to initialize the logic or configure whatever resource *triggerType* is utilizing (such as a pin multiplexer).

For convenience, a second form of the *SpanButton()* constructor is also provided:
  * `SpanButton(int pin, boolean (*triggerType)(int), uint16_t longTime=2000, uint16_t singleTime=5, uint16_t doubleTime=200)`
    * this allows you to set just the *pin* and *triggerType* while leaving the remaining parameters at their default values
 
#### Trigger Rules ###
* If button is pressed and continuously held, a Long Press will be triggered every longTime ms until the button is released
* If button is pressed for more than singleTime ms but less than longTime ms and then released, a Single Press will be triggered, UNLESS the button is pressed a second time within doubleTime ms AND held again for at least singleTime ms, in which case a DoublePress will be triggered;  no further events will occur until the button is released
* If singleTime>longTime, only Long Press triggers can occur
* If doubleTime=0, Double Presses cannot occur
 
#### Usage ####
HomeSpan automatically calls the `button(int pin, int pressType)` method of a Service upon a trigger event in any SpanButton associated with that Service, where *pin* is the ESP32 pin to which the pushbutton is connected, and *pressType* is an integer that can also be represented by the enum constants indicated:
  * 0=single press (`SpanButton::SINGLE`)
  * 1=double press (`SpanButton::DOUBLE`)
  * 2=long press (`SpanButton::LONG`)  
  
HomeSpan will report a warning, but not an error, during initialization if the user had not overridden the virtual button() method for a Service contaning one or more Buttons; triggers of those Buttons will simply ignored.
 
When using one or more Touch Sensors, HomeSpan automatically calibrates the threshold at which they are triggered by polling the baseline sensor reading upon instantiation of first SpanButton of type `SpanButton::TRIGGER_ON_TOUCH`.  For ESP32 devices, the threshold is set to 50% of the baseline value since triggers occur when a sensor value falls *below* the threhold level.  For ESP32-S2 and ESP32-S3 devices, the threshold is set to 200% of the baseline value since triggers occur when a sensor value rises *above* the threhold level.  Normally HomeSpan's auto calibration will result in accurate detection of SINGLE, DOUBLE, and LONG presses of touch sensors.  However, if needed you can set your own threshold value using the following class-level method:

 * `void SpanButton::setTouchThreshold(uint32_t thresh)`
   * sets the threshold value above (for ESP32 devices) or below (for ESP32-S2 and ESP32-S3 devices) which touch sensors are triggered to *thresh*
   * the threshold specified is considered global and used for *all* SpanButton instances of type `SpanButton::TRIGGER_ON_TOUCH`
   * this method can be called either before or after SpanButtons are created
 
Note the Arduino-ESP32 core and ESP-IDF include other functions you can use to more finely tune the ESP32 touch-sensor peripheral logic if needed.

### *SpanToggle(int pin, boolean (\*triggerType)(int)=PushButton::TRIGGER_ON_LOW, uint16_32 toggleTime=5)*
 
Creating an instance of this **class** attaches a toggle-switch handler to the ESP32 *pin* specified.  This is a child class of *SpanButton* and thus derives all of the same functionality.  For example, you can set *triggerType* to PushButton::TRIGGER_ON_HIGH, create your own trigger function, etc. However, instead of HomeSpan calling `button(int pin, int pressType)` when a pushbutton is "pressed," HomeSpan calls the same `button()` method when the switch is "toggled" from one position to another.  In this case the parameter *pressType* that is passed into `button()` has a different set of enumerations:
  * 3=switch is closed (`SpanToggle::CLOSED`)
  * 4=switch is open (`SpanToggle::OPEN`)
 
Note there are no *singleTime*, *longTime*, or *doubleTime* parameters in the constructor since you can't single-press, double-press, or long-press a toggle switch.  Instead, the constructor supports the single parameter *toggleTime* (default=5ms if left unspecified) that sets the minimum time at which the switch needs to be moved to the closed position in order to trigger a call to the `button()` method.  This effectively "debounces" the toggle switch.
 
SpanToggle also supports the following additional method:
 
 * `int position()`
   * returns the current position of the toggle switch (i.e. SpanToggle::CLOSED or SpanToggle::OPEN)
   * is equivalent to the *pressType* parameter passed to the `button()` method, but can be called from anywhere in a sketch
   * useful for reading the initial state of a contact switch upon start-up so that the initial value of Characteristic::ContactSensorState can be set accordingly
   * example `sensorState=new Characteristic::ContactSensorState(toggleSwitch->position()==SpanToggle::OPEN);`
 
### *SpanUserCommand(char c, const char \*desc, void (\*f)(const char \*buf [,void \*obj]) [,void \*userObject])*

Creating an instance of this **class** adds a user-defined command to the HomeSpan Command-Line Interface (CLI), where:

  * *c* is the single-letter name of the user-defined command
  * *desc* is a description of the user-defined command that is displayed when the user types '?' into the CLI
  * *f* is a pointer to a user-defined function that is called when the command is invoked.  Allowable forms for *f* are:
    1. `void f(const char *buf)`, or
    1. `void f(const char *buf, void *obj)`
  * *userObject* is a pointer to an arbitrary object HomeSpan passes to the function *f* as the second argument when the second form of *f* is used.  Note it is an error to include *userObject* when the first form of *f* is used, and it is similarly an error to exclude *userObject* when the second form of *f* is used

To invoke your custom command from the CLI, preface the single-letter name *c* with '@'.  This allows HomeSpan to distinguish user-defined commands from its built-in commands.  For example,

```C++
new SpanUserCommand('s', "save current configuration", saveConfig);
...
void saveConfig(const char *buf){ ... };
```

would add a new command '@s' to the CLI with description "save current configuration" that will call the user-defined function `void saveConfig(const char *buf)` when invoked.  The argument *buf* points to an array of all characters typed into the CLI after the '@'.  This allows the user to pass arguments from the CLI to the user-defined function.  For example, typing '@s123' into the CLI sets *buf* to "s123" when saveConfig is called.

In the second form of the argument, HomeSpan will pass an additional object to your function *f*.  For example,

```C++
struct myConfigurations[10];
new SpanUserCommand('s', "<n> save current configuration for specified index, n", saveConfig, myConfigurations);
...
void saveConfig(const char *buf, void *obj){ ... do something with myConfigurations ... };
```

might be used to save all the elements in *myArray* when called with just the '@s' command, and perhaps save only one element based on an index added to the command, such as '@s34' to save element 34 in *myArray*.  It is up to the user to create all necessary logic within the function *f* to parse and process the full command text passed in *buf*, as well as act on whatever is being passed via *obj.

To create more than one user-defined command, simply create multiple instances of SpanUserCommand, each with its own single-letter name.  Note that re-using the same single-letter name in an instance of SpanUserCommand over-rides any previous instances using that same letter.

## Custom Characteristics and Custom Services Macros

### *CUSTOM_CHAR(name,uuid,perms,format,defaultValue,minValue,maxValue,staticRange)*
### *CUSTOM_CHAR_STRING(name,uuid,perms,defaultValue)*
### *CUSTOM_CHAR_DATA(name,uuid,perms)*
### *CUSTOM_CHAR_TLV8(name,uuid,perms)*

Creates a custom Characteristic that can be added to any Service.  Custom Characteristics are generally ignored by the Home App but may be used by other third-party applications (such as *Eve for HomeKit*).  The first form should be used create numerical Characterstics (e.g., UINT8, BOOL...); the second form is used to STRING-based Characteristics; the third form is used for DATA-based (i.e. byte-array) Characteristics; and the fourth form is used for TLV8-based (i.e. *structured* byte-array) Characteristics  Parameters are as follows (note that quotes should NOT be used in any of the macro parameters, except for *defaultValue* when applied to a STRING-based Characteristic):

* *name* - the name of the custom Characteristic.  This will be added to the Characteristic namespace so that it is accessed the same as any HomeSpan Characteristic.  Use UTF-8 coded string for non-ASCII characters.
* *uuid* - the UUID of the Characteristic as defined by the manufacturer.  Must be either:
  * *exactly* 36 characters of the form XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX, where *X* represent a valid hexidecimal digit, or
  * a single hexidecimal number of the form XXXXXXXX with *8 digits or less*, and no leading zeros
* *perms* - additive list of permissions.  Valid values are PR, PW, EV, AA, TW, HD, and WR (e.g. PR+PW+EV)
* *format* - for numerical Characteristics, specifies the number format.  Valid value are BOOL, UINT8, UINT16, UNIT32, UINT64, INT, and FLOAT.  Not applicable for the STRING, DATA, or TLV8 Characteristic macros
* *defaultValue* - specifies the default value of the Characteristic when not defined during instantiation.  Not applicable for the DATA or TLV7 Characteristic macros.
* *minValue* - specifies the default minimum range for a valid value, which may be able to be overriden by a call to `setRange()`.  Not applicable for the STRING, DATA or TLV8 Characteristic macros
* *minValue* - specifies the default minimum range for a valid value, which may be able to be overriden by a call to `setRange()`.  Not applicable for the STRING, DATA or TLV8 Characteristic macros
* *staticRange* - set to *true* if *minValue* and *maxValue* are static and cannot be overridden with a call to `setRange()`.  Set to *false* if calls to `setRange()` are allowed.  Not applicable for the STRING, DATA or TLV8 Characteristic macros

As an example, the first line below creates a custom Characteristic named "Voltage" with a UUID code that is recognized by the *Eve for HomeKit* app.  The parameters show that the Characteristic is read-only (PR) and notifications are enabled (EV).  The default range of allowed values is 0-240, with a default of 120.  The range *can* be overridden by subsequent calls to `setRange()`.  The second line below creates a custom read-only String-based Characteristic:

```C++
CUSTOM_CHAR(Voltage, E863F10A-079E-48FF-8F27-9C2605A29F52, PR+EV, UINT16, 120, 0, 240, false);
CUSTOM_CHAR_STRING(UserTag, AAAAAAAA-BBBB-AAAA-AAAA-AAAAAAAAAAAA, PR, "Tag 123");
...
new Service::LightBulb();
  new Characteristic::Name("Low-Voltage Lamp");
  new Characteristic::On(0);
  new Characteristic::Brightness(50);
  new Characteristic::Voltage(12);      // adds Voltage Characteristic and sets initial value to 12 volts
  new Characteristic::UserTag();        // adds UserTag Characteristic and retains default initial value of "Tag 123"
```

Note that Custom Characteristics must be created at the global level (i.e. not inside `setup()`) and prior to calling `homeSpan.begin()`

> Advanced Tip 1: When presented with an unrecognized Custom Characteristic, *Eve for HomeKit* helpfully displays a *generic control* allowing you to interact with any Custom Characteristic you create in HomeSpan.  However, since Eve does not recognize the Characteristic, it will only render the generic control if the Characteristic includes a **description** field, which you can add to any Characteristic using the `setDescription()` method described above.  You may also want to use `setUnit()` and `setRange()` so that the Eve App displays a control with appropriate ranges for your Custom Characteristic.
 
> Advanced Tip 2: The DATA format is not currently used by any native Home App Characteristic, though it is part of the HAP-R2 specifications.  This format is included in HomeSpan because other applications, such as *Eve for HomeKit* do use these types of Characteristics to create functionality beyond that of the Home App, and are thus provided for advanced users to experiment.
 
> Advanced Tip 3: When using multi-file sketches, the compiler will throw a "redefinition error" if you define the same Custom Characteristic in more than one file.  To avoid this error and allow the same Custom Characteristic to be used across more than one file, add the line `#define CUSTOM_CHAR_HEADER` *before* `#include "HomeSpan.h"` in each file containing a *duplicate* definition of a previously-defined Custom Characteristic.

### *CUSTOM_SERV(name,uuid)*

Creates a custom Service for use with third-party applications (such as *Eve for HomeKit*).  Custom Services will be displayed in the native Apple Home App with a Tile labeled "Not Supported", but otherwise the Service will be safely ignored by the Home App.  Parameters are as follows (note that quotes should NOT be used in either of the macro parameters):

* *name* - the name of the custom Service.  This will be added to the Service namespace so that it is accessed the same as any HomeSpan Service.  For example, if *name*="Vent", HomeSpan would recognize `Service::Vent` as a new service class
* *uuid* - the UUID of the Service as defined by the manufacturer.  Must be *exactly* 36 characters in the form XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX, where *X* represent a valid hexidecimal digit.  Leading zeros are required if needed as described more fully in HAP-R2 Section 6.6.1

Custom Services may contain a mix of both Custom Characteristics and standard HAP Characteristics, though since the Service itself is custom, the Home App will ignore the entire Service even if it contains some standard HAP Characterstics.  Note that Custom Services must be created prior to calling `homeSpan.begin()`

A fully worked example showing how to use both the ***CUSTOM_SERV()*** and ***CUSTOM_CHAR()*** macros to create a Pressure Sensor Accessory that is recognized by *Eve for HomeKit* can be found in the Arduino IDE under [*File → Examples → HomeSpan → Other Examples → CustomService*](../examples/Other%20Examples/CustomService).

## Other Macros

### *SPAN_ACCESSORY()* and *SPAN_ACCESSORY(NAME)*
 
A "convenience" macro that implements the following very common code snippet used when creating Accessories.  The last line is only included if *NAME* (a C-style string) has been included as an argument to the macro:

```C++
new SpanAccessory();
 new Service::AccessoryInformation();
 new Characteristic::Identify();
 new Characteristic::Name(NAME);   // included only in the second form of the macro
```

## User-Definable Macros
 
### *#define REQUIRED VERSION(major,minor,patch)*

If REQUIRED is defined in the main sketch *prior* to including the HomeSpan library with `#include "HomeSpan.h"`, HomeSpan will throw a compile-time error unless the version of the library included is equal to, or later than, the version specified using the VERSION macro.  Example:

```C++
#define REQUIRED VERSION(1,3,0)   // throws a compile-time error unless HomeSpan library used is version 1.3.0 or later
#include "HomeSpan.h"
```

---

[↩️](../README.md) Back to the Welcome page
