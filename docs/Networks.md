# WiFi and Ethernet Connectivity Options

HomeSpan can connect to your home network via either WiFi or Ethernet. All of the ESP32 chips supported by HomeSpan come with built-in WiFi so no additional hardware is needed. Only a small number of ESP32 development boards come with an Ethernet interface (such as this [Silicognition wESP32](https://wesp32.com)) so additional hardware (such as this [Adafruit Ethernet FeatherWing](https://www.adafruit.com/product/3201)) would be needed to connect via Ethernet.

## HomeSpan WiFi connectivity

Unless HomeSpan detects that you installed and configured an Ethernet interface (see below), it will default to using WiFi to connect to your home network.  To make the connection HomeSpan requires the SSID and password for your network, which it saves in non-volatile storage (NVS) for retention even when power is lost.  At start-up HomeSpan checks its NVS for the SSID and password you saved.  If found, it is used to make the WiFi connection.  If not, HomeSpan outputs a message to the Serial Interface (if your device is connected to a computer) and flashes the HomeSpan Status LED (if you've added one) indicating that it requires you to provide your network SSID and password.  There are three ways of doing so:

1. If your device is connected to a computer via USB you can type 'W' into the Serial Monitor interface.  HomeSpan will scan your WiFi environment and display all network SSIDs found.  You can then choose which SSID to use, or type your own, and provide its password.  HomeSpan stores this information in NVS, reboots, and then uses the SSID and password just stored to connect to your network.

1. If your device is not connected to a computer but you've added both a HomeSpan Control Button and a HomeSpan Status LED you can activate command mode (see the [User Guide](UserGuide.md) for details) and start the HomeSpan Setup Access Point, which launches a captive WiFi Access Point hosting a web page from your device and into which you can enter the SSID and password for your home network.  HomeSpan then tries to connect to the network you specified, and if successful will save the info in NVS, reboot, and proceed as per above.   
    1. you can alternatively start the HomeSpan Setup Access Point by typing 'A' intp the Serial Monitor interface if your device is connected to a computer via USB, though this is primarily for testing purposes since if you are connected to a computer you might as well use the 'W' method above
    1. you can change the name, password, and time-out of the HomeSpan Setup Access Point using the following homeSpan methods: `setApSSID()`, `setApPassword()`, and `setApTimeout()` (see the [API Reference](Reference.md) page for details on all homeSpan methods)
    1. you can also program HomeSpan to automatically launch its Setup Access Point if it does not find an SSID and password stored in NVS upon start-up by adding the homeSpan `enableAutoStartAP()` method to your sketch
    1. if you really don't like the look and feel of the HomeSpan Setup Access Point you can create your own method of having HomeSpan acquire your home network SSID and password by implement the homeSpan `setApFunction()` method in your sketch
    1. note that Apple provides more robust methods for WiFi-based HomeKit devices to seemlessly acquire your WiFi SSID and password from your iPhone when configuring your device.  Unfortunately Apple restricts this feature to licensed commercial devices only and it is not possible to do this with non-commercial HomeKit devices, such as HomeSpan
    
1. If neither of the above methods is appealing, you have the option of hard-coding your SSID and password into your sketch using the homeSpan `setWifiCredentials()` method, though this is not considered a good practice for security reasons

When HomeSpan tries to connect to your home network using the SSSID and password you provided, it makes the requests and then waits for a response.  If after a certain time it does not connect, it makes another request and waits a bit longer for a response.  After each failed attempt, HomeSpan increases the wait time using a specified pattern, after which the pattern repeats.  The default wait-time pattern is to wait 5 seconds, then 8, 14, 22, 36, and 60 seconds, before re-starting this pattern.  If desired, you can create your own request/response wait-time pattern using the homeSpan `setConnectionTimes()` method.

Once connected, HomeSpan automatically manages all reconnects if connectivity is lost (using the same request/response wait pattern as above).  If you have enabled Web Logging, all disconnect/reconnects events are tallied and recorded.

If your home network is based on a mesh router with multiple access points sharing the same SSID, HomeSpan automatically connects the access point with the strongest RSSI signal.  As an option, you can add the homeSpan `enableWiFiRescan()` method to your sketch to have HomeSpan periodically recan your WiFi network to see if there is a stronger access point matching your SSID.  If one is found, HonmeSpan disconnects from the existing access point and reconnects to the stronger one.  In both the Serial Monitor and the Web Log (if enabled), HomeSpan indicates the BSSID (i.e. the 6-byte MAC address) of the specific access point to which it is connected.  As an option, you can map BSSIDs to custom display names using the homeSpan `addBssidName()` method.  When specified, HomeSpan will show these display names next to any BSSID in a log file to make it easier to track which access point is being used.

Internally, HomeSpan manages WiFi connectivity using the Arduino-ESP32 Library's global `WiFi` object and iniaties connections using the `WiFi.begin()` method.  This method assumes connectivity to standard WiFi network requiring an SSID and password.  If you are trying to connect to an enterprise WiFi network, or if you have other specialized configuration requirements that need to be made while connecting to your WiFi network (such as changing the power of your WiFi antenna), you can HomeSpan call your own "begin" function instead of simply calling `WiFi.begin()` by implementing the homeSpan `setWifiBegin()` method in your sketch.

Lastly, if you need to run a custom function either once, or every time, a WiFi connection is established (or re-established after a disconnect) you can implement the homeSpan `setConnectionCallback()` method in your sketch.

## HomeSpan Ethernet connectivity

HomeSpan utilizes the Arduino-ESP32 Library's global `ETH` object to manage Ethernet connectivity and thus supports any Ethernet board that is supported by the Arduino-ESP32 library.  To establish an Ethernet connection, simply add `ETH.begin()` in the `setup()` section of your sketch somewhere before calling `homeSpan.begin()`.  The `ETH.begin` function requires different parameters depending on your type of Ethernet board.  Please consult the instructions that came with your Ethernet board and work with the Ethernet examples Espressif provides in the Arduino-ESP32 Library to determine the correct parameters for your board.

There is nothing you need to do in HomeSpan to inform it that you will be using an Ethernet connection instead of WiFi.  Rather, at startup, if HomeSpan detects an Ethernet interface device has been properly configured (using `ETH.begin()`) and any external hardware needed is connected to the ESP device, HomeSpan will automatically switch into "Ethernet mode" and use Ethernet instead of WiFi for all communications to and from your home network.  Note that the Ethernet cable itself does not need to be plugged into your router for HomeSpan to switch into "Ethernet mode" during start-up.

Similar to WiFi connectivity, HomeSpan automatically handles all disconnects/reconnects (e.g. if you unplug the Ethernet cable and then plug it back into the router, or if the router itself reboots) and records such events in the Web Log (if enabled).  To run a custom function either once, or every time, an Ethernet connection is established (or re-established after a disconnect) you can implement the homeSpan `setConnectionCallback()` method in your sketch.

---

[↩️](../README.md) Back to the Welcome page


