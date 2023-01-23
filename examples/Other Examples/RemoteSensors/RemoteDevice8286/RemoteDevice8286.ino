
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Crypto.h>

float temp=-10.0;

uint8_t main_mac[6]={0x84,0xCC,0xA8,0x11,0xB4,0x85};

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.printf("Last Packet Send Status: %s\n",sendStatus==0?"Success":"Fail");
}

void setup() {

  Serial.begin(115200);
  delay(1000); 
  Serial.printf("\nMAC Address: %s\n",WiFi.macAddress().c_str());
  
  WiFi.mode(WIFI_STA);
  wifi_set_channel(6);
  
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  uint8_t hash[32];
  char password[]="HomeSpan";
  
  experimental::crypto::SHA256::hash(password,strlen(password),hash);
  
  esp_now_register_send_cb(OnDataSent);
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  esp_now_set_kok(hash+16,16);
        
  esp_now_add_peer(main_mac, ESP_NOW_ROLE_COMBO, 0, hash, 16);
}

void loop() {

  Serial.printf("Sending Temperature: %f\n",temp);  
  esp_now_send(main_mac, (uint8_t *)&temp, sizeof(temp));

  temp+=0.5;
  if(temp>35.0)
    temp=-10.0;

  delay(5000);
}
