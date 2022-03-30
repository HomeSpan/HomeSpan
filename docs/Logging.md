# Message Logging

HomeSpan includes three types of message logging:

* **HomeSpan Log Messages** - these are messages produced by HomeSpan and output to the Arduino Serial Monitor.  The messages are categorized according to 3 levels of detail

  * Level-0 messages contain all HomeSpan configuration data and some basic status information
  * Level-1 adds in a more verbose set of status messages
  * Level-2 further includes all HAP communication packages to and from the HomeSpan device.
  * Users can programmatically control which log messages are output to the Serial Monitor by setting the log level using the homeSpan method `setLogLevel(uint8_t level)` as described in the [HomeSpan API Reference](API.md).  Level-0 messages are always output.  Level-1 messages are only output if the log level is set to 1 or greater.  Level-2 messages are only output if the log level is set to to 2
  * The log level can also be changed dynamically via the Serial Monitor at any time via the 'L' as described in the [HomeSpan CLI](CLI.md).

1. **User Log Messages** - these are messages produced by the user and output to the Arduino Serial.  User can output messages using either the Arduino standard Serial.print(x) method, or the    



= top-level HomeSpan status messages, and any messages output by the user using Serial.print() or Serial.printf() (default)
1 = all HomeSpan status messages, and any LOG1() messages specified in the sketch by the user
2 = all HomeSpan status messages plus all HAP communication packets to and from the HomeSpan device, as well as all LOG1() and LOG2() messages specified in the sketch by the user




### *LOG1(X)* and *LOG2(X)*
### *LOG1(const char \*fmt, ...)* and *LOG2(const char \*fmt, ...)*

Displays user-defined log messages on the Arduino Serial Monitor according to the log level specified with `setLogLevel()`, or as specified at runtime with the 'L' command via the [HomeSpan CLI](CLI.md).  `LOG1()` messages will be output only if the log level is set to 1 or greater.  `LOG2()` messages will be output only if the log level is set to 2.
 
* In the first form (e.g. `LOG1(X)`), the macro calls `Serial.print(X)`.  The argument *X* can be any variable recognized by the Arduino `Serial.print()` function.  For example, `int val=255; LOG1(val);` outputs "255" to the Serial Monitor
* In the second form (e.g. `LOG1(const char *fmt, ...)`), the macro calls `Serial.printf(fmt, ...)` enabling you to create printf-style formatted output.  For example, `int val=255; LOG1("The value is: %X",val);` outputs the "The value is: FF" to the Serial Monitor 
* See [Example 9 - MessageLogging](https://github.com/HomeSpan/HomeSpan/blob/master/docs/Tutorials.md#example-9---messagelogging) for a tutorial sketch demonstrating these functions
 
### WEBLOG(const char *fmt, ...) 

---

[↩️](README.md) Back to the Welcome page


