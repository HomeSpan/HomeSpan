
#include <Arduino.h>
#include <esp_now.h>


class SpanNow {

  esp_now_peer_info_t peerInfo;
  uint8_t mac[6];
  boolean started=false;
  static QueueHandle_t statusQueue;  
  int channel=1;
  uint16_t channelMask=0x3FFE;

  static void onDataSent(const uint8_t *mac, esp_now_send_status_t status) {
    xQueueOverwrite( statusQueue, &status );
  }

  public:

  void setChannelMask(uint16_t cm){channelMask = cm & 0x3FFE;}  
  void start(const char *macAddress, const char *password="HomeSpan");
  boolean send(uint8_t *data, size_t len);  
  
};
