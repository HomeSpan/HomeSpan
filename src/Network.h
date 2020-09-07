
#include <WiFi.h>
#include "Settings.h"

///////////////////////////////

struct Network {

  const int MAX_HTTP=4095;                // max number of bytes in HTTP message
  const char *apPassword="homespan";      // Access Point password (does not need to be secret - only used to ensure excrypted WiFi connection)
  const unsigned long lifetime=120000;    // length of time (in milliseconds) to keep Access Point alive before shutting down and re-starting

  WiFiClient client;                      // client used for HTTP calls
  unsigned long timer;                    // length of time of trying to connect to WiFi
  unsigned long alarmTimeOut;             // alarm time after which access point is shut down and HomeSpan is re-started

  char ssid[MAX_SSID+1];
  char pwd[MAX_PWD+1];
  char setupCode[8+1];  

  int serialConfigure();                                                    // configure homeSpan WiFi and Setup Code from Serial Monitor; return 1=save connection, -1=cancel and restart
  int apConfigure(char *hostName);                                          // configure homeSpan WiFi and Setup Code using temporary Captive Access Point 'hostName'; return 1=save connection, -1=cancel and restart
  int processRequest(char *body, char *formData);                           // process the HTTP request; return 0=continue, 1=save connection, -1=cancel and re-start
  int getFormValue(char *formData, char *tag, char *value, int maxSize);    // search for 'tag' in 'formData' and copy result into 'value' up to 'maxSize' characters; returns number of characters, else -1 if 'tag' not found
  int badRequestError();                                                    // return 400 error
  
};
