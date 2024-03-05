# Common Problems and Potential Solutions

### *HomeSpan works correctly when my ESP32 is plugged into a computer or separately powered through the USB port, but it fails to work when powered directly through the ESP32 5V pin without any USB connection*

* On some ESP32 boards, the USB-UART chip only receives power if power is applied through the USB port, and thus remains unpowered if the ESP32 is powered solely through the ESP32 5V pin.  As a result, the Serial RX pin associated with UART0 on the ESP32, which is normally driven by the USB-UART chip, is free to float at any voltage.  If this pin floats low, the ESP32 thinks there is data to be read from the Serial Monitor, and HomeSpan falls into an infinite loop trying to read this non-existent data.

* **Resolution:**  Add a pull-up resistor (10 kΩ should be fine) connecting the UART0 RX pin on your board to +3.3V.  For most boards the default UART0 RX pin is as follows:

  * Pin 3 for the ESP32
  * Pin 44 for the ESP32-S2 and ESP32-S3
  * Pin 20 for the ESP32-C3

* Note that adding this pull-up resistor should **not** interefere with normal serial operation of the board, such as using the Serial Monitor or uploading sketches.

* *As an alternative*, instead of adding a pull-up resistor, you can simply exclude (or comment out) the `Serial.begin()` line in your sketch.  This prevents the problem of HomeSpan hanging when you power it through the 5V pin, but it unfortunately means the Serial Monitor will not function when you connect the board to your computer, and you will need to add back `Serial.begin()` whenever you want to use the Serial Monitor.

### *HomeSpan crashes when I enable PSRAM, but works fine if I disable PSRAM*

* If your ESP32 comes with Quad or Octal PSRAM, the chip will likely need to use one or more additional I/O pins so that it can access the PSRAM using an extended SPI bus, as required for these types of PSRAM.  If you happen to use one of those pins for something else (e.g an input button, an LED, etc.) the ESP32 will likely crash whenever PSRAM is enabled.
  
* **Resolution:**  Check the documentation for your board to see what pins are reserved for use when PSRAM is enabled, and *don't use those pins for anything else.*

### *The Serial Monitor is reporting* "gpio: gpio_set_level(226): GPIO output gpio_num error"

* This is an ESP32-generated error message and it occurs if you try to set the output of a pin that either does not exist on your chip, or does exist but it is input-only (i.e. it cannot be used as a output).  This typically occurs when you try to compile code for one chip (such as an ESP32-S2) on another chip (such as an ESP32-C3).  The code will compile fine, but may produce the above error during run-time.
  
* **Resolution:**  Check the documentation for your board and *use only pins that exist for your chip, and are not reserved for internal functions, and are not input-only.*

### *My sketch is too large to be uploaded (error when trying to upload)*

* Though all ESP32 chips have a minimum of 4MB of flash memory, the amount that can be used to store a program depends on the *partition scheme* selected.  By default, the ESP32 uses a parition scheme that reserves 1408 KB for a SPIFFS partition (SPI Flash File Storage) and splits the majority of the remaining flash into two equal OTA partitions of 1280 KB each for program storage.  Most HomeSpan sketches come close to filling up an entire OTA partition, and if you add a lot of other libraries you will likely exceed 1280&nbsp;KB.

* **Resolution:**  Select a different partition table that does not reserve so much flash memory for a SPIFFS partition since SPIFFS is not used at all by HomeSpan and *this partition is just wasting space.*

* From within the Arduino IDE, the easiest way to reduce the SPIFFS partition is to select the *Minimal SPIFFS* partition scheme from under the Tools menu, and then simply recompile and upload your sketch.  This scheme reserves only 128 KB for the SPIFFS partition, which leaves a full 1920 KB of program storage for each OTA partition.  This represents a 50% increase in program size, which should suffice for most applications.
  
* If for some reason you still need more space, and you only have 4MB of flash, you can try selecting the *Huge App* partition scheme, which reserves 896 KB for SPIFFS and leaves a *single* partition of 3072 KB for program storage. OTA will unfortunately *not* be available in this scheme. 

* Note: if you are not using the Arduino IDE to compile and upload HomeSpan sketches, please consult the documentation for your IDE to learn how to change the partition scheme. 

* Note: if none of the built-in partition schemes contain the right balance of partition sizes for your sketch, you can always create a custom partition scheme as demonstrated in HomeSpan's  [CustomNVSParititon Example](../examples/Other%20Examples/CustomNVSPartition).  This technique should work even if not using the Arduino IDE.

---

[↩️](../README.md) Back to the Welcome page

