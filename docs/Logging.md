# Message Logging

### *LOG1(X)* and *LOG2(X)*
### *LOG1(const char \*fmt, ...)* and *LOG2(const char \*fmt, ...)*

Displays user-defined log messages on the Arduino Serial Monitor according to the log level specified with `setLogLevel()`, or as specified at runtime with the 'L' command via the [HomeSpan CLI](CLI.md).  `LOG1()` messages will be output only if the log level is set to 1 or greater.  `LOG2()` messages will be output only if the log level is set to 2.
 
* In the first form (e.g. `LOG1(X)`), the macro calls `Serial.print(X)`.  The argument *X* can be any variable recognized by the Arduino `Serial.print()` function.  For example, `int val=255; LOG1(val);` outputs "255" to the Serial Monitor
* In the second form (e.g. `LOG1(const char *fmt, ...)`), the macro calls `Serial.printf(fmt, ...)` enabling you to create printf-style formatted output.  For example, `int val=255; LOG1("The value is: %X",val);` outputs the "The value is: FF" to the Serial Monitor 
* See [Example 9 - MessageLogging](https://github.com/HomeSpan/HomeSpan/blob/master/docs/Tutorials.md#example-9---messagelogging) for a tutorial sketch demonstrating these functions
 
### WEBLOG(const char *fmt, ...) 

---

[↩️](README.md) Back to the Welcome page


