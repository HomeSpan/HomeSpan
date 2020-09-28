
#include <WiFi.h>
#include <unordered_set>
#include "Settings.h"

using std::unordered_set;

///////////////////////////////

struct Network {

  const int MAX_HTTP=4095;                            // max number of bytes in HTTP message
  char *apPassword=DEFAULT_AP_PASSWORD;               // Access Point password (does not need to be secret - only used to ensure excrypted WiFi connection)
  unsigned long lifetime=DEFAULT_AP_TIMEOUT*1000;     // length of time (in milliseconds) to keep Access Point alive before shutting down and re-starting
  
  char **ssidList=NULL;
  int numSSID;

  WiFiClient client=NULL;                 // client used for HTTP calls
  unsigned long timer;                    // length of time of trying to connect to WiFi
  unsigned long alarmTimeOut;             // alarm time after which access point is shut down and HomeSpan is re-started
  boolean landingPage=false;              // check to see if captive access point landing page is accessed
  int apStatus;                           // tracks access point status (0=timed-out, -1=cancel, 1=save)

  struct {
    char ssid[MAX_SSID+1];
    char pwd[MAX_PWD+1];
  } wifiData;
  
  char setupCode[8+1];  

  void scan();                                                              // scan for WiFi networks and save only those with unique SSIDs
  boolean serialConfigure();                                                // configure homeSpan WiFi and Setup Code from Serial Monitor; return 1=save settings, 0=cancel settings
  boolean allowedCode(char *s);                                             // checks if Setup Code is allowed (HAP defines a list of disallowed codes)
  void apConfigure(char *hostName);                                         // configure homeSpan WiFi and Setup Code using temporary Captive Access Point 'hostName'; only returns if sucessful, else ESP restarts
  void processRequest(char *body, char *formData);                          // process the HTTP request
  int getFormValue(char *formData, char *tag, char *value, int maxSize);    // search for 'tag' in 'formData' and copy result into 'value' up to 'maxSize' characters; returns number of characters, else -1 if 'tag' not found
  int badRequestError();                                                    // return 400 error
  
};
