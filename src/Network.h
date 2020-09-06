
#include <WiFi.h>
#include "Settings.h"

///////////////////////////////

struct Network {

  struct {                           
    char ssid[MAX_SSID+1];            
    char pwd[MAX_PWD+1];
  } wifiData;

  const int MAX_HTTP=4095;      // max number of bytes in HTTP message

  char setupCode[9];            // 8-digit Setup Code with room for null terminator

  WiFiClient client;            // client used for HTTP calls
  unsigned long timer;          // time of trying to connect to WiFi
  unsigned long alarmTimeOut;   // alarm time after which access point is shut down and HomeSpan is paused until reset 

  void configure(char *hostName);                                           // configure homeSpan WiFi and Setup Code using temporary Captive Access Point 'hostName'
  void processRequest(char *body, char *formData);                          // process the HTTP request
  int getFormValue(char *formData, char *tag, char *value, int maxSize);    // search for 'tag' in 'formData' and copy result into 'value' up to 'maxSize' characters; returns number of characters, else -1 if 'tag' not found
  int badRequestError();                                                    // return 400 error
  
};
