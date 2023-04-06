# Message Logging

HomeSpan includes a variety of message logs with different levels of verbosity, as well built-in methods to create your own log messages and web logs.

## HomeSpan Log Messages

HomeSpan log messages are typically output directly to the Arduino Serial Monitor with three possible levels of verbosity:

|Log Level|Output|
|---------|------|
|Level 0|HomeSpan configuration data and some basic status information|
|Level 1|Eveything in Level 0 plus additional and more verbose status messages|
|Level 2|Everything im Level 1 plus all HAP communication packages sent to and from the HomeSpan device|

You can set the *Log Level* in your sketch using the method `homeSpan.setLogLevel(uint8_t level)` as described in the [HomeSpan API Reference](API.md).  Level 0 messages are always output; Level 1 messages are only output if the *Log Level* is set to 1 or greater; and Level 2 messages are only output if the *Log Level* is set to to 2.  The *Log Level* can also be changed dynamically via the Serial Monitor at any time by typing either the 'L0', 'L1', or 'L2' as described in the [HomeSpan CLI](CLI.md).

## User-Defined Log Messages

You can add your own log messages to any sketch using HomeSpan's **LOG0()**, **LOG1()**, and **LOG2()** macros.  Messages created with these macros will be output to the Arduino Serial Monitor according the *Log Level* setting described above.  Each **LOGn()** macro (where n=\[0,2\]) is available in two flavors depending on the number of arguments specified:

* `LOGn(val)` - when only one argument is specified, HomeSpan outputs *val* using the standard Arduino `Serial.print(val)` method, which means *val* can be nearly any timvariable type.  The downside is you have no control over the format.  For example, `int n=255; LOG1(n);` outputs the number "255" to the Arduino Serial Monitor, provided that the *Log Level* is set to 1 or greater.

* `LOGn(const char *fmt, ...)` - when more than one argument is specified, HomeSpan outputs the message using the ESP32 `Serial.printf(fmt, ...)` method, which allows you to format messages with a variable number of arguments using standard C++ *printf* conventions.  For example, `int n=255; LOG2("The value is 0x%X",n);` outputs the message "The value is 0xFF" to the Arduino Serial Monitor, provided that the *Log Level* is set to 2.

See [Example 9 - MessageLogging](Tutorials.md#example-9---messagelogging) for a tutorial sketch demonstrating these macros.
 
## Web Logging 

In addition to logging messages to the Arduino Serial Monitor, HomeSpan can optionally serve a Web Log page at any page address you choose.  Since the Web Log is hosted as part of HomeSpan's HAP Server, its base address and port will be the same as that of your device.  For example, if your device name is *http<nolink>://homespan-4e8eb8504e59.local* (assuming port 80) and you choose "myLog" as the Web Log page address, it will be hosted at *http<nolink>://homespan-4e8eb8504e59.local/myLog*.

Also embedded in the HomeSpan's Web Log functionality is the ability to call an NTP time server to set the device clock.  This optional feature allows HomeSpan to create clock-based timestamps (e.g. *Sat Apr 16 19:48:41 2022*).

The HomeSpan Web Log page itself comprises two parts:
 
  * the top of the page provides HomeSpan-generated status information, such as the name of the device, total uptime since last reboot, and version numbers of the various software components
 
  * the bottom of the page posts messages you create using the **WEBLOG()** macro.  This macro comes only in the *printf*-style form `WEBLOG(const char *fmt, ...)`, similar to the second version of the LOG() macros described above.
 
Messages produced with WEBLOG() are *also* echoed to the Arduino Serial Monitor with the same priority as LOG1() messages, meaning they will be output to the Serial Monitor if the *Log Level* is set to 1 or greater.  The Web Log page displays messages in reverse-chronological order, supplemented with the following additional items:

* *Entry Number* - HomeSpan numbers each message, starting with 1 for the first message after rebooting
* *Up Time* - relative message time, in the form DDD:HH:MM:SS, starting at 000:00:00:00 after rebooting
* *Log Time* - absolute message time, in standard UNIX format, provided that Web Logging has been enabled with an NTP Time Server (see below)
* *Client* - the IP Address of the Client connected to HomeSpan at the time the WEBLOG() message was created.  Only applicable for messages produced within the `update()` method of a Service.  Client is otherwise set to '0.0.0.0' in all other instances
* *Message* - the text of the formatted message.  For example, `int ledNumber=5; WEBLOG("Request to turn LED %d OFF\n",ledNumber);` would produce the message "Request to turn LED 5 OFF"

To enable Web Logging (it's turned off by default), call the method `homeSpan.enableWebLog()`, as more fully described in the [HomeSpan API Reference](Reference.md), near the top of your sketch.  This method allows you to set:

* the total number of WEBLOG() messages to be stored - older messages are discarded in favor of newer ones once the limit you set is reached
* the URL of an NTP time server - this is optional and only needed if you want to set the clock of the device at start-up
* the time zone for the device - this is only needed if an NTP time server has been specified
* the URL of the Web Log page - if unspecified, HomeSpan will serve the Web Log at a page named "status"
 
Additional notes:
 
  * it is okay to include WEBLOG() messages in your sketch even if Web Logging is *not* enabled.  In such cases HomeSpan will not serve a Web Log page, but WEBLOG() messages will still be output to the Arduino Serial Monitor if the *Log Level* is set to 1 or greater
  * messages are **not** stored in NVS and are thus **not** saved between reboots
 
See [Example 19 - WebLog](Tutorials.md#example-19---weblog) for a tutorial sketch demonstrating the use of `homeSpan.enableWebLog()` and the WEBLOG() macro.
 
---

[↩️](../README.md) Back to the Welcome page


