# Over-the-Air (OTA) Updates

HomeSpan supports Over-the-Air (OTA) updates, which allows you to *wirelessly* upload sketches directly from the Arduino IDE - no serial connection needed.  To activate this feature for your sketch, simply call the method `homeSpan.enableOTA()` prior to calling `homeSpan.begin()`.

When a HomeSpan sketch is run with OTA enabled, the device shows up as a "network" port that can be selected under the *Tools → Port* menu in the Arduino IDE.  Once selected, the IDE will direct all uploads to the device via WiFi instead of looking for it on a serial port.  Note that you can upload via OTA even if your device is still connected to a serial port, but the Arduino IDE does not presently support multiple port connections at the same time.  If you select a "network" port, the IDE will automatically close the Serial Monitor if it is open.  To re-instate uploads via the "serial" port, simply choose that port from the *Tools → Port* menu in the Arduino IDE.  Uploading via the serial port is always possible regardless of whether you have enabled OTA for a sketch.

By default, HomeSpan requires the use of a password whenever you begin an OTA upload.  The default OTA password is "homespan-ota".  The Arduino will prompt you for this password upon your first attempt to upload a sketch to a newly-connected device.  However, once the password for a specific device is entered, the Arduino IDE retains it in memory as long as the IDE is running, thereby saving you from having to type it again every time you re-upload a sketch via OTA.

You can change the password for a HomeSpan device from the [HomeSpan CLI](CLI.md) with the 'O' command.  Similar to a device's Setup Code, HomeSpan saves a non-recoverable hashed version of the OTA password you specify in non-volatile storage (NVS).  If you forget the password you specified, you'll need to create a new one using the 'O' command, or you can restore the default OTA password by fully erasing the NVS with the 'E' command.

> :exclamation: Though not recommended, you can override the requirement for a password when enabling OTA for your sketch by including *false* as a parameter to the enabling method as such: `homeSpan.enableOTA(false)`.  Use with caution!  Anyone who can access the device over your network will now be able to upload a new sketch.

Note that in in order for OTA to properly operate, your sketch must be compiled with a partition scheme that includes OTA partitions.  Partition schemes are found under the *Tools → Partition Scheme* menu of the Arduino IDE.  Select a scheme that indicates it supports OTA.  Note that schemes labeled "default" usually include OTA partitions.  If unsure, try it out.  HomeSpan will let you know if it does or does not.

This is because HomeSpan checks that a sketch has been compiled with OTA partitions if OTA has been enabled for that sketch.  If OTA has been enabled but HomeSpan does not find any OTA partitioms, it will indicate it cannot start the OTA Server via a warning message sent to the Serial Monitor immediately after WiFi connectivity has been established.  Otherwise it will output a confirmation message indicating the OTA Server has sucessfully started.

### OTA Tips and Tricks

* The name of the device HomeSpan uses for OTA is the same as the name you assigned in your call to `homeSpan.begin()`.  If you have multiple devices you intend to maintain with OTA, use `homeSpan.begin()` to give them each different names so you can tell them apart when selecting which one to connect to from the Arduino IDE.

* Use the `homeSpan.setSketchVersion()` method to set a version for your sketch (see the [HomeSpan API](Reference.md) for details).  If specified, HomeSpan will include the sketch version as part of its HAP MDNS broadcast.  This allows you determine which version of a sketch is running on a remote HomeSpan device, even if you can't plug it into a serial port for use with the Arduino Serial Monitor.  In addition to the sketch version, HomeSpan also includes two other fields in its MDNS broadcast: the version number of the HomeSpan *library* used to compile the sketch, and a field indicating whether or not OTA is enabled for the sketch.

* If a sketch you've uploaded with OTA does not operate as expected, you can continue making modifications to the code and re-upload again.  Or, you can upload a prior version that was working properly.  However, this assumes that the sketch you uploaded does not have major problems, such as causing a kernel panic that leads to an endless cycle of device reboots.  If this happens, HomeSpan won't be able to run the OTA Server code, and further OTA updates will *not* ne possible.  Instead, you'll have to connect the device through a serial port to upload a new, working sketch.  **For this reason you should always fully test out a new sketch on a local device connected to your computer *before* uploading it to a remote, hard-to-access device via OTA.**

* The ESP32 itself supports "automated" rollbacks that are designed to restore a device with a previously-working sketch if the latest sketch causes a reboot before being able to set a self-test flag verifiying the code is operating correctly.  However, the version of the ESP32-IDF library (3.2.3) used by the latest version of the Arduino-ESP32 Library (1.0.4, at the time of this posting) does not support this feature.

---

[↩️](README.md) Back to the Welcome page


