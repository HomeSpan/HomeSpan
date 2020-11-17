# Getting Started with HomeSpan

This page provides step-by-step instructions for acquiring and setting up all the software and hardware needed to develop HomeSpan devices.  Those with Arduino and/or ESP32 experience may find they can skip some of the steps.

#### Step 1: Install the Arduino IDE

Since HomeSpan is a library designed for Arduino's Integrated Development Environment, the first step is to download and install the latest version of this IDE from the [Arduino Software page](https://www.arduino.cc/en/software).  If you have experience with programming Arduino sketches, you'll find Homespan's use of the standard Arduino setup() and loop() functions, as well as the ever-present *\*.ino* file, very familiar.  If you've never programmed an Arduino, you may want to begin by first reviewing [Arduino's Guide to the IDE](https://www.arduino.cc/en/Guide/Environment) (and maybe try out a few Arduino examples) before proceeding to program with HomeSpan.

#### Step 2: Install the Arduino-ESP32 Core

The Arduino IDE comes with built-in compilers and support for a variety of Arduino boards, such as the Arduino Uno and Arduino Nano.  However, the IDE does not natively support programming for the ESP32.  Fortunately, the IDE allows you to download and install *cores* for other micro-controllers, including the ESP32, so  they can be programming from within the Arduino environment.  See the [Espressif Arduino-ESP32 GitHub page](https://github.com/espressif/arduino-esp32#arduino-core-for-esp32-wifi-chip) for complete instructions on how to install the Arduino-ESP32 core into the Arduino IDE using the [Arduino IDE Board Manager](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md).

#### Step 3: Install HomeSpan

HomeSpan is packaged as a standard Arduino IDE.  To install, first download the [latest version](https://github.com/HomeSpan/HomeSpan/releases/latest) of *Source Code (zip)* to your desktop. It does not matter if your operating system keeps the package in zip form, or automatically unzips it when saving to you desktop.  Next, from the top menu bar within the Arduino IDE, select *Sketch → Include Library → Add .ZIP Library...* and navigtate to the Desktop folder where you should see the HomeSpan package you just downloaded (either as a zip file, or a folder).  Select the package (don't open it) and click `Choose`.  This directs the Arduino IDE to copy the HomeSpan package into its library sub-folder.  HomeSpan is now ready for use as a standard Arduino library (you may delete the HomeSpan package from your desktop as it is no longer needed).

#### Step 4: Acquire an ESP32 Board

ESP32 development boards are widely available in different configurations from most electronics hobby sites, such as [Adafruit](https://www.adafruit.com) and [Sparkfun](https://www.sparkfun.com).  HomeSpan was developed using a dual-core ESP32-WROOM-32 (the [Huzzah32 from Adafruit](https://www.adafruit.com/product/3619)) but should work equally well on dual-core ESP32-WROVER chips.  HomeSpan has *not* been tested on any single-core ESP32 chips, such as the ESP32-S2.

#### Step 5: Install any required USB Drivers

Programming an ESP32 board is generally done serially via USB.  Most ESP32 development boards contain a specialized chip that converts the serial UART signals used by the ESP32 to USB signals that can be transmitted to and from your computer, usually over a standard USB cable.  Depending on the specific USB-UART chip used on your ESP32 board, you may need to install a separate driver.  The manufacturer of your board should provides instructions on how to download and install whatever drivers are required for your operating system.

#### Step 6: Program your First HomeSpan Device

Before proceeding, you may want to first read through the [HomeSpan Overview](Overview.md) page.  Alternatively, you can jump right in and program your device using one of HomeSpan's built-in tutorial sketches:

* Plug your ESP32 board into a free USB port on your computer
* Launch the Arduino IDE
* Load one of the HomeSpan tutorial sketches (for example, *File → Examples → HomeSpan → 01-SimpleLightBulb*)
* Set the Board to match your ESP32 board (for example, *Tools → Board → ESP32 Arduino → Adafruit ESP32 Feather*)
* Open the Arduino Serial Monitor (*Tools → Serial Monitor*)
* Compile and upload the HomeSpan sketch (*Sketch → Upload*)

After compilation and uploading completes, HomeSpan will report a variety of initial diagnostics messages to the Arduino Serial Monitor and report that it is ready.  Congratulations!  You've just programmed your first HomeSpan device.

#### Step 7: Configure and Pair your HomeSpan Device

Though the device is now programmed and fully operational, it needs to be configured with WiFi Credentials and a HomeKit Setup Code before it can be paired to your HomeKit account.  See HomeSpan's [Command-Line Interface (CLI)](CLI.md) page for details on how to configure HomeSpan via the Arduino Serial Monitor.  Once the device is connected to your WiFi network, HomeSpan is now ready to be paired from the Home App on your iPhone!

## What Next?

If you've not yet read through the [HomeSpan Overview](Overview.md) page, you should do so now so you'll have a good understanding of the framework for operating and programming HomeSpan devices.  If you are new to HomeKit development, you may also want to read through HomeSpan's [HomeKit Primer](HomeKitPrimer.md), which should provide you with some important foundational knowledge about HomeKit itself.

Next, explore the tutorial sketches, upload a few, and see how they work. The examples start simple and grow in complexity, taking you through all the functions and features of HomeSpan.  Along the way you'll also learn a lot of HomeKit tips and tricks.  See [HomeSpan Tutorials](Tutorials.md) for a summary of all the included examples.

Find something in a sketch you don't understand?  Visit the [HomeSpan Library Reference](Reference.md) for details on all HomeSpan objects, functions, and methods.  Ready to start creating your own HomeSpan sketches?  Check out the [HomeSpan Services and Characteristics] page for a full list of all the HomeKit Services and Characteristics supported by HomeSpan.  And don't forget to use Apple's [HomeKit Accessory Protocol Specification, Release R2 (HAP-R2)](https://developer.apple.com/support/homekit-accessory-protocol/) as your go-to reference for details on every Service and Characteristic.

Finally, disconnect your HomeSpan device from the computer and power it directly from a wall adapter.  After all, HomeSpan devices are designed to run on a standalone basis connected to real-world applicances like lights, fans, door locks, and window shades.  See the [Home User Guide](UserGuide.md) for end-user instructions on how to operate and configure a standlone HomeSpan device's WiFi Credentials and HomeKit Setup Code *without the need to connect the device to a computer*.

---

[↩️](README.md) Back to the Welcome page
