
#include "Now.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <mbedtls/sha256.h>
    
void SpanNow::start(const char *macAddress, const char *password){

  if(sscanf(macAddress,"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",mac,mac+1,mac+2,mac+3,mac+4,mac+5)!=6){
    Serial.printf("*** ERROR:  Can't start HomeSpan NOW!  Bad MAC Address '%s'\n\n",macAddress);
    return;
  }

  statusQueue = xQueueCreate(1,sizeof(esp_now_send_status_t));

  WiFi.mode(WIFI_AP_STA);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_now_init();
  esp_now_register_send_cb(onDataSent);
  
  uint8_t lmk[32];
  
  mbedtls_sha256_ret((const unsigned char *)password,strlen(password),lmk,0);
  esp_now_set_pmk(lmk+16);

  memcpy(peerInfo.peer_addr, mac, 6);
  peerInfo.encrypt = true;
  memcpy(peerInfo.lmk, lmk, 16);
  esp_now_add_peer(&peerInfo);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);

  Serial.printf("Started HomeSpan NOW on Channel=%d.  Hub MAC Address: %X:%X:%X:%X:%X:%X\n",channel,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
  started=true;
}

boolean SpanNow::send(uint8_t *data, size_t len){
  
  if(!started){
    Serial.printf("*** ERROR:  Can't send data until HomeSpanNOW has been started.\n\n");
    return(false);
    }

  esp_now_send_status_t status = ESP_NOW_SEND_FAIL;

  for(int c=0;c<13;c++){
    if((1<<channel) & channelMask){
      for(int i=1;i<=3;i++){
        Serial.printf("Sending on channel %d, attempt #%d\n",channel,i);
        esp_now_send(mac, data, len);
        xQueueReceive(statusQueue, &status, pdMS_TO_TICKS(2000));
        if(status==ESP_NOW_SEND_SUCCESS)
          return(true);
        delay(10);
      }
    }
    channel++;
    if(channel==14)
      channel=1;
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  }
  return(false);
}    

QueueHandle_t SpanNow::statusQueue;

SpanNow homeSpanNow;
