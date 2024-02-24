/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2024 Gregg E. Berman
 *  
 *  https://github.com/HomeSpan/HomeSpan
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *  
 ********************************************************************************/
 
#pragma once

#ifndef ARDUINO_ARCH_ESP32
#error ERROR: HOMESPAN IS ONLY AVAILABLE FOR ESP32 MICROCONTROLLERS!
#endif

#pragma GCC diagnostic ignored "-Wpmf-conversions"                // eliminates warning messages from use of pointers to member functions to detect whether update() and loop() are overridden by user
#pragma GCC diagnostic ignored "-Wunused-result"                  // eliminates warning message regarded unused result from call to crypto_scalarmult_curve25519()

#include <Arduino.h>
#include <unordered_map>
#include <vector>
#include <list>
#include <nvs.h>
#include <ArduinoOTA.h>
#include <esp_now.h>
#include <mbedtls/base64.h>

#include "src/extras/Blinker.h"
#include "src/extras/Pixel.h"
#include "src/extras/RFControl.h"
#include "src/extras/PwmPin.h"
#include "src/extras/StepperControl.h"

#include "Settings.h"
#include "Utils.h"
#include "Network.h"
#include "HAPConstants.h"
#include "HapQR.h"
#include "Characteristics.h"

using std::vector;
using std::unordered_map;
using std::list;

enum {
  GET_AID=1,
  GET_META=2,
  GET_PERMS=4,
  GET_TYPE=8,
  GET_EV=16,
  GET_DESC=32,
  GET_NV=64,
  GET_VALUE=128,
  GET_STATUS=256
};

///////////////////////////////

#define STATUS_UPDATE(LED_UPDATE,MESSAGE_UPDATE)  {homeSpan.statusLED->LED_UPDATE;if(homeSpan.statusCallback)homeSpan.statusCallback(MESSAGE_UPDATE);}

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
  HS_OTA_STARTED                          // HomeSpan is in the process of recveived an Over-the-Air software update
};

///////////////////////////////

// Forward-Declarations

struct Span;
struct SpanAccessory;
struct SpanService;
struct SpanCharacteristic;
struct SpanRange;
struct SpanBuf;
struct SpanButton;
struct SpanUserCommand;

extern Span homeSpan;

////////////////////////////////////////////////////////
// INTERNAL HOMESPAN STRUCTURES - NOT FOR USER ACCESS //
////////////////////////////////////////////////////////

struct SpanPartition{
  char magicCookie[32];
  uint8_t reserved[224];
};

///////////////////////////////

struct SpanConfig{                         
  int configNumber=0;                         // configuration number - broadcast as Bonjour "c#" (computed automatically)
  uint8_t hashCode[48]={0};                   // SHA-384 hash of Span Database stored as a form of unique "signature" to know when to update the config number upon changes
};

///////////////////////////////

struct SpanBuf{                               // temporary storage buffer for use with putCharacteristicsURL() and checkTimedResets() 
  uint32_t aid=0;                             // updated aid 
  int iid=0;                                  // updated iid
  char *val=NULL;                             // updated value (optional, though either at least 'val' or 'ev' must be specified)
  char *ev=NULL;                              // updated event notification flag (optional, though either at least 'val' or 'ev' must be specified)
  StatusCode status;                          // return status (HAP Table 6-11)
  SpanCharacteristic *characteristic=NULL;    // Characteristic to update (NULL if not found)
};
  
///////////////////////////////

struct SpanWebLog{                            // optional web status/log data
  boolean isEnabled=false;                    // flag to inidicate WebLog has been enabled
  uint16_t maxEntries=0;                      // max number of log entries;
  int nEntries=0;                             // total cumulative number of log entries
  const char *timeServer=NULL;                // optional time server to use for acquiring clock time
  const char *timeZone;                       // optional time-zone specification
  boolean timeInit=false;                     // flag to indicate time has been initialized
  char bootTime[33]="Unknown";                // boot time
  String statusURL;                           // URL of status log
  uint32_t waitTime=120000;                   // number of milliseconds to wait for initial connection to time server
  String css="";                              // optional user-defined style sheet for web log
    
  struct log_t {                              // log entry type
    uint64_t upTime;                          // number of seconds since booting
    struct tm clockTime;                      // clock time
    char *message;                            // pointers to log entries of arbitrary size
    String clientIP;                          // IP address of client making request (or "0.0.0.0" if not applicable)
  } *log=NULL;                                // array of log entries 

  void init(uint16_t maxEntries, const char *serv, const char *tz, const char *url);
  static void initTime(void *args);  
  void vLog(boolean sysMsg, const char *fmr, va_list ap);
};

///////////////////////////////

struct SpanOTA{                               // manages OTA process
  
  char otaPwd[33]="";                         // MD5 Hash of OTA password, represented as a string of hexidecimal characters

  static boolean enabled;                     // enables OTA - default if not enabled
  static boolean auth;                        // indicates whether OTA password is required
  static int otaPercent;
  static boolean safeLoad;                    // indicates whether OTA update should reject any application update that is not another HomeSpan sketch
  
  int init(boolean auth, boolean safeLoad, const char *pwd);
  int setPassword(const char *pwd);
  static void start();
  static void end();
  static void progress(uint32_t progress, uint32_t total);
  static void error(ota_error_t err);
};

//////////////////////////////////////
//   USER API CLASSES BEGINS HERE   //
//////////////////////////////////////

class Span{

  friend class SpanAccessory;
  friend class SpanService;
  friend class SpanCharacteristic;
  friend class SpanUserCommand;
  friend class SpanButton;
  friend class SpanRange;
  friend class SpanWebLog;
  friend class SpanOTA;
  friend class Network;
  friend class HAPClient;
  
  const char *displayName;                      // display name for this device - broadcast as part of Bonjour MDNS
  const char *hostNameBase;                     // base of hostName of this device - full host name broadcast by Bonjour MDNS will have 6-byte accessoryID as well as '.local' automatically appended
  const char *hostNameSuffix=NULL;              // optional "suffix" of hostName of this device.  If specified, will be used as the hostName suffix instead of the 6-byte accessoryID
  char *hostName=NULL;                          // derived full hostname
  const char *modelName;                        // model name of this device - broadcast as Bonjour field "md" 
  char category[3]="";                          // category ID of primary accessory - broadcast as Bonjour field "ci" (HAP Section 13)
  unsigned long snapTime;                       // current time (in millis) snapped before entering Service loops() or updates()
  boolean isInitialized=false;                  // flag indicating HomeSpan has been initialized
  boolean isBridge=true;                        // flag indicating whether device is configured as a bridge (i.e. first Accessory contains nothing but AccessoryInformation and HAPProtocolInformation)
  HapQR qrCode;                                 // optional QR Code to use for pairing
  const char *sketchVersion="n/a";              // version of the sketch
  nvs_handle charNVS;                           // handle for non-volatile-storage of Characteristics data
  nvs_handle wifiNVS=0;                         // handle for non-volatile-storage of WiFi data
  nvs_handle otaNVS;                            // handle for non-volatile storage of OTA data
  char pairingCodeCommand[12]="";               // user-specified Pairing Code - only needed if Pairing Setup Code is specified in sketch using setPairingCode()
  String lastClientIP="0.0.0.0";                // IP address of last client accessing device through encrypted channel
  boolean newCode;                              // flag indicating new application code has been loaded (based on keeping track of app SHA256)
  boolean serialInputDisabled=false;            // flag indiating that serial input is disabled
  uint8_t rebootCount=0;                        // counts number of times device was rebooted (used in optional Reboot callback)
  uint32_t rebootCallbackTime;                  // length of time to wait (in milliseconds) before calling optional Reboot callback
  
  int connected=0;                              // WiFi connection status (increments upon each connect and disconnect)
  unsigned long waitTime=60000;                 // time to wait (in milliseconds) between WiFi connection attempts
  unsigned long alarmConnect=0;                 // time after which WiFi connection attempt should be tried again
  
  const char *defaultSetupCode=DEFAULT_SETUP_CODE;            // Setup Code used for pairing
  uint16_t autoOffLED=0;                                      // automatic turn-off duration (in seconds) for Status LED
  int logLevel=DEFAULT_LOG_LEVEL;                             // level for writing out log messages to serial monitor
  uint8_t maxConnections=CONFIG_LWIP_MAX_SOCKETS-2;           // maximum number of allowed simultaneous HAP connections
  uint8_t requestedMaxCon=CONFIG_LWIP_MAX_SOCKETS-2;          // requested maximum number of simultaneous HAP connections
  unsigned long comModeLife=DEFAULT_COMMAND_TIMEOUT*1000;     // length of time (in milliseconds) to keep Command Mode alive before resuming normal operations
  uint16_t tcpPortNum=DEFAULT_TCP_PORT;                       // port for TCP communications between HomeKit and HomeSpan
  char qrID[5]="";                                            // Setup ID used for pairing with QR Code
  void (*wifiCallback)()=NULL;                                // optional callback function to invoke once WiFi connectivity is initially established
  void (*wifiCallbackAll)(int)=NULL;                          // optional callback function to invoke every time WiFi connectivity is established or re-established
  void (*weblogCallback)(String &)=NULL;                      // optional callback function to invoke after header table in Web Log is produced
  void (*pairCallback)(boolean isPaired)=NULL;                // optional callback function to invoke when pairing is established (true) or lost (false)
  boolean autoStartAPEnabled=false;                           // enables auto start-up of Access Point when WiFi Credentials not found
  void (*apFunction)()=NULL;                                  // optional function to invoke when starting Access Point
  void (*statusCallback)(HS_STATUS status)=NULL;              // optional callback when HomeSpan status changes
  void (*rebootCallback)(uint8_t)=NULL;                       // optional callback when device reboots
  
  WiFiServer *hapServer;                            // pointer to the HAP Server connection
  Blinker *statusLED;                               // indicates HomeSpan status
  Blinkable *statusDevice = NULL;                   // the device used for the Blinker
  PushButton *controlButton = NULL;                 // controls HomeSpan configuration and resets
  Network network;                                  // configures WiFi and Setup Code via either serial monitor or temporary Access Point
  SpanWebLog webLog;                                // optional web status/log
  TaskHandle_t pollTaskHandle = NULL;               // optional task handle to use for poll() function
  TaskHandle_t loopTaskHandle;                      // Arduino Loop Task handle
  boolean verboseWifiReconnect = true;              // set to false to not print WiFi reconnect attempts messages
    
  SpanOTA spanOTA;                                  // manages OTA process
  SpanConfig hapConfig;                             // track configuration changes to the HAP Accessory database; used to increment the configuration number (c#) when changes found
  vector<SpanAccessory *, Mallocator<SpanAccessory *>> Accessories;              // vector of pointers to all Accessories
  vector<SpanService *, Mallocator<SpanService *>> Loops;                      // vector of pointer to all Services that have over-ridden loop() methods
  vector<SpanBuf, Mallocator<SpanBuf>> Notifications;                    // vector of SpanBuf objects that store info for Characteristics that are updated with setVal() and require a Notification Event
  vector<SpanButton *,  Mallocator<SpanButton *>> PushButtons;                 // vector of pointer to all PushButtons
  unordered_map<uint64_t, uint32_t> TimedWrites;    // map of timed-write PIDs and Alarm Times (based on TTLs)
  
  unordered_map<char, SpanUserCommand *> UserCommands;           // map of pointers to all UserCommands

  void pollTask();                              // poll HAP Clients and process any new HAP requests
  int getFreeSlot();                            // returns free HAPClient slot number. HAPClients slot keep track of each active HAPClient connection
  void checkConnect();                          // check WiFi connection; connect if needed
  void commandMode();                           // allows user to control and reset HomeSpan settings with the control button
  void resetStatus();                           // resets statusLED and calls statusCallback based on current HomeSpan status
  void reboot();                                // reboots device

  void printfAttributes(int flags=GET_VALUE|GET_META|GET_PERMS|GET_TYPE|GET_DESC);   // writes Attributes JSON database to hapOut stream
  
  SpanCharacteristic *find(uint32_t aid, int iid);                        // return Characteristic with matching aid and iid (else NULL if not found)
  int countCharacteristics(char *buf);                                    // return number of characteristic objects referenced in PUT /characteristics JSON request
  int updateCharacteristics(char *buf, SpanBuf *pObj);                    // parses PUT /characteristics JSON request 'buf into 'pObj' and updates referenced characteristics; returns 1 on success, 0 on fail
  void printfAttributes(SpanBuf *pObj, int nObj);                         // writes SpanBuf objects to hapOut stream
  boolean printfAttributes(char **ids, int numIDs, int flags);            // writes accessory requested characteristic ids to hapOut stream - returns true if all characteristics are found and readable, else returns false
  void clearNotify(int slotNum);                                          // set ev notification flags for connection 'slotNum' to false across all characteristics 
  void printfNotify(SpanBuf *pObj, int nObj, int conNum);                 // writes notification JSON to hapOut stream based on SpanBuf objects and specified connection number

  static boolean invalidUUID(const char *uuid, boolean isCustom){
    int x=0;
    sscanf(uuid,"%*8[0-9a-fA-F]-%*4[0-9a-fA-F]-%*4[0-9a-fA-F]-%*4[0-9a-fA-F]-%*12[0-9a-fA-F]%n",&x);
    return(isCustom && (strlen(uuid)!=36 || x!=36));    
  }
  
  public:

  void begin(Category catID=DEFAULT_CATEGORY,
             const char *displayName=DEFAULT_DISPLAY_NAME,
             const char *hostNameBase=DEFAULT_HOST_NAME,
             const char *modelName=DEFAULT_MODEL_NAME);        
             
  void poll();                                  // calls pollTask() with some error checking
  void processSerialCommand(const char *c);     // process command 'c' (typically from readSerial, though can be called with any 'c')
  
  boolean updateDatabase(boolean updateMDNS=true);   // updates HAP Configuration Number and Loop vector; if updateMDNS=true and config number has changed, re-broadcasts MDNS 'c#' record; returns true if config number changed
  boolean deleteAccessory(uint32_t aid);             // deletes Accessory with matching aid; returns true if found, else returns false 

  Span& setControlPin(uint8_t pin, PushButton::triggerType_t triggerType=PushButton::TRIGGER_ON_LOW){            // sets Control Pin, with optional trigger type   
    controlButton=new PushButton(pin, triggerType);
    return(*this);
    }
    
  int getControlPin(){return(controlButton?controlButton->getPin():-1);}                 // get Control Pin (returns -1 if undefined)

  Span& setStatusPin(uint8_t pin){statusDevice=new GenericLED(pin);return(*this);}       // sets Status Device to a simple LED on specified pin
  Span& setStatusPixel(uint8_t pin,float h=0,float s=100,float v=100){                   // sets Status Device to an RGB Pixel on specified pin
    statusDevice=((new Pixel(pin))->setOnColor(Pixel::HSV(h,s,v)));
    return(*this);
  }
  Span& setStatusDevice(Blinkable *sDev){statusDevice=sDev;return(*this);}               // sets Status Device to a generic Blinkable object
  
  Span& setStatusAutoOff(uint16_t duration){autoOffLED=duration;return(*this);}          // sets Status LED auto off (seconds)  
  int getStatusPin(){return(statusLED->getPin());}                                       // get Status Pin (returns -1 if undefined)
  void refreshStatusDevice(){if(statusLED)statusLED->refresh();}                         // refreshes state of Status LED

  Span& setApSSID(const char *ssid){network.apSSID=ssid;return(*this);}                  // sets Access Point SSID
  Span& setApPassword(const char *pwd){network.apPassword=pwd;return(*this);}            // sets Access Point Password
  Span& setApTimeout(uint16_t nSec){network.lifetime=nSec*1000;return(*this);}           // sets Access Point Timeout (seconds)
  Span& setCommandTimeout(uint16_t nSec){comModeLife=nSec*1000;return(*this);}           // sets Command Mode Timeout (seconds)
  Span& setLogLevel(int level){logLevel=level;return(*this);}                            // sets Log Level for log messages (0=baseline, 1=intermediate, 2=all, -1=disable all serial input/output)
  int getLogLevel(){return(logLevel);}                                                   // get Log Level
  Span& setSerialInputDisable(boolean val){serialInputDisabled=val;return(*this);}       // sets whether serial input is disabled (true) or enabled (false)
  boolean getSerialInputDisable(){return(serialInputDisabled);}                          // returns true if serial input is disabled, or false if serial input in enabled
  Span& reserveSocketConnections(uint8_t n){maxConnections-=n;return(*this);}            // reserves n socket connections *not* to be used for HAP
  Span& setHostNameSuffix(const char *suffix){hostNameSuffix=suffix;return(*this);}      // sets the hostName suffix to be used instead of the 6-byte AccessoryID
  Span& setPortNum(uint16_t port){tcpPortNum=port;return(*this);}                        // sets the TCP port number to use for communications between HomeKit and HomeSpan
  Span& setQRID(const char *id);                                                         // sets the Setup ID for optional pairing with a QR Code
  Span& setSketchVersion(const char *sVer){sketchVersion=sVer;return(*this);}            // set optional sketch version number
  const char *getSketchVersion(){return sketchVersion;}                                  // get sketch version number
  Span& setWifiCallback(void (*f)()){wifiCallback=f;return(*this);}                      // sets an optional user-defined function to call once WiFi connectivity is initially established
  Span& setWifiCallbackAll(void (*f)(int)){wifiCallbackAll=f;return(*this);}             // sets an optional user-defined function to call every time WiFi connectivity is established or re-established
  Span& setPairCallback(void (*f)(boolean isPaired)){pairCallback=f;return(*this);}      // sets an optional user-defined function to call when Pairing is established (true) or lost (false)
  Span& setApFunction(void (*f)()){apFunction=f;return(*this);}                          // sets an optional user-defined function to call when activating the WiFi Access Point  
  Span& enableAutoStartAP(){autoStartAPEnabled=true;return(*this);}                      // enables auto start-up of Access Point when WiFi Credentials not found
  Span& setWifiCredentials(const char *ssid, const char *pwd);                           // sets WiFi Credentials
  Span& setStatusCallback(void (*f)(HS_STATUS status)){statusCallback=f;return(*this);}  // sets an optional user-defined function to call when HomeSpan status changes
  const char* statusString(HS_STATUS s);                                                 // returns char string for HomeSpan status change messages
  Span& setPairingCode(const char *s);                                                   // sets the Pairing Code - use is NOT recommended.  Use 'S' from CLI instead
  void deleteStoredValues(){processSerialCommand("V");}                                  // deletes stored Characteristic values from NVS  

  int enableOTA(boolean auth=true, boolean safeLoad=true){return(spanOTA.init(auth, safeLoad, NULL));}   // enables Over-the-Air updates, with (auth=true) or without (auth=false) authorization password  
  int enableOTA(const char *pwd, boolean safeLoad=true){return(spanOTA.init(true, safeLoad, pwd));}      // enables Over-the-Air updates, with custom authorization password (overrides any password stored with the 'O' command)

  Span& enableWebLog(uint16_t maxEntries=0, const char *serv=NULL, const char *tz="UTC", const char *url=DEFAULT_WEBLOG_URL){     // enable Web Logging
    webLog.init(maxEntries, serv, tz, url);
    return(*this);
  }

  void addWebLog(boolean sysMsg, const char *fmt, ...){               // add Web Log entry
    va_list ap;
    va_start(ap,fmt);
    webLog.vLog(sysMsg,fmt,ap);
    va_end(ap);    
  }

  Span& setWebLogCSS(const char *css){webLog.css="\n" + String(css) + "\n";return(*this);}
  Span& setWebLogCallback(void (*f)(String &)){weblogCallback=f;return(*this);} 
  void getWebLog(void (*f)(const char *, void *), void *);

  Span& setVerboseWifiReconnect(bool verbose=true){verboseWifiReconnect=verbose;return(*this);}

  Span& setRebootCallback(void (*f)(uint8_t),uint32_t t=DEFAULT_REBOOT_CALLBACK_TIME){rebootCallback=f;rebootCallbackTime=t;return(*this);}

  void autoPoll(uint32_t stackSize=8192, uint32_t priority=1, uint32_t cpu=0){     // start pollTask()
    xTaskCreateUniversal([](void *parms){
      for(;;){
        homeSpan.pollTask();
        vTaskDelay(5);
        }
      },
      "pollTask", stackSize, NULL, priority, &pollTaskHandle, cpu);
    LOG0("\n*** AutoPolling Task started with priority=%d\n\n",uxTaskPriorityGet(pollTaskHandle)); 
  }

  TaskHandle_t getAutoPollTask(){return(pollTaskHandle);}

  Span& setTimeServerTimeout(uint32_t tSec){webLog.waitTime=tSec*1000;return(*this);}    // sets wait time (in seconds) for optional web log time server to connect
 
  [[deprecated("Please use reserveSocketConnections(n) method instead.")]]
  void setMaxConnections(uint8_t n){requestedMaxCon=n;}                   // sets maximum number of simultaneous HAP connections
};

///////////////////////////////

class SpanAccessory{

  friend class Span;
  friend class SpanService;
  friend class SpanCharacteristic;
  friend class SpanButton;
  friend class SpanRange;
    
  uint32_t aid=0;                                         // Accessory Instance ID (HAP Table 6-1)
  int iidCount=0;                                         // running count of iid to use for Services and Characteristics associated with this Accessory                                 
  vector<SpanService *, Mallocator<SpanService*>> Services;                         // vector of pointers to all Services in this Accessory  

  void printfAttributes(int flags);                       // writes Accessory JSON to hapOut stream

  protected:

  ~SpanAccessory();                                       // destructor

  public:

  void *operator new(size_t size){return(HS_MALLOC(size));}     // override new operator to use PSRAM when available
  SpanAccessory(uint32_t aid=0);                                // constructor
};

///////////////////////////////

class SpanService{

  friend class Span;
  friend class SpanAccessory;
  friend class SpanCharacteristic;
  friend class SpanRange;

  int iid=0;                                              // Instance ID (HAP Table 6-2)
  const char *type;                                       // Service Type
  const char *hapName;                                    // HAP Name
  boolean hidden=false;                                   // optional property indicating service is hidden
  boolean primary=false;                                  // optional property indicating service is primary
  vector<SpanCharacteristic *, Mallocator<SpanCharacteristic*>> Characteristics;           // vector of pointers to all Characteristics in this Service  
  vector<SpanService *, Mallocator<SpanService *>> linkedServices;                   // vector of pointers to any optional linked Services
  boolean isCustom;                                       // flag to indicate this is a Custom Service
  SpanAccessory *accessory=NULL;                          // pointer to Accessory containing this Service
  
  void printfAttributes(int flags);                       // writes Service JSON to hapOut stream

  protected:
  
  virtual ~SpanService();                                 // destructor
  vector<HapChar *, Mallocator<HapChar*>> req;                                  // vector of pointers to all required HAP Characteristic Types for this Service
  vector<HapChar *, Mallocator<HapChar*>> opt;                                  // vector of pointers to all optional HAP Characteristic Types for this Service

  public:
  
  void *operator new(size_t size){return(HS_MALLOC(size));}                       // override new operator to use PSRAM when available
  SpanService(const char *type, const char *hapName, boolean isCustom=false);     // constructor
  SpanService *setPrimary();                                                      // sets the Service Type to be primary and returns pointer to self
  SpanService *setHidden();                                                       // sets the Service Type to be hidden and returns pointer to self
  SpanService *addLink(SpanService *svc);                                         // adds svc as a Linked Service and returns pointer to self
  vector<SpanService *, Mallocator<SpanService *>> getLinks(){return(linkedServices);}                       // returns linkedServices vector for use as range in "for-each" loops

  virtual boolean update() {return(true);}                // placeholder for code that is called when a Service is updated via a Controller.  Must return true/false depending on success of update
  virtual void loop(){}                                   // loops for each Service - called every cycle if over-ridden with user-defined code
  virtual void button(int pin, int pressType){}           // method called for a Service when a button attached to "pin" has a Single, Double, or Long Press, according to pressType
};

///////////////////////////////

class SpanCharacteristic{

  friend class Span;
  friend class SpanService;

  union UVal {                                  
    BOOL_t BOOL;
    UINT8_t UINT8;
    UINT16_t UINT16;
    UINT32_t UINT32;
    UINT64_t UINT64;
    INT_t INT;
    FLOAT_t FLOAT;
    STRING_t STRING = NULL;
  };

  int iid=0;                               // Instance ID (HAP Table 6-3)
  HapChar *hapChar;                        // pointer to HAP Characteristic structure
  const char *type;                        // Characteristic Type
  const char *hapName;                     // HAP Name
  UVal value;                              // Characteristic Value
  uint8_t perms;                           // Characteristic Permissions
  FORMAT format;                           // Characteristic Format        
  char *desc=NULL;                         // Characteristic Description (optional)
  char *unit=NULL;                         // Characteristic Unit (optional)
  UVal minValue;                           // Characteristic minimum (not applicable for STRING)
  UVal maxValue;                           // Characteristic maximum (not applicable for STRING)
  UVal stepValue;                          // Characteristic step size (not applicable for STRING)
  boolean staticRange;                     // Flag that indicates whether Range is static and cannot be changed with setRange()
  boolean customRange=false;               // Flag for custom ranges
  char *validValues=NULL;                  // Optional JSON array of valid values.  Applicable only to uint8 Characteristics
  boolean *ev;                             // Characteristic Event Notify Enable (per-connection)
  char *nvsKey=NULL;                       // key for NVS storage of Characteristic value
  boolean isCustom;                        // flag to indicate this is a Custom Characteristic
  boolean setRangeError=false;             // flag to indicate attempt to set Range on Characteristic that does not support changes to Range
  boolean setValidValuesError=false;       // flag to indicate attempt to set Valid Values on Characteristic that does not support changes to Valid Values
  
  uint32_t aid=0;                          // Accessory ID - passed through from Service containing this Characteristic
  boolean isUpdated=false;                 // set to true when new value has been requested by PUT /characteristic
  unsigned long updateTime=0;              // last time value was updated (in millis) either by PUT /characteristic OR by setVal()
  UVal newValue;                           // the updated value requested by PUT /characteristic
  SpanService *service=NULL;               // pointer to Service containing this Characteristic

  void printfAttributes(int flags);               // writes Characteristic JSON to hapOut stream
  StatusCode loadUpdate(char *val, char *ev);     // load updated val/ev from PUT /characteristic JSON request.  Return intitial HAP status code (checks to see if characteristic is found, is writable, etc.)  
    
  String uvPrint(UVal &u){
    char c[67];               // space for 64 characters + surrounding quotes + terminating null
    switch(format){
      case FORMAT::BOOL:
        return(String(u.BOOL));      
      case FORMAT::INT:
        return(String(u.INT));
      case FORMAT::UINT8:
        return(String(u.UINT8));        
      case FORMAT::UINT16:
        return(String(u.UINT16));        
      case FORMAT::UINT32:
        return(String(u.UINT32));        
      case FORMAT::UINT64:
        sprintf(c,"%llu",u.UINT64);
        return(String(c));        
      case FORMAT::FLOAT:
        sprintf(c,"%g",u.FLOAT);
        return(String(c));        
      case FORMAT::STRING:
      case FORMAT::DATA:
        sprintf(c,"\"%.64s\"",u.STRING);  // Truncating string to 64 chars
        return(String(c));        
    } // switch
    return(String());       // included to prevent compiler warnings
  }

  void uvSet(UVal &dest, UVal &src){
    if(format==FORMAT::STRING || format==FORMAT::DATA)
      uvSet(dest,(const char *)src.STRING);
    else
      dest=src;
  }

  void uvSet(UVal &u, const char *val){
    u.STRING = (char *)HS_REALLOC(u.STRING, strlen(val) + 1);
    strcpy(u.STRING, val);
  }

  template <typename T> void uvSet(UVal &u, T val){  
    switch(format){
      case FORMAT::BOOL:
        u.BOOL=(boolean)val;
      break;
      case FORMAT::INT:
        u.INT=(int)val;
      break;
      case FORMAT::UINT8:
        u.UINT8=(uint8_t)val;
      break;
      case FORMAT::UINT16:
        u.UINT16=(uint16_t)val;
      break;
      case FORMAT::UINT32:
        u.UINT32=(uint32_t)val;
      break;
      case FORMAT::UINT64:
        u.UINT64=(uint64_t)val;
      break;
      case FORMAT::FLOAT:
        u.FLOAT=(double)val;
      break;
      case FORMAT::STRING:
      case FORMAT::DATA:
      break;
    } // switch
  }
 
  template <class T> T uvGet(UVal &u){
  
    switch(format){   
      case FORMAT::BOOL:
        return((T) u.BOOL);        
      case FORMAT::INT:
        return((T) u.INT);        
      case FORMAT::UINT8:
        return((T) u.UINT8);        
      case FORMAT::UINT16:
        return((T) u.UINT16);        
      case FORMAT::UINT32:
        return((T) u.UINT32);        
      case FORMAT::UINT64:
        return((T) u.UINT64);        
      case FORMAT::FLOAT:
        return((T) u.FLOAT);        
      case FORMAT::STRING:
      case FORMAT::DATA:
      break;
    }
    return((T)0);       // included to prevent compiler warnings  
  }
    
  protected:

  ~SpanCharacteristic();                                                  // destructor  
    
  template <typename T, typename A=boolean, typename B=boolean> void init(T val, boolean nvsStore, A min=0, B max=1){

    uvSet(value,val);

    if(nvsStore){
      nvsKey=(char *)HS_MALLOC(16);
      uint16_t t;
      sscanf(type,"%hx",&t);
      sprintf(nvsKey,"%04X%08X%03X",t,aid,iid&0xFFF);
      size_t len;    

      if(format!=FORMAT::STRING && format!=FORMAT::DATA){
        if(nvs_get_u64(homeSpan.charNVS,nvsKey,&(value.UINT64))!=ESP_OK) {
          nvs_set_u64(homeSpan.charNVS,nvsKey,value.UINT64);                // store data as uint64_t regardless of actual type (it will be read correctly when access through uvGet())         
          nvs_commit(homeSpan.charNVS);                                     // commit to NVS  
        }     
      } else {
        if(!nvs_get_str(homeSpan.charNVS,nvsKey,NULL,&len)){
          value.STRING = (char *)HS_REALLOC(value.STRING,len);
          nvs_get_str(homeSpan.charNVS,nvsKey,value.STRING,&len);
        }
        else {
          nvs_set_str(homeSpan.charNVS,nvsKey,value.STRING);               // store string data
          nvs_commit(homeSpan.charNVS);                                    // commit to NVS  
        }
      }
    }
  
    uvSet(newValue,value);

    if(format!=FORMAT::STRING && format!=FORMAT::DATA) {
        uvSet(minValue,min);
        uvSet(maxValue,max);
        uvSet(stepValue,0);
    }
          
  } // init()

  public:

  void *operator new(size_t size){return(HS_MALLOC(size));}               // override new operator to use PSRAM when available
  SpanCharacteristic(HapChar *hapChar, boolean isCustom=false);           // constructor

  template <class T=int> T getVal(){
    return(uvGet<T>(value));
  }

  template <class T=int> T getNewVal(){
    return(uvGet<T>(newValue));
  }
    
  char *getString(){
    if(format == FORMAT::STRING)
        return value.STRING;

    return NULL;
  }

  char *getNewString(){
    if(format == FORMAT::STRING)
        return newValue.STRING;

    return NULL;
  }

  void setString(const char *val){

    if((perms & EV) == 0){
      LOG0("\n*** WARNING:  Attempt to update Characteristic::%s with setString() ignored.  No NOTIFICATION permission on this characteristic\n\n",hapName);
      return;
    }

    uvSet(value,val);
    uvSet(newValue,value);
      
    updateTime=homeSpan.snapTime;
    
    SpanBuf sb;                             // create SpanBuf object
    sb.characteristic=this;                 // set characteristic          
    sb.status=StatusCode::OK;               // set status
    char dummy[]="";
    sb.val=dummy;                           // set dummy "val" so that printfNotify knows to consider this "update"
    homeSpan.Notifications.push_back(sb);   // store SpanBuf in Notifications vector  

    if(nvsKey){
      nvs_set_str(homeSpan.charNVS,nvsKey,value.STRING);    // store data
      nvs_commit(homeSpan.charNVS);
    }
    
  } // setString()

  size_t getData(uint8_t *data, size_t len){    
    if(format!=FORMAT::DATA)
      return(0);

    size_t olen;
    int ret=mbedtls_base64_decode(data,len,&olen,(uint8_t *)value.STRING,strlen(value.STRING));
    
    if(data==NULL)
      return(olen);
      
    if(ret==MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL)
      LOG0("\n*** WARNING:  Can't decode Characteristic::%s with getData().  Destination buffer is too small (%d out of %d bytes needed)\n\n",hapName,len,olen);
    else if(ret==MBEDTLS_ERR_BASE64_INVALID_CHARACTER)
      LOG0("\n*** WARNING:  Can't decode Characteristic::%s with getData().  Data is not in base-64 format\n\n",hapName);
      
    return(olen);
  }

  size_t getNewData(uint8_t *data, size_t len){    
    if(format!=FORMAT::DATA)
      return(0);

    size_t olen;
    int ret=mbedtls_base64_decode(data,len,&olen,(uint8_t *)newValue.STRING,strlen(newValue.STRING));
    
    if(data==NULL)
      return(olen);
      
    if(ret==MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL)
      LOG0("\n*** WARNING:  Can't decode Characteristic::%s with getData().  Destination buffer is too small (%d out of %d bytes needed)\n\n",hapName,len,olen);
    else if(ret==MBEDTLS_ERR_BASE64_INVALID_CHARACTER)
      LOG0("\n*** WARNING:  Can't decode Characteristic::%s with getData().  Data is not in base-64 format\n\n",hapName);
      
    return(olen);
  }  

  void setData(uint8_t *data, size_t len){

    if((perms & EV) == 0){
      LOG0("\n*** WARNING:  Attempt to update Characteristic::%s with setData() ignored.  No NOTIFICATION permission on this characteristic\n\n",hapName);
      return;
    }

    if(len<1){
      LOG0("\n*** WARNING:  Attempt to update Characteristic::%s with setData() ignored.  Size of data buffer must be greater than zero\n\n",hapName);
      return;      
    }

    size_t olen;
    mbedtls_base64_encode(NULL,0,&olen,data,len);                    // get length of string buffer needed (mbedtls includes the trailing null in this size)
    TempBuffer<char> tBuf(olen);                                     // create temporary string buffer, with room for trailing null
    mbedtls_base64_encode((uint8_t*)tBuf.get(),olen,&olen,data,len );  // encode data into string buf
    setString(tBuf);                                             // call setString to continue processing as if characteristic was a string
  }  

  template <typename T> void setVal(T val, boolean notify=true){

    if((perms & EV) == 0){
      LOG0("\n*** WARNING:  Attempt to update Characteristic::%s with setVal() ignored.  No NOTIFICATION permission on this characteristic\n\n",hapName);
      return;
    }

    if(!((val >= uvGet<T>(minValue)) && (val <= uvGet<T>(maxValue)))){
      LOG0("\n*** WARNING:  Attempt to update Characteristic::%s with setVal(%g) is out of range [%g,%g].  This may cause device to become non-responsive!\n\n",
      hapName,(double)val,uvGet<double>(minValue),uvGet<double>(maxValue));
    }
   
    uvSet(value,val);
    uvSet(newValue,value);
      
    updateTime=homeSpan.snapTime;

    if(notify){
      SpanBuf sb;                             // create SpanBuf object
      sb.characteristic=this;                 // set characteristic          
      sb.status=StatusCode::OK;               // set status
      char dummy[]="";
      sb.val=dummy;                           // set dummy "val" so that printfNotify knows to consider this "update"
      homeSpan.Notifications.push_back(sb);   // store SpanBuf in Notifications vector  
  
      if(nvsKey){
        nvs_set_u64(homeSpan.charNVS,nvsKey,value.UINT64);            // store data as uint64_t regardless of actual type (it will be read correctly when access through uvGet())         
        nvs_commit(homeSpan.charNVS);
      }
    }
    
  } // setVal()

  boolean updated(){return(isUpdated);}             // returns isUpdated
  unsigned long timeVal();                          // returns time elapsed (in millis) since value was last updated
  
  SpanCharacteristic *setValidValues(int n, ...);   // sets a list of 'n' valid values allowed for a Characteristic and returns pointer to self.  Only applicable if format=INT, UINT8, UINT16, or UINT32

  template <typename A, typename B, typename S=int> SpanCharacteristic *setRange(A min, B max, S step=0){

    if(!staticRange){
      uvSet(minValue,min);
      uvSet(maxValue,max);
      uvSet(stepValue,step);  
      customRange=true; 
    } else
      setRangeError=true;
      
    return(this);
    
  } // setRange()

  SpanCharacteristic *setPerms(uint8_t perms){
    perms&=0x7F;
    if(perms>0)
      this->perms=perms;
    return(this);
  }

  SpanCharacteristic *addPerms(uint8_t dPerms){
    return(setPerms(perms|dPerms));
  }
  
  SpanCharacteristic *removePerms(uint8_t dPerms){
    return(setPerms(perms&(~dPerms)));
  }

  SpanCharacteristic *setDescription(const char *c){
    desc = (char *)HS_REALLOC(desc, strlen(c) + 1);
    strcpy(desc, c);
    return(this);
  }  

  SpanCharacteristic *setUnit(const char *c){
    unit = (char *)HS_REALLOC(unit, strlen(c) + 1);
    strcpy(unit, c);
    return(this);
  }  

};

///////////////////////////////

struct [[deprecated("Please use Characteristic::setRange() method instead.")]] SpanRange{
  SpanRange(int min, int max, int step);
};

///////////////////////////////

class SpanButton : public PushButton {

  friend class Span;
  friend class SpanService;
   
  uint16_t singleTime;           // minimum time (in millis) required to register a single press
  uint16_t longTime;             // minimum time (in millis) required to register a long press
  uint16_t doubleTime;           // maximum time (in millis) between single presses to register a double press instead
  SpanService *service;          // Service to which this PushButton is attached  
  
  void check();                  // check PushButton and call button() if "pressed"

  protected:
  
  enum buttonType_t {
    HS_BUTTON,
    HS_TOGGLE
  };

  buttonType_t buttonType=HS_BUTTON;      // type of SpanButton  
  
  public:
  
  SpanButton(int pin, uint16_t longTime=2000, uint16_t singleTime=5, uint16_t doubleTime=200, triggerType_t triggerType=TRIGGER_ON_LOW);
  SpanButton(int pin, triggerType_t triggerType, uint16_t longTime=2000, uint16_t singleTime=5, uint16_t doubleTime=200) : SpanButton(pin,longTime,singleTime,doubleTime,triggerType){};

};

///////////////////////////////

class SpanToggle : public SpanButton {

  public:

  SpanToggle(int pin, triggerType_t triggerType=TRIGGER_ON_LOW, uint16_t toggleTime=5) : SpanButton(pin,triggerType,toggleTime){buttonType=HS_TOGGLE;};
  int position(){return(pressType);}
};

///////////////////////////////

class SpanUserCommand {

  friend class Span;
  
  const char *s;                                            // description of command
  void (*userFunction1)(const char *v)=NULL;                // user-defined function to call
  void (*userFunction2)(const char *v, void *arg)=NULL;     // user-defined function to call with user-defined arg
  void *userArg;

  public:

  SpanUserCommand(char c, const char *s, void (*f)(const char *));  
  SpanUserCommand(char c, const char *s, void (*f)(const char *, void *), void *arg);  
};

///////////////////////////////

class SpanPoint {

  friend class Span;

  int receiveSize;                            // size (in bytes) of messages to receive
  int sendSize;                               // size (in bytes) of messages to send
  esp_now_peer_info_t peerInfo;               // structure for all ESP-NOW peer data
  QueueHandle_t receiveQueue;                 // queue to store data after it is received
  uint32_t receiveTime=0;                     // time (in millis) of most recent data received
  
  static uint8_t lmk[16];
  static boolean initialized;
  static boolean isHub;
  static boolean useEncryption;
  static vector<SpanPoint *, Mallocator<SpanPoint *>> SpanPoints;
  static uint16_t channelMask;                // channel mask (only used for remote devices)
  static QueueHandle_t statusQueue;           // queue for communication between SpanPoint::dataSend and SpanPoint::send
  static nvs_handle pointNVS;                 // NVS storage for channel number (only used for remote devices)
  
  static void dataReceived(const uint8_t *mac, const uint8_t *incomingData, int len);
  static void init(const char *password="HomeSpan");
  static void setAsHub(){isHub=true;}
  static uint8_t nextChannel();
  
  static void dataSent(const uint8_t *mac, esp_now_send_status_t status) {
    xQueueOverwrite( statusQueue, &status );
  }
  
  public:

  SpanPoint(const char *macAddress, int sendSize, int receiveSize, int queueDepth=1, boolean useAPaddress=false);
  static void setPassword(const char *pwd){init(pwd);}
  static void setChannelMask(uint16_t mask);
  static void setEncryption(boolean encrypt){useEncryption=encrypt;}
  boolean get(void *dataBuf);
  boolean send(const void *data);
  uint32_t time(){return(millis()-receiveTime);}
};

/////////////////////////////////////////////////

#include "Span.h"
