# Getting Started with HomeSpan

This page provides step-by-step instructions for acquiring and setting up all the software and hardware needed to develop HomeSpan devices.  Those with Arduino and/or ESP32 experience may find they can skip some of the steps.

#### Step 1: Install the Arduino IDE

Since HomeSpan is a library designed for Arduino's Integrated Development Environment, the first step is to download and install the latest version of this IDE from the [Arduino Software page](https://www.arduino.cc/en/software).  If you have experience with programming Arduino sketches, you'll find Homespan's use of the standard Arduino setup() and loop() functions, as well as the ever-present *\*.ino* file, very familiar.  If you've never programmed an Arduino, you may want to begin by first reviewing [Arduino's Guide to the IDE](https://www.arduino.cc/en/Guide/Environment) (and maybe try out a few Arduino examples) before proceeding to program with HomeSpan.

#### Step 2: Install the Arduino-ESP32 Core

The Arduino IDE comes with built-in compilers and support for a variety of Arduino boards, such as the Arduino Uno and Arduino Nano.  However, the IDE does not natively support programming for the ESP32.  Fortunately, the IDE allows you to download and install *cores* for other micro-controllers, including the ESP32, so  they can be programming from within the Arduino environment.  See the [Espressif Arduino-ESP32 GitHub page](https://github.com/espressif/arduino-esp32) for details on the Arduino-ESP32 Core, as well as for instructions on how to install easily this core (and keep it updated) from within the Arduino IDE using the [Arduino IDE Board Manager](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html#installing-using-arduino-ide).

#### Step 3: Install HomeSpan

HomeSpan is packaged as a standard Arduino library and can be installed either automatically from the Arduino IDE, or manually via GitHub.

* To install automatically, open the Arduino Library Manager from within the Arduino IDE by selecting *Sketch → Include Library → Manage Libraries...* from the Arduino menu bar.  Then, type *HomeSpan* into the search box, choose the latest version from the drop-down box, and click the `Install` button.  The Arduino Library Manager will automatically download and install the version you selected. HomeSpan and all of the tutorial example sketches are now ready for use.

* To install manually, first download the [latest version](https://github.com/HomeSpan/HomeSpan/releases/latest) of *Source Code (zip)* to your desktop. It does not matter if your operating system keeps the package in zip form, or automatically unzips it when saving to you desktop.  Next, from the top menu bar within the Arduino IDE, select *Sketch → Include Library → Add .ZIP Library...* and navigtate to the Desktop folder where you should see the HomeSpan package you just downloaded (either as a zip file, or a folder).  Select the package (don't open it) and click `Choose`.  This directs the Arduino IDE to copy the HomeSpan package into its library sub-folder.  HomeSpan is now ready for use as a standard Arduino library (you may delete the HomeSpan package from your desktop as it is no longer needed).

#### Step 4: Acquire an ESP32 Board

ESP32 development boards are widely available in different configurations from most electronics hobby sites, such as [Adafruit](https://www.adafruit.com) and [Sparkfun](https://www.sparkfun.com).  HomeSpan works with any ESP32 supported by the [Arduino-ESP32 Core](https://docs.espressif.com/projects/arduino-esp32/en/latest/getting_started.html#supported-soc-s), including the original ESP32, the ESP32-S2 and ESP32&#8209;S3, and the ESP32-C3.

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

## Adding a Control Button and Status LED (*optional*)

In addition to being able to configure a HomeSpan device using the [HomeSpan CLI](CLI.md) via the Arduino Serial Monitor, HomeSpan provides an alternative method for end-users to configure a standalone HomeSpan device that is not connected to a computer.  This method requires the installation of two external components:

1. a normally-open single-pole pushbutton to function as the HomeSpan Control Button, and
1. an LED (with a current-limiting resistor) to function as the HomeSpan Status LED.

The Control Button should be installed between ground and any pin on the ESP32 that can serve as an input.  To inform HomeSpan of which pin you chose, you must call the method `homeSpan.setControlPin(pin)` near the top of your sketch (see the [HomeSpan API Reference](Reference.md) for details), else HomeSpan will assume a Control Button has **not** be installed.

Similarly, the Status LED can be connected to any pin on the ESP32 that can serve as an output (and grounded through an appropriately-sized current-limiting resistor).  To inform HomeSpan of which pin you chose, you must call the method `homeSpan.setStatusPin(pin)` near the top of your sketch, else HomeSpan will assume a Status LED has **not** been installed.  Note some ESP32 boards have a built-in LED --- it is fine to use this for the Status LED if it is a simple on/off LED.  If your built-in LED is an RGB NeoPixel, or if you would like to use an external RGB NeoPixel for your Status LED, call the method `homeSpan.setStatusPixel(pin)` instead, in which case HomeSpan will use its NeoPixel driver to operate the status LED.

Using the Control Button and Status LED to configure a standalone HomeSpan device, including starting HomeSpan's temporary WiFi network to configure the device's WiFi Credentials and HomeKit Setup Code, is fully explained in the [HomeSpan User Guide](UserGuide.md).

## What Next?

If you've not yet read through the [HomeSpan API Overview](Overview.md) page, you should do so now so you'll have a good understanding of the framework for developing HomeSpan sketches.

Next, explore the tutorial sketches, upload a few, and see how they work. The examples start simple and grow in complexity, taking you through all the functions and features of HomeSpan.  Along the way you'll also learn a lot of HomeKit tips and tricks.  See [HomeSpan Tutorials](Tutorials.md) for a summary of all the included examples.  Find something in a sketch you don't understand?  Visit the [HomeSpan API Reference](Reference.md) for details on all HomeSpan objects, functions, and methods.  Have a more general question?  See if it's been answered on the [HomeSpan FAQ](FAQ.md) page or any of the [Disussion](https://github.com/HomeSpan/HomeSpan/discussions) or [Issues](https://github.com/HomeSpan/HomeSpan/issues) pages.  If not, feel free to join the Discusion by adding a new question.

Ready to start creating your own HomeSpan sketches?  Check out the [HomeSpan Services and Characteristics](ServiceList.md) page for a full list of all the HomeKit Services and Characteristics supported by HomeSpan, as well as the [HomeSpan Categories](Categories.md) page for a list of all supported HomeKit Categories.  And don't forget to use Apple's HomeKit Accessory Protocol Specification, Release R2 (HAP-R2) as your go-to reference for details on every Service and Characteristic.

While developing your sketch remember to utilize the Arduino Serial Monitor.  HomeSpan produces extensive diagnostics that will help you debug your sketches as well as monitor all aspects of the HomeSpan device.  You'll also be able to control various aspects of HomeSpan from the Serial Monitor using the [HomeSpan Command-Line Interface (CLI)](CLI.md), including configuring the device's WiFi Credentials and HomeKit Setup Code.

For access to even more advanced features, check out the [HomeSpan PWM](https://github.com/HomeSpan/HomeSpan/blob/master/docs/PWM.md), [HomeSpan RFControl](https://github.com/HomeSpan/HomeSpan/blob/master/docs/RMT.md), and [HomeSpan Pixels](https://github.com/HomeSpan/HomeSpan/blob/master/docs/Pixels.md) pages for tutorials and details on how HomeSpan can be used to control LEDs, lights, and Servo Motors with PWM signals; generate RF and IR Remote Control codes to operate appliances and TVs; and even control multicolor RGB LED strips.

Finally, disconnect your HomeSpan device from the computer and power it directly from a wall adapter.  After all, HomeSpan devices are designed to run on a standalone basis connected to real-world applicances like lights, fans, door locks, and window shades.  See the [HomeSpan User Guide](UserGuide.md) for end-user instructions on how to operate and configure a standlone HomeSpan device's WiFi Credentials and HomeKit Setup Code *without the need to connect the device to a computer*.

---

[↩️](../README.md) Back to the Welcome page
