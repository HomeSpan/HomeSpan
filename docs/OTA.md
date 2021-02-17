# Over-the-Air (OTA) Updates

HomeSpan supports Over-the-Air (OTA) updates, which allows you to *wirelessly* upload sketches directly from the Arduino IDE.  To activate this feature for your sketch, simply call the method `homeSpan.enableOTA()` prior to calling `homeSpan.begin()`.

When a HomeSpan sketch is run with OTA enabled, the device shows up as a "network" port that can be selected under *Tools → Port* in the Arduino IDE.  Once selected, the IDE will direct all uploads to the device via WiFi instead of looking for it on a serial port.  Note that you can upload via OTA even if your device is still connected to a serial port, but the IDE does not presently support multiple port connections at the same time and will automatically close the Serial Monitor upon selecting your device via a network port.

By default, HomeSpan requires the use of a password whenever you begin an OTA upload.  The default OTA password is "homespan-ota".  The Arduino will prompt you for this when you attempt the first OTA upload after opening the Arduino IDE, after which it will remember the password until the IDE is shut down.

You can change the password for a HomeSpan device from the [HomeSpan CLI](CLI.md) with the 'O' command.  Similar to a device's Setup Code, HomeSpan saves a non-recoverable hashed version of the OTA password you specify in non-volatile storage (NVS).  If you forget the password you specified, you'll need to create a new one using the 'O' command, or you can restore the default OTA password by fully erasing the NVS with the 'E' command.

> :exclamation: Though not recommended, you can override the requirement for a password by enabling OTA with *false* as the parameter as such: `homeSpan.enableOTA(false)`.  Use with caution!  Anyone who can access the device over your network will now be able to upload a new sketch.

Note that in in order for OTA to properly operate, your sketch must be compiled using by selecting a Parition Scheme under the *Tools* menut that includes OTA partitions.  These are usually labeled to indicate if OTA is supported, though sometimes the "default" scheme is also set up for OTA.

When OTA is enabled in a sketch, HomeSpan checks that it has been compiled with OTA partitions, and will output a warning to the Arduino Serial Monitor if it does not detect partition scheme that includes OTA partitions, and therefore cannot enable OTA.  This checking is done upon start-up of a sketch immediately after WiFi connectivity has been established.

### OTA Tips and Tricks

The name of the device HomeSpan uses for OTA is the same as the name you assigned in your call to `homeSpan.begin()`.  If you have more than one device you intend to maintain with OTA, use `homeSpan.begin()` to give them different names so you can tell them apart when selecting which one to connect to from the Arduino IDE.

Use the `homeSpan.setSketchVersion()` method to set a version number for you sketch.  HomeSpan includes this value as part of its HAP MDNS broadcast, which means that by simply inspecting this broadcast you can learn which version of a sketch is running on a remote HomeSpan device, even if you can't plug it into a serial port for use with the Arduino Serial Monitor.  In addition to the sketch version, HomeSpan also broadcasts two other useful fields: the version number of the HomeSpan library used when the sketch was compiled, and an indicator of whether OTA is enabled.

The easiest way to inspect HAP MDNS broadcasts is with an MDNS browser.  There are a number of free ones available in the App Store.  HAP broadcasts will be found under the MDNS service name "_hap._tcp."

Always test out a new sketch on a local device connected to your computer before uploading it to a remote device via OTA.  If the sketch doesn't operate as expected, you can always upload the previous version, unless the new sketch causes crashes that continously reboot HomeSpan.  If HomeSpan can't run the OTA code, you won't be able to upload any sketches via OTA and you'll need to connect the device to a computer for programming via the serial port instead.

The ESP32 supports automated rollbacks that are designed to restore a device with a previouslt-working sketch under exactly these circumstances.  However, the current Arduino-ESP32 library does not support this feature.




