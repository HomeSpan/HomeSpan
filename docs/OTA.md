# Over-the-Air (OTA) Updates

HomeSpan supports Over-the-Air (OTA) updates, which allows you to *wirelessly* upload sketches directly from the Arduino IDE - no serial connection needed.  To activate this feature for your sketch, simply call the method `homeSpan.enableOTA()` prior to calling `homeSpan.begin()`.

When a HomeSpan sketch is run with OTA enabled, the device shows up as a "network" port that can be selected under the *Tools → Port* menu in the Arduino IDE.  Once selected, the IDE will direct all uploads to the device via WiFi instead of looking for it on a serial port.  Note that you can upload via OTA even if your device is still connected to a serial port, but the Arduino IDE does not presently support multiple port connections at the same time.  If you select a "network" port, the IDE will automatically close the Serial Monitor if it is open.  To re-instate uploads via the "serial" port, simply choose that port from the *Tools → Port* menu in the Arduino IDE.  Uploading via the serial port is always possible regardless of whether you have enabled OTA for a sketch.

By default, HomeSpan requires the use of a password whenever you begin an OTA upload.  The default OTA password is "homespan-ota".  The Arduino will prompt you for this password upon your first attempt to upload a sketch to a newly-connected device.  However, once the password for a specific device is entered, the Arduino IDE retains it in memory as long as the IDE is running, thereby saving you from having to type it again every time you re-upload a sketch via OTA.

You can change the password for a HomeSpan device from the [HomeSpan CLI](CLI.md) with the 'O' command.  Similar to a device's Setup Code, HomeSpan saves a non-recoverable hashed version of the OTA password you specify in non-volatile storage (NVS).  If you forget the password you specified, you'll need to create a new one using the 'O' command, or you can restore the default OTA password by fully erasing the NVS with the 'E' command.

You can also change the password programmatically from within a sketch by calling `homeSpan.enableOTA(const char *pwd)`.  This is not as secure as setting the password using the method above since your sketch will contain a plaintext-version, instead of a hashed-version, or your password.  Note that setting your password this way causes HomeSpan to ignore, but does not alter, any password you have saved in NVS using the 'O' command.

> :exclamation: Though not recommended, you can override the requirement for a password when enabling OTA for your sketch by including *false* as a parameter to the enabling method as such: `homeSpan.enableOTA(false)`.  Use with caution!  Anyone who can access the device over your network will now be able to upload a new sketch.

Note that in in order for OTA to properly operate, your sketch must be compiled with a partition scheme that includes OTA partitions.  Partition schemes are found under the *Tools → Partition Scheme* menu of the Arduino IDE.  Select a scheme that indicates it supports OTA.  Note that schemes labeled "default" usually include OTA partitions.  If unsure, try it out.  HomeSpan will let you know if it does or does not.

This is because HomeSpan checks that a sketch has been compiled with OTA partitions if OTA has been enabled for that sketch.  If OTA has been enabled but HomeSpan does not find any OTA partitions, it will indicate it cannot start the OTA Server via a warning message sent to the Serial Monitor immediately after WiFi connectivity has been established.  Otherwise it will output a confirmation message indicating the OTA Server has sucessfully started.

### OTA Safe Load

HomeSpan includes two additional safety checks when using OTA to upload a sketch:

1. HomeSpan checks to make sure the new sketch being uploaded is also another HomeSpan sketch. If not, HomeSpan will reject the new sketch and report an OTA error back to the Arduino IDE after the new sketch is uploaded, but before the device reboots.  Instead, HomeSpan will close the OTA connection and resume normal operations based on the existing sketch without rebooting.  The purpose of this safety check is to prevent you from accidentally uploading a non-HomeSpan sketch onto a remote device, making it impossible for you to re-upload the correct sketch without retreiving the remote device and connecting to you computer via the serial port.

1. After a successful upload of a new HomeSpan sketch via OTA, HomeSpan will check that the new HomeSpan sketch just loaded *also* has OTA enabled.  This check occurs after HomeSpan is rebooted with the new sketch.  If HomeSpan does not find OTA enabled, it will mark the current partition as invalid and reboot the device, causing the device to "roll back" to the previous version of the sketch that had OTA enabled.  The purpose of this safety check is to ensure you do not use OTA to upload a new HomeSpan sketch to a remote device, but failed to enable OTA in the new HomeSpan sketch.  If you did this you would be locked out of making any further updated via OTA and would instead need to retreive the remote device and connect it to your computer via the serial port.

Note that these check are *only* applicable when uploading sketches via OTA.  They are ignored whenever sketches are uploaded via the serial port.  Also, though these safety checks are enabled by default, they can be disabled when you first enable OTA by setting the second (optional) argument to *false* as such: `homeSpan.enableOTA(..., false)`.  See the API for details.

### OTA Tips and Tricks

* The name of the device HomeSpan uses for OTA is the same as the name you assigned in your call to `homeSpan.begin()`.  If you have multiple devices you intend to maintain with OTA, use `homeSpan.begin()` to give them each different names so you can tell them apart when selecting which one to connect to from the Arduino IDE.

* Use the `homeSpan.setSketchVersion()` method to set a version for your sketch (see the [HomeSpan API](Reference.md) for details).  If specified, HomeSpan will include the sketch version as part of its HAP MDNS broadcast.  This allows you determine which version of a sketch is running on a remote HomeSpan device, even if you can't plug it into a serial port for use with the Arduino Serial Monitor.  In addition to the sketch version, HomeSpan also includes other fields in its MDNS broadcast that are useful in identifying the device: the version number of the HomeSpan *library* used to compile the sketch, a field indicating whether or not OTA is enabled for the sketch, the version number of the Arduino-ESP32 library used when compiling, and the type of board (e.g. *feather_esp32*).

* If a sketch you've uploaded with OTA does not operate as expected, you can continue making modifications to the code and re-upload again.  Or, you can upload a prior version that was working properly.  However, the Safe Load features described above cannot protect against a HomeSpan sketch that has major run-time problems, such as causing a kernel panic that leads to an endless cycle of device reboots.  If this happens, HomeSpan won't be able to run the OTA Server code, and further OTA updates will *not* be possible.  Instead, you'll have to connect the device through a serial port to upload a new, working sketch.  **For this reason you should always fully test out a new sketch on a local device connected to your computer *before* uploading it to a remote, hard-to-access device via OTA.**

* Note that though the ESP IDF supports "automated" rollbacks that are designed to solve the problem of endless reboots after a bad upload, this feature is *not* enabled in the latest version of the Arudino-ESP32 library (2.0.2 at the time of this posting).

---

[↩️](../README.md) Back to the Welcome page


