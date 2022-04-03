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

---

[↩️](README.md) Back to the Welcome page


