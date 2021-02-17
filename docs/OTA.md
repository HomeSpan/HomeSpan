# Over-the-Air (OTA) Updates

HomeSpan integrates the ArduinoOTA library, which allows you to wirelessly update sketches directly from the Arduino IDE.  To save resources (as well as maximize security) HomeSpan does not enable OTA unless specifically requested.  To do so, simply call the method `homeSpan.enableOTA()` somewhere in your sketch *before* the call to `homeSpan.begin()`.

By default, HomeSpan requires the use of a password when updating a sketch via OTA from the Arduino IDE.  The default OTA password is "homespan-ota".  You can change this password from the [HomeSpan CLI](CLI.md) with the 'O' command.  Similar to a device's Setup Code, HomeSpan saves a non-recoverable hashed version of the OTA password you specify in its non-volatile storage (NVS).  If you forget the password you specified, you'll need to create a new one using this command. Alternatively, you can restore the default OTA password by fully erasing the NVS with the 'E' command.

Though not recommended, you can disable the requirement for a password by enabling OTA with *false* as the parameter as such: `homeSpan.enableOTA(false)`.  Use with caution!  Anyone who can access the device over your network will be able to upload a new sketch.

If OTA is enabled, HomeSpan will check that the sketch has been compiled with OTA partitions, and output a confirmation message to the Arduino Serial Monitor upon start-up immediately after establishing WiFi connectivity.  If OTA is enabled in a sketch but OTA partitions are not found, HomeSpan will instead report a warning that it can't start OTA.  Partition selection is found under the *Tools* menu of the Arduino IDE.  Note that depending on your specific ESP32 device, the Partition Scheme labeled "default" may bne configured with OTA partitions.  The easiest way to find out is to select that scheme and check for verification by HomeSpan.


