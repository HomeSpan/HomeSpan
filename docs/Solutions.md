# Common Problems and Potential Solutions

#### *HomeSpan works correctly when my ESP32 is plugged into a computer or separately powered through the USB port, but it fails to work when powered directly through the ESP32 5V pin without any USB connection*

* On some ESP32 boards, the USB-UART chip only receives power if power is applied through the USB port, and thus remains unpowered if the ESP32 is powered solely through the ESP32 5V pin.  As a result, the Serial RX pin associated with UART0 on the ESP32, which is normally driven by the USB-UART chip, is free to float at any voltage.  If this pin floats low, the ESP32 thinks there is data to be read from the Serial Monitor, and HomeSpan falls into an infinite loop trying to read this non-existent data.

* **Resolution:**  Add a pull-up resistor (10 kΩ should be fine) connecting the UART0 RX pin on your board to +3.3V.  For most boards the default UART0 RX pin is as follows:

  * Pin 3 for the ESP32
  * Pin 44 for the ESP32-S2 and ESP32-S3
  * Pin 20 for the ESP32-C3

* Note that adding this pull-up resistor should **not** interefere with normal serial operation of the board, such as using the Serial Monitor or uploading sketches.

* *As an alternative*, instead of adding a pull-up resistor, you can simply exclude (or comment out) the `Serial.begin()` line in your sketch.  This prevents the problem of HomeSpan hanging when you power it through the 5V pin, but it unfortunately means the Serial Monitor will not function when you connect the board to your computer, and you will need to add back `Serial.begin()` whenever you want to use the Serial Monitor.  

#### *Compiler Error - `core_version.h` file not found*

* HomeSpan requires the file `core_version.h`.  This file is part of each Arduino-ESP32 release package and is properly installed if you use the Arduino Board Manager.  If you install the Arduino-ESP32 package manually by downloading the official release zip file (listed under the "Assets" section for each release), you will also find the `core_version.h` is included.

* If you receive the above error when you try to compile a HomeSpan sketch, that means the Arduino-ESP32 package was not properly installed.  This can occur if instead of using one of the two methods above, you've tried to manually install the Arduino-ESP32 package by downloading a zip file created live from a specific branch of the Arduino-ESP32 code (typically via the `<> Code` button on GitHub when browsing a particular branch).  The `core_version.h` file is usually not included in any of the branches, and thus will not be part of any zip files you request directly from a specific branch.

* **Resolution:** Use the Arduino Board manager to install the Arduino-ESP32 package (recommended), or install manually using an *official release zip* file found under the "Assets" section of each release version shown on the Arduino-ESP32 GitHub site.

---

[↩️](../README.md) Back to the Welcome page

