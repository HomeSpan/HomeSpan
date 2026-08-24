
#include "SpanPoint.h"

///////////////////////////////

void SpanPoint::configure(uint8_t deviceID, SpConfig_t cfg){

  if(configured){
    Serial.printf("\nFATAL ERROR!  SpanPoint already configured with Device ID of %hhu! ***\n",deviceID);
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }

  if(deviceID==0){
    Serial.printf("\nFATAL ERROR!  Can't configure SpanPoint - Device ID must be greater than zero ***\n");
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }

  WiFi.mode(WIFI_AP);         // set the mode to AP
  delay(10);
  wifi_set_channel(1);        // you also need to manually set the channel to match whatever channel is used by the ESP32 after it connects to your WiFi network

  deviceAddress = new SpAddress(deviceID,cfg.network);
  wifi_set_macaddr(SOFTAP_IF, deviceAddress->mac);

  esp_now_init();                           // initialize ESP-NOW

  mKey = new MasterKey(cfg.password.c_str(),"SpanPoint");

  uint8_t pmk[16];
  mKey->create("Key for PMK",pmk,16); 
  esp_now_set_kok(pmk,16);

  localHMAC = new HMAC(mKey,deviceAddress->mac,6);

  // esp_now_register_recv_cb(version==2 ? dataReceivedV2 : dataReceivedV1);                       // set callback for receiving data based on version
  // statusQueue = xQueueCreate(1,sizeof(esp_now_send_status_t));                                  // create statusQueue even if not needed

  // esp_now_register_send_cb([](const esp_now_send_info_t *mac, esp_now_send_status_t status){    // create callback for sending data
  //   xQueueOverwrite( statusQueue, &status );
  // });

  // spConf.channelMask=cfg.channelMask;                             // save a subset of the config data that will needed in other functions
  // spConf.encrypt=cfg.encrypt;                                         
  // spConf.channelSelector=cfg.channelSelector;                                         

  // initializeChannels();                                           // verify channel mask and set first channel
  // configured=true;                                                // set configured to true 
}

///////////////////////////////

boolean send(const void *data){

    uint8_t msg[36];
    memcpy(msg,data,4);

    SpanPoint::localHMAC->create(msg,4,msg+4);

  //  esp_now_send(remoteAddress.mac, (uint8_t *)&temp, sizeof(temp));     // Send the Data to the Main Device!
  //  esp_now_send(remoteAddress.mac, msg, 36);     // Send the Data to the Main Device!
    return(true);
}

///////////////////////////////



//vector<SpanPoint *, Mallocator<SpanPoint *>> SpanPoint::SpanPoints;
SpanPoint::SpAddress *SpanPoint::deviceAddress=NULL;
SpanPoint::SpConfig_t SpanPoint::spConf{};
boolean SpanPoint::configured=false;
MasterKey *SpanPoint::mKey;
HMAC *SpanPoint::localHMAC;
volatile int SpanPoint::sendStatus=-1;

///////////////////////////////


