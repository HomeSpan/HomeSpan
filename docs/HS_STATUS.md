# HomeSpan Status

The optional ***homeSpan*** method, `void setStatusCallback(void (*func)(HS_STATUS status))`, can be used to create a callback function, *func*, that HomeSpan calls whenever its status changes.  HomeSpan passes *func* a single argument, *status*, of type *HS_STATUS*, defined as follows:

```C++
enum HS_STATUS {
  HS_WIFI_NEEDED,                         // WiFi Credentials have not yet been set/stored
  HS_WIFI_CONNECTING,                     // HomeSpan is trying to connect to the network specified in the stored WiFi Credentials
  HS_PAIRING_NEEDED,                      // HomeSpan is connected to central WiFi network, but device has not yet been paired to HomeKit
  HS_PAIRED,                              // HomeSpan is connected to central WiFi network and ther device has been paired to HomeKit
  HS_ENTERING_CONFIG_MODE,                // User has requested the device to enter into Command Mode
  HS_CONFIG_MODE_EXIT,                    // HomeSpan is in Command Mode with "Exit Command Mode" specified as choice
  HS_CONFIG_MODE_REBOOT,                  // HomeSpan is in Command Mode with "Reboot" specified as choice
  HS_CONFIG_MODE_LAUNCH_AP,               // HomeSpan is in Command Mode with "Launch Access Point" specified as choice
  HS_CONFIG_MODE_UNPAIR,                  // HomeSpan is in Command Mode with "Unpair Device" specified as choice
  HS_CONFIG_MODE_ERASE_WIFI,              // HomeSpan is in Command Mode with "Erase WiFi Credentials" specified as choice
  HS_CONFIG_MODE_EXIT_SELECTED,           // User has selected "Exit Command Mode" 
  HS_CONFIG_MODE_REBOOT_SELECTED,         // User has select "Reboot" from the Command Mode
  HS_CONFIG_MODE_LAUNCH_AP_SELECTED,      // User has selected "Launch AP Access" from the Command Mode
  HS_CONFIG_MODE_UNPAIR_SELECTED,         // User has seleected "Unpair Device" from the Command Mode
  HS_CONFIG_MODE_ERASE_WIFI_SELECTED,     // User has selected "Erase WiFi Credentials" from the Command Mode
  HS_REBOOTING,                           // HomeSpan is in the process of rebooting the device
  HS_FACTORY_RESET,                       // HomeSpan is in the process of performing a Factory Reset of device
  HS_AP_STARTED,                          // HomeSpan has started the Access Point but no one has yet connected
  HS_AP_CONNECTED,                        // The Access Point is started and a user device has been connected
  HS_AP_TERMINATED,                       // HomeSpan has terminated the Access Point 
  HS_OTA_STARTED,                         // HomeSpan is in the process of receiving an Over-the-Air software update  
  HS_WIFI_SCANNING,                       // HomeSpan is in the process of scanning for WiFi networks
  HS_ETH_CONNECTING                       // HomeSpan is trying to connect to an Ethernet network
};
```

The ***homeSpan*** method `char* statusString(HS_STATUS s)`, is a convenience function for converting any of the above enumerations to short, pre-defined character string messages as follows:

```C++
const char* Span::statusString(HS_STATUS s){
  switch(s){
    case HS_WIFI_NEEDED: return("WiFi Credentials Needed");
    case HS_WIFI_CONNECTING: return("WiFi Connecting");
    case HS_ETH_CONNECTING: return("Ethernet Connecting");
    case HS_PAIRING_NEEDED: return("Device not yet Paired");
    case HS_PAIRED: return("Device Paired");
    case HS_ENTERING_CONFIG_MODE: return("Entering Command Mode");
    case HS_CONFIG_MODE_EXIT: return("1. Exit Command Mode"); 
    case HS_CONFIG_MODE_REBOOT: return("2. Reboot Device");
    case HS_CONFIG_MODE_LAUNCH_AP: return("3. Launch Access Point");
    case HS_CONFIG_MODE_UNPAIR: return("4. Unpair Device");
    case HS_CONFIG_MODE_ERASE_WIFI: return("5. Erase WiFi Credentials");
    case HS_CONFIG_MODE_EXIT_SELECTED: return("Exiting Command Mode...");
    case HS_CONFIG_MODE_REBOOT_SELECTED: return("Rebooting Device...");
    case HS_CONFIG_MODE_LAUNCH_AP_SELECTED: return("Launching Access Point...");
    case HS_CONFIG_MODE_UNPAIR_SELECTED: return("Unpairing Device...");
    case HS_CONFIG_MODE_ERASE_WIFI_SELECTED: return("Erasing WiFi Credentials...");
    case HS_REBOOTING: return("REBOOTING!");
    case HS_FACTORY_RESET: return("Performing Factory Reset...");
    case HS_AP_STARTED: return("Access Point Started");
    case HS_AP_CONNECTED: return("Access Point Connected");
    case HS_AP_TERMINATED: return("Access Point Terminated");
    case HS_OTA_STARTED: return("OTA Update Started");
    case HS_WIFI_SCANNING: return("WiFi Scanning Started");
    default: return("Unknown");
  }
}
```

### Example:

```C++
#include "HomeSpan.h"

void setup(){
  homeSpan.setStatusCallback(statusUpdate);   // set callback function
  ...
  homeSpan.begin();
  ...
}

// create a callback function that simply prints the pre-defined short messages on the Serial Monitor whenever the HomeSpan status changes

void statusUpdate(HS_STATUS status){
  Serial.printf("\n*** HOMESPAN STATUS CHANGE: %s\n",homeSpan.statusString(status));
}
```

You can of course create any alternative messsages, or take any actions desired, in *func* and do not need to use the pre-defined strings above.

---

[↩️](Reference.md) Back to the Reference API page
