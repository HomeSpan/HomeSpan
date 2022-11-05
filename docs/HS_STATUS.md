# HomeSpan Status

The optional ***homeSpan*** method, `void setStatusCallback(void (*func)(HS_STATUS status))`, can be used to create a callback function, *func*, that HomeSpan calls whenever its status changes.  HomeSpan passes *func* a single argument, *status*, of type *HS_STATUS*, defined as follows:

```C++
enum HS_STATUS {
  HS_WIFI_NEEDED,
  HS_WIFI_CONNECTING,
  HS_PAIRING_NEEDED,
  HS_PAIRED,
  HS_ENTERING_CONFIG_MODE,
  HS_CONFIG_MODE_EXIT,  
  HS_CONFIG_MODE_REBOOT,
  HS_CONFIG_MODE_LAUNCH_AP,
  HS_CONFIG_MODE_UNPAIR,
  HS_CONFIG_MODE_ERASE_WIFI,
  HS_CONFIG_MODE_EXIT_SELECTED, 
  HS_CONFIG_MODE_REBOOT_SELECTED,
  HS_CONFIG_MODE_LAUNCH_AP_SELECTED,
  HS_CONFIG_MODE_UNPAIR_SELECTED,
  HS_CONFIG_MODE_ERASE_WIFI_SELECTED,
  HS_REBOOTING,
  HS_FACTORY_RESET,
  HS_AP_STARTED,
  HS_AP_CONNECTED,
  HS_AP_TERMINATED,  
  HS_OTA_STARTED    
};
```

The ***homeSpan*** method `char* statusString(HS_STATUS s)`, is a convenience function for converting any of the above enumerations to short, pre-defined character string messages as follows:

```C++
const char* Span::statusString(HS_STATUS s){
  switch(s){
    case HS_WIFI_NEEDED: return("WiFi Credentials Needed");
    case HS_WIFI_CONNECTING: return("WiFi Connecting");
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
    default: return("Unknown");
  }
}
```

You can of course create any alternative messsages, or take any actions desired, in *func* and do not need to use the pre-defined strings above.

---

[↩️](Reference.md) Back to the Reference API page
