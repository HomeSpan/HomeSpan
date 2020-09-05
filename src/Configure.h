
#include <WiFi.h>
#include "Settings.h"

///////////////////////////////

struct Configure {

  struct {                           
    char ssid[MAX_SSID+1];            
    char pwd[MAX_PWD+1];
  } wifiData;

  char setupCode[9];
  unsigned long timer;          // time of trying to connect to WiFi

  void processRequest(WiFiClient &client, char *body, char *formData);
  int getFormValue(char *formData, char *tag, char *value, int maxSize);
  
};
