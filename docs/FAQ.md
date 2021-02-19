# Frequently Asked Questions

*A list of answers to frequently-asked-questions, as well as discussions of various topics of interest.*

#### How do I set my WiFi SSID and Password?

* In the spirit of promoting good security practices, HomeSpan does not allow you to embed your WiFi SSID and Password (collectively known as WiFi Credentials) into your sketch.  This eliminates the possibility of accidentally disclosing this information should you share your sketch with others.

* Instead, HomeSpan stores your WiFi Credentials in a dedicated non-volatile storage (NVS) partition of the ESP32.  There are two ways of entering this information.  If you are developing a sketch within the Arduino IDE, simply use the HomeSpan Command Line Interface and type 'W' into the Serial Monitor.  HomeSpan will prompt you to input your WiFi SSID and Password (see [CLI](CLI.md) for details).  As an alternative, if your HomeSpan device is not connected to a computer, you can launch HomeSpan's Temporary WiFi Setup Network and input your WiFi Credentials directly into the web forms served by your device (see the [User Guide](https://github.com/HomeSpan/HomeSpan/blob/master/docs/UserGuide.md#setting-homespans-wifi-credentials-and-setup-code) for details).
