
#include "SpanPoint.h"
#include <EEPROM.h>

///////////////////////////////

void SpanPoint::configure(uint8_t deviceID, SpConfig_t cfg){

  if(configured){
    Serial.printf("\nFATAL ERROR!  SpanPoint already configured with Device ID of %hhu! ***\n",deviceID);
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }

  WiFi.mode(WIFI_STA);
  delay(10);
  deviceAddress = new SpAddress(deviceID,cfg.network);
  wifi_set_macaddr(STATION_IF, deviceAddress->mac);

  esp_now_init();

  mKey = new MasterKey(cfg.password.c_str(),"SpanPoint");

  uint8_t pmk[ESP_NOW_KEY_LEN];
  mKey->create("Key for PMK",pmk,ESP_NOW_KEY_LEN); 
  esp_now_set_kok(pmk,ESP_NOW_KEY_LEN);

  localHMAC = new HMAC(mKey,deviceAddress->mac,6);

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // esp_now_register_recv_cb(version==2 ? dataReceivedV2 : dataReceivedV1);                       // set callback for receiving data based on version

  esp_now_register_send_cb([](uint8_t *mac_addr, uint8_t status){                                  // create callback for sending data
    SpanPoint::sendStatus = (status==0 ? SpanPoint::ESP_NOW_SEND_SUCCESS : SpanPoint::ESP_NOW_SEND_FAIL);
  });

  spConf.channelMask=cfg.channelMask;                             // save a subset of the config data that will needed in other functions
  initializeChannels();                                           // verify channel mask and set first channel
  configured=true;                                                // set configured to true 
}

///////////////////////////////

SpanPoint::SpanPoint(uint8_t deviceID, size_t sendSize, size_t receiveSize, size_t queueDepth){

  if(!configured){
    Serial.printf("\nFATAL ERROR!  Can't create new SpanPoint(%d,%d,%d,%d) - SpanPoint not yet configured! ***\n",deviceID,sendSize,receiveSize,queueDepth);
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }

  SpAddress destAddress(deviceID, deviceAddress->netID);
  memcpy(peerInfo.peer_addr,destAddress.mac,6);

  if(deviceID==deviceAddress->devID || esp_now_is_peer_exist(destAddress.mac)){
    Serial.printf("\nFATAL ERROR!  Can't create new SpanPoint(%d,%d,%d,%d) - deviceID already used ***\n",deviceID,sendSize,receiveSize,queueDepth);
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }

  if(sendSize>(ESP_NOW_MAX_DATA_LEN-crypto_auth_BYTES) || receiveSize>(ESP_NOW_MAX_DATA_LEN-crypto_auth_BYTES) || (sendSize==0 && receiveSize==0)){
    Serial.printf("\nFATAL ERROR!  Can't create new SpanPoint(%d,%d,%d,%d) - invalid send/receive size parameters ***\n",deviceID,sendSize,receiveSize,queueDepth);
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }
  
  this->sendSize=sendSize;
  this->receiveSize=receiveSize;

  uint8_t lmk[ESP_NOW_KEY_LEN];
  char *keyContext;
  asprintf(&keyContext,"Key for LMK: NetID=%hu DevID1=%hhu DevID2=%hhu",deviceAddress->netID,
            deviceID<(deviceAddress->devID)?deviceID:deviceAddress->devID,
            deviceID>(deviceAddress->devID)?deviceID:deviceAddress->devID);

  SpanPoint::mKey->create(keyContext,lmk,ESP_NOW_KEY_LEN);
  free(keyContext);

  if(spConf.encrypt)
    esp_now_add_peer(peerInfo.peer_addr, ESP_NOW_ROLE_COMBO, 0, lmk, ESP_NOW_KEY_LEN);
  else
    esp_now_add_peer(peerInfo.peer_addr, ESP_NOW_ROLE_COMBO, 0, NULL, 0);

  // if(receiveSize>0){
  //   receiveQueue = xQueueCreate(queueDepth>0?queueDepth:1,receiveSize);
  //   overwriteQueue=(queueDepth==0);
  // }


//  SpanPoints.push_back(this);             
}

///////////////////////////////

boolean SpanPoint::send(const void *data){

  if(sendSize==0)
    return(false);
  
  uint8_t channel = wifi_get_channel();
  uint8_t startingChannel=channel;              // set starting channel to current channel

  const SpAddress *destAddress = (SpAddress *)peerInfo.peer_addr;

  size_t msgSize=sendSize+crypto_auth_BYTES;                       // size of message with HMAC
  uint8_t *msg=(uint8_t *)malloc(msgSize);                         // allocate new memory reflecting large size
  memcpy(msg,data,sendSize);                                       // copy data into first part of memory block
  localHMAC->create(msg,sendSize,msg+sendSize);                    // create HMAC from authKey and load into second part of memory block

  sendStatus=ESP_NOW_SEND_IDLE;

  do {
    for(int i=0; sendStatus!=ESP_NOW_SEND_SUCCESS && i<3; i++){      
      Serial.printf("SpanPoint: Sending %d bytes to node %hhu using WiFi channel %hhu... ",sendSize,destAddress->devID,channel);
      sendStatus=ESP_NOW_SEND_IDLE;
      esp_now_send(peerInfo.peer_addr, msg, msgSize);
      while(sendStatus==ESP_NOW_SEND_IDLE)
        delay(10);
      Serial.printf("%s\n",sendStatus==ESP_NOW_SEND_SUCCESS ? "Success!" : "Failed.");
    }    
  } while(sendStatus!=ESP_NOW_SEND_SUCCESS && (channel=nextChannel(channel))!=startingChannel);

  if(sendStatus!=ESP_NOW_SEND_SUCCESS)
    Serial.printf("SpanPoint: ERROR! Node %hhu on Network %hu unreachable.\n",destAddress->devID,deviceAddress->netID);

  free(msg);

  return(sendStatus==ESP_NOW_SEND_SUCCESS);
}

///////////////////////////////

void SpanPoint::initializeChannels(){

  wifi_country_t country;
  wifi_get_country(&country);
  spConf.channelMask=spConf.channelMask & ((1<<country.nchan)-1)<<country.schan;     // overlay country-specific mask (e.g. channels 1-11, 1-13, or 1-14 only)  

  if(spConf.channelMask==0)
    return;

  uint8_t channel=0;
  EEPROM.begin(1);
  channel=EEPROM.read(0) & 0x0F;

  for(int i=0;i<16;i++,channel=(channel+1)%16){               // loop over all mask bits (starting with saved channel)
    if(spConf.channelMask & (1<<channel)){                    // if channel is allowed by channel mask
      if(i>0){
        EEPROM.write(0,channel);
        EEPROM.commit();
      }
      wifi_promiscuous_enable(true);
      wifi_set_channel(channel);
      wifi_promiscuous_enable(false);
      return;
    }
  }
}

///////////////////////////////

uint8_t SpanPoint::nextChannel(uint8_t channel){

  // do NOT change channel if channel mask is set to zero or channel mask does not allow for any other channels

  if((spConf.channelMask==0) || spConf.channelMask==(1<<channel))
    return(channel);

  do {
    channel=(channel<13)?channel+1:1;              // advance to next channel
  } while(!(spConf.channelMask & (1<<channel)));   // until we find next valid one

  wifi_promiscuous_enable(true);
  wifi_set_channel(channel);                       // set the WiFi channel
  wifi_promiscuous_enable(false);
  
  EEPROM.write(0,channel);
  EEPROM.commit();  
     
  return(channel);  
}

///////////////////////////////

//vector<SpanPoint *, Mallocator<SpanPoint *>> SpanPoint::SpanPoints;
SpanPoint::SpAddress *SpanPoint::deviceAddress=NULL;
SpanPoint::SpConfig_t SpanPoint::spConf{};
boolean SpanPoint::configured=false;
MasterKey *SpanPoint::mKey;
HMAC *SpanPoint::localHMAC;
volatile SpanPoint::esp_now_send_status_t SpanPoint::sendStatus;

///////////////////////////////


