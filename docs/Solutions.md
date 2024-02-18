# Common Problems and Potential Solutions

#### *HomeSpan works correctly when my ESP32 is plugged into a computer or separately powered through the USB port, but it fails to work when powered directly through the ESP32 5V pin without any USB connection*

* On some ESP32 boards, the USB-UART chip only receives power if power is applied through the USB port, and thus remains unpowered if the ESP32 is powered solely through the ESP32 5V pin.  As a result, the Serial RX pin associated with UART0 on the ESP32, which is normally driven by the USB-UART chip, is free to float at any voltage.  If this pin floats low, the ESP32 thinks there is data to be read from the Serial Monitor, and HomeSpan falls into an infinite loop trying to read this non-existent data.

* **Resolution:**  Add a pull-up resistor (10 kΩ should be fine) connecting the UART0 RX pin on your board to +3.3V.  For most boards the default UART0 RX pin is as follows:

  * Pin 3 for the ESP32
  * Pin 44 for the ESP32-S2 and ESP32-S3
  * Pin 20 for the ESP32-C3

* Note that adding this pull-up resistor should **not** interefere with normal serial operation of the board, such as using the Serial Monitor or uploading sketches.

* *As an alternative*, instead of adding a pull-up resistor, you can simply exclude (or comment out) the `Serial.begin()` line in your sketch.  This prevents the problem of HomeSpan hanging when you power it through the 5V pin, but it unfortunately means the Serial Monitor will not function when you connect the board to your computer, and you will need to add back `Serial.begin()` whenever you want to use the Serial Monitor.

#### *HomeSpan crashes when I enable PSRAM, but works fine if I disable PSRAM*

* If your ESP32 comes with Quad or Octal PSRAM, the chip will likely need to use one or more additional I/O pins so that it can access the PSRAM using an extended SPI bus, as required for these types of PSRAM.  If you happen to use one of those pins for something else (e.g an input button, an LED, etc.) the ESP32 will likely crash whenever PSRAM is enabled.  The solution is to check the documentation for your board to see what pins are reserved for use when PSRAM is enabled, and don't use those pins for anything else.

#### *The Serial Monitor is reporting* "gpio: gpio_set_level(226): GPIO output gpio_num error"

* This is an ESP32-generated error message and it occurs if you try to set the output of a pin that either does not exist on your chip, or does exist but it is input-only (i.e. it cannot be used as a output).  This typically occurs when you try to compile code for one chip (such as an ESP32-S2) on another chip (such as an ESP32-C3).  The code will compile fine, but may produce the above error during run-time.  The solution is to check the documentation for your board and use only pins that exist for your chip, are not reserved for internal functions, and are not input-only.

---

[↩️](../README.md) Back to the Welcome page

