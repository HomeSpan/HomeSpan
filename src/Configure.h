
#include <WiFi.h>

///////////////////////////////

struct Configure {

    void processRequest(WiFiClient &client, char *body, char *formData);
};
