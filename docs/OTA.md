# Over-the-Air (OTA) Updates

HomeSpan integrates the ArduinoOTA library, which allows you to wirelessly update sketches directly from the Arduino IDE.  To save resources (as well as maximize security) HomeSpan does not enable OTA unless specifically requested.  To do so, simply call the method `homeSpan.enableOTA()` somewhere in your sketch *before* the call to `homeSpan.begin()`.

By default, HomeSpan requires the use of a password when updating a sketch via OTA from the Arduino IDE.  The default OTA password is "homespan-ota".  You can change this password from the [HomeSpan CLI](CLI.md) with the 'O' command.  Similar to a device's Setup Code, HomeSpan saves a non-recoverable hashed version of the OTA password you specify in its non-volatile storage (NVS).  If you forget the password you specified, you'll need to create a new one using this command. Alternatively, you can restore the default OTA password by fully erasing the NVS with the 'E' command.

Though not recommended, you can disable the requirement for a password by enabling OTA with *false* as the parameter as such: `homeSpan.enableOTA(false)`.  Use with caution!  Anyone who can access the device over your network will be able to upload a new sketch.

