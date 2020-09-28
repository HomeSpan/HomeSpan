
#ifndef ARDUINO_ARCH_ESP32
#error ERROR: HOMESPAN IS ONLY AVAILABLE FOR ESP32 MICROCONTROLLERS!
#endif

#include <Arduino.h>
#include <unordered_map>

#include "Settings.h"
#include "Utils.h"
#include "Network.h"

using std::vector;
using std::unordered_map;

enum {
  GET_AID=1,
  GET_META=2,
  GET_PERMS=4,
  GET_TYPE=8,
  GET_EV=16,
  GET_DESC=32,
  GET_ALL=255
};

// Forward-Declarations

struct Span;
struct SpanAccessory;
struct SpanService;
struct SpanCharacteristic;
struct SpanRange;
struct SpanBuf;
struct SpanButton;

///////////////////////////////

struct SpanConfig {                         
  int configNumber=0;                         // configuration number - broadcast as Bonjour "c#" (computed automatically)
  uint8_t hashCode[48]={0};                   // SHA-384 hash of Span Database stored as a form of unique "signature" to know when to update the config number upon changes
};

///////////////////////////////

struct Span{

  char *displayName;                            // display name for this device - broadcast as part of Bonjour MDNS
  char *hostNameBase;                           // base of host name of this device - full host name broadcast by Bonjour MDNS will have 6-byte accessoryID as well as '.local' automatically appended
  char *modelName;                              // model name of this device - broadcast as Bonjour field "md" 
  char category[3]="";                          // category ID of primary accessory - broadcast as Bonjour field "ci" (HAP Section 13)
  unsigned long snapTime;                       // current time (in millis) snapped before entering Service loops() or updates()
  
  char *defaultSetupCode=DEFAULT_SETUP_CODE;    // Setup Code used for pairing
  uint8_t statusPin=DEFAULT_STATUS_PIN;         // pin for status LED    
  uint8_t controlPin=DEFAULT_CONTROL_PIN;       // pin for Control Pushbutton

  Blinker statusLED;                                // indicates HomeSpan status
  PushButton controlButton;                         // controls HomeSpan configuration and resets
  Network network;                                  // configures WiFi and Setup Code via either serial monitor or temporary Access Point
    
  SpanConfig hapConfig;                             // track configuration changes to the HAP Accessory database; used to increment the configuration number (c#) when changes found
  vector<SpanAccessory *> Accessories;              // vector of pointers to all Accessories
  vector<SpanService *> Loops;                      // vector of pointer to all Services that have over-ridden loop() methods
  vector<SpanBuf> Notifications;                    // vector of SpanBuf objects that store info for Characteristics that are updated with setVal() and require a Notification Event
  vector<SpanButton *> PushButtons;                 // vector of pointer to all PushButtons
  unordered_map<uint64_t, uint32_t> TimedWrites;    // map of timed-write PIDs and Alarm Times (based on TTLs)
  
  void begin(Category catID,
             char *displayName=DEFAULT_DISPLAY_NAME,
             char *hostNameBase=DEFAULT_HOST_NAME,
             char *modelName=DEFAULT_MODEL_NAME);        
             
  void poll();                                  // poll HAP Clients and process any new HAP requests
  int getFreeSlot();                            // returns free HAPClient slot number. HAPClients slot keep track of each active HAPClient connection
  void initWifi();                              // initialize and connect to WiFi network
  void processSerialCommand(char *c);           // process command 'c' (typically from readSerial, though can be called with any 'c')

  int sprintfAttributes(char *cBuf);            // prints Attributes JSON database into buf, unless buf=NULL; return number of characters printed, excluding null terminator, even if buf=NULL
  void prettyPrint(char *buf, int nsp=2);       // print arbitrary JSON from buf to serial monitor, formatted with indentions of 'nsp' spaces
  SpanCharacteristic *find(int aid, int iid);   // return Characteristic with matching aid and iid (else NULL if not found)
  
  int countCharacteristics(char *buf);                                    // return number of characteristic objects referenced in PUT /characteristics JSON request
  int updateCharacteristics(char *buf, SpanBuf *pObj);                    // parses PUT /characteristics JSON request 'buf into 'pObj' and updates referenced characteristics; returns 1 on success, 0 on fail
  int sprintfAttributes(SpanBuf *pObj, int nObj, char *cBuf);             // prints SpanBuf object into buf, unless buf=NULL; return number of characters printed, excluding null terminator, even if buf=NULL
  int sprintfAttributes(char **ids, int numIDs, int flags, char *cBuf);   // prints accessory.characteristic ids into buf, unless buf=NULL; return number of characters printed, excluding null terminator, even if buf=NULL

  void clearNotify(int slotNum);                                          // set ev notification flags for connection 'slotNum' to false across all characteristics 
  int sprintfNotify(SpanBuf *pObj, int nObj, char *cBuf, int conNum);     // prints notification JSON into buf based on SpanBuf objects and specified connection number

  void setControlPin(uint8_t pin){controlPin=pin;}                        // sets Control Pin
  void setStatusPin(uint8_t pin){statusPin=pin;}                          // sets Status Pin
  void setApPassword(char *pwd){network.apPassword=pwd;}                  // sets Access Point Password
  void setApTimeout(uint16_t nSec){network.lifetime=nSec*1000;}           // sets Access Point Timeout (seconds)
};

///////////////////////////////

struct SpanAccessory{
    
  int aid=0;                                // Accessory Instance ID (HAP Table 6-1)
  int iidCount=0;                           // running count of iid to use for Services and Characteristics associated with this Accessory                                 
  vector<SpanService *> Services;           // vector of pointers to all Services in this Accessory  

  SpanAccessory();

  int sprintfAttributes(char *cBuf);        // prints Accessory JSON database into buf, unless buf=NULL; return number of characters printed, excluding null terminator, even if buf=NULL  
};

///////////////////////////////

struct SpanService{

  int iid=0;                                              // Instance ID (HAP Table 6-2)
  const char *type;                                       // Service Type
  boolean hidden=false;                                   // optional property indicating service is hidden
  boolean primary=false;                                  // optional property indicating service is primary
  vector<SpanCharacteristic *> Characteristics;           // vector of pointers to all Characteristics in this Service  
  
  SpanService(const char *type);

  SpanService *setPrimary();                              // sets the Service Type to be primary and returns pointer to self
  SpanService *setHidden();                               // sets the Service Type to be hidden and returns pointer to self

  int sprintfAttributes(char *cBuf);                      // prints Service JSON records into buf; return number of characters printed, excluding null terminator
  virtual StatusCode update() {return(StatusCode::OK);}   // update Service and return final statusCode based on updated Characteristics - should be overridden by DEVICE-SPECIFIC Services
  virtual void event(){}                                  // event generation for Services that create their own events and need to notify HomeKit of a new Characteristic value(s)
  virtual void loop(){}                                   // loops for each Service
  virtual void button(int pin, boolean isLong){}          // method called for a Service when a button attached to "pin" has a Short-Press or Long-Press, according to "isLong"
};

///////////////////////////////

struct SpanCharacteristic{

  enum {          // create bitflags based on HAP Table 6-4
    PR=1,
    PW=2,
    EV=4,
    AA=8,
    TW=16,
    HD=32,
    WR=64
  };

  enum FORMAT {                           // HAP Table 6-5
    BOOL,
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    INT,
    FLOAT,
    STRING
    };
    
  union UVal {                                  
    boolean BOOL;
    uint8_t UINT8;
    uint16_t UINT16;
    uint32_t UINT32;
    uint64_t UINT64;
    int32_t INT;
    double FLOAT;
    const char *STRING;      
  };
     
  int iid=0;                               // Instance ID (HAP Table 6-3)
  char *type;                              // Characteristic Type
  UVal value;                              // Characteristic Value
  uint8_t perms;                           // Characteristic Permissions
  FORMAT format;                           // Characteristic Format        
  char *desc=NULL;                         // Characteristic Description (optional)
  SpanRange *range=NULL;                   // Characteristic min/max/step; NULL = default values (optional)
  boolean ev[MAX_CONNECTIONS]={false};     // Characteristic Event Notify Enable (per-connection)
  
  int aid=0;                               // Accessory ID - passed through from Service containing this Characteristic
  boolean isUpdated=false;                 // set to true when new value has been requested by PUT /characteristic
  unsigned long updateTime;                // last time value was updated (in millis) either by PUT /characteristic OR by setVal()
  UVal newValue;                           // the updated value requested by PUT /characteristic
  SpanService *service=NULL;               // pointer to Service containing this Characteristic
      
  SpanCharacteristic(char *type, uint8_t perms);
  SpanCharacteristic(char *type, uint8_t perms, boolean value);
  SpanCharacteristic(char *type, uint8_t perms, uint8_t value);
  SpanCharacteristic(char *type, uint8_t perms, uint16_t value);
  SpanCharacteristic(char *type, uint8_t perms, uint32_t value);
  SpanCharacteristic(char *type, uint8_t perms, uint64_t value);
  SpanCharacteristic(char *type, uint8_t perms, int32_t value);
  SpanCharacteristic(char *type, uint8_t perms, double value);
  SpanCharacteristic(char *type, uint8_t perms, const char* value);

  int sprintfAttributes(char *cBuf, int flags);   // prints Characteristic JSON records into buf, according to flags mask; return number of characters printed, excluding null terminator  
  StatusCode loadUpdate(char *val, char *ev);     // load updated val/ev from PUT /characteristic JSON request.  Return intiial HAP status code (checks to see if characteristic is found, is writable, etc.)
  
  template <class T=int> T getVal(){return(getValue<T>(value));}                    // returns UVal value
  template <class T=int> T getNewVal(){return(getValue<T>(newValue));}              // returns UVal newValue
  template <class T> T getValue(UVal v);                                            // returns UVal v

  void setVal(int value);                                                           // sets value of UVal value for all integer-based Characterstic types
  void setVal(double value);                                                        // sets value of UVal value for FLOAT Characteristic type

  boolean updated(){return(isUpdated);}                                             // returns isUpdated
  int timeVal();                                                                    // returns time elapsed (in millis) since value was last updated
  
};

///////////////////////////////

template <class T> T SpanCharacteristic::getValue(UVal v){

  switch(format){
    case BOOL:
      return((T) v.BOOL);
    case INT:
      return((T) v.INT);
    case UINT8:
      return((T) v.UINT8);
    case UINT16:
      return((T) v.UINT16);
    case UINT32:
      return((T) v.UINT32);
    case UINT64:
      return((T) v.UINT64);
    case FLOAT:
      return((T) v.FLOAT);
    case STRING:
      Serial.print("*** ERROR:  Can't use getVal() or getNewVal() for string Characteristics.\n\n");
      return(0);
  }
    
};

///////////////////////////////

struct SpanRange{
  int min;
  int max;
  int step;

  SpanRange(int min, int max, int step);
};

///////////////////////////////

struct SpanBuf{                               // temporary storage buffer for use with putCharacteristicsURL() and checkTimedResets() 
  int aid=0;                                  // updated aid 
  int iid=0;                                  // updated iid
  char *val=NULL;                             // updated value (optional, though either at least 'val' or 'ev' must be specified)
  char *ev=NULL;                              // updated event notification flag (optional, though either at least 'val' or 'ev' must be specified)
  StatusCode status;                          // return status (HAP Table 6-11)
  SpanCharacteristic *characteristic=NULL;    // Characteristic to update (NULL if not found)
};
  
///////////////////////////////

struct SpanButton{

  int pin;                       // pin number  
  boolean isTriggered=false;     // flag triggered when button is pressed
  unsigned long shortTime;       // time (in millis) required to register a short press
  unsigned long longTime;        // time (in millis) required to register a long press
  unsigned long shortAlarm;      // alarm time to trigger a short press
  unsigned long longAlarm;       // alarm time to triger a long press
  SpanService *service;          // Service to which this PushButton is attached

  SpanButton(int pin, unsigned long longTime=2000, unsigned long shortTime=5);
  void check();
};

/////////////////////////////////////////////////
// Extern Variables

extern Span homeSpan;

/////////////////////////////////////////////////

#include "Services.h"
