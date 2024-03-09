# Common Problems and Potential Solutions

### *My HomeSpan device does not appear in the Home App when I try to pair it*

* There are a few reasons this may occur:

  * **You have not yet entered your WiFi credentials**.  HomeSpan can't connect to your WiFi network until you provide it with your WiFi&nbsp;SSID and password. If HomeSpan can't find these Credentials it outputs a warning message to the Serial Monitor during start-up.  You can enter your WiFi Credentials into HomeSpan either directly from the Serial Monitor by using the 'W' CLI command, or via the HomeSpan Access Point Setup Page, or by hardcoding it into your sketch with the `homeSpan.setWifiCredentials()` function, though this last method is not recommended for security reasons.

  * **You are out of range of your WiFi network, or entered the wrong WiFi Credentials**.  When HomeSpan first boots, if you've previously entered your WiFi Credentials (see above) it will use them to connect to your WiFi network.  Check the Serial Monitor for status on whether a connection has been successfully made.  If not, make sure your device is in the range of your WiFi network, and re-enter your WiFi Credentials in case you entered them incorrectly the first time.  To double-check that your HomeSpan device is indeed connected to your network after HomeSpan reports a successful connection, open up a terminal window on your computer and use the `ping` command to confirm you can reach the device. 
 
  * **Your iPhone and ESP32 device are not connected to the same WiFi network**.  Make sure your HomeSpan device is connected to the same SSID as your iPhone and any HomeKit Hubs (e.g. HomePods or Apple TV).  Some routers provide a separate SSID to use for IoT ("Internet of Things") devices. If you decide to use a separate SSID, make sure to configure your router so that message traffic flows unimpeded between your main SSID and your dedicated IoT SSID with absolutely no filtering of any messages. Also note that like most commercial HomeKit devices, ESP32 devices operate only on the 2.4 GHz WiFi band.  Most iPhones can operate on either the 2.4 GHz or the 5.0 GHz WiFi bands, so if your router provides multi-band access, you need to make sure it is configured to allow unimpeded cross-traffic between the bands.
    
  * **Your device thinks it is already paired (this is the most common reason)**.  Check the Serial Monitor when HomeSpan first boots - it will let you know if the device is currently *paired* or *unpaired*.  If its already *paired* you must unpair it before it can be paired again with the Home App.  Normally you would unpair the device from the Home App itself, but if for whatever reason you can't (perhaps the device is no longer showing up in the Home App) you can force HomeSpan to forget all its pairing data and reset its state to *unpaired* by typing either the 'U' or 'H' CLI command into the Serial Monitor.  The 'U' command instructs HomeSpan to simply erase all its *Controller* pairing data and reset its state to *unpaired*.  The 'H' command instructs HomeSpan to  erase all its *Controller* pairing data **and** its *HomeKit Device ID*, after which it reboots into the *unpaired* state and generates a new *HomeKit Device ID*.  Typing 'H' is recommended to get the cleanest refresh.  Note that your WiFi Credentials and Pairing Setup Code are not changed by either of these commands.

### *HomeSpan works correctly when my ESP32 is plugged into a computer or separately powered through the USB port, but it fails to work when powered directly through the ESP32 5V pin without any USB connection*

* On some ESP32 boards, the USB-UART chip only receives power if power is applied through the USB port, and thus remains unpowered if the ESP32 is powered solely through the ESP32 5V pin.  As a result, the Serial RX pin associated with UART0 on the ESP32, which is normally driven by the USB-UART chip, is free to float at any voltage.  If this pin floats low, the ESP32 thinks there is data to be read from the Serial Monitor, and HomeSpan falls into an infinite loop trying to read this non-existent data.

* **Resolution:**  Add a pull-up resistor (10 kΩ should be fine) connecting the UART0 RX pin on your board to +3.3V.  For most boards the default UART0 RX pin is as follows:

  * Pin 3 for the ESP32
  * Pin 44 for the ESP32-S2 and ESP32-S3
  * Pin 20 for the ESP32-C3

* Note that adding this pull-up resistor should **not** interefere with normal serial operation of the board, such as using the Serial Monitor or uploading sketches.

* *As an alternative*, instead of adding a pull-up resistor, you can simply exclude (or comment out) the `Serial.begin()` line in your sketch, or call `homeSpan.setSerialInputDisable(false)` to disable HomeSpan reading from the Serial port.  This should prevent the problem of HomeSpan hanging when you power it through the 5V pin, but it unfortunately means the Serial Monitor will not function when you connect the board to your computer, and you will need to add back `Serial.begin()`, or remove `homeSpan.setSerialInputDisable(false)` whenever you want to use the Serial Monitor.

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

