#include "Esp32Now.h"




uint32_t infoSensorMsg::fnv1aHash(const void* data, size_t len) {
  uint32_t hash = 2166136261u;
  const uint8_t* ptr = static_cast<const uint8_t*>(data);
  for (size_t i = 0; i < len; ++i) {
      hash ^= ptr[i];
      hash *= 16777619u;
  }
  return hash;
}


void infoSensorMsg::computeChecksum() {
  size_t sizeWithoutChecksum = sizeof(infoSensorMsg) - sizeof(checksum);
  checksum = fnv1aHash(this, sizeWithoutChecksum);
}

bool infoSensorMsg::validateChecksum() const {
  size_t sizeWithoutChecksum = sizeof(infoSensorMsg) - sizeof(checksum);
  return checksum == fnv1aHash(this, sizeWithoutChecksum);
}


// {0xCC, 0xBA, 0x97, 0x0D, 0xE4, 0xA0};
void readMacAddress(){
    uint8_t baseMac[6];
    esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
    if (ret == ESP_OK) {
      Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                    baseMac[0], baseMac[1], baseMac[2],
                    baseMac[3], baseMac[4], baseMac[5]);
    } else {
      Serial.println("Failed to read MAC address");
    }
}

void intEsp32Now(void (*OnDataSentCB)(const uint8_t *mac_addr, esp_now_send_status_t status), void (*OnDataRcvCB)(const uint8_t * mac, const uint8_t *incomingData, int len)) {
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    else {
      Serial.println("ESP init");
    }
    esp_now_register_send_cb(OnDataSentCB);
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRcvCB));
}

void sendEspNow(const uint8_t* peerMAC, infoSensorMsg myData) {

    infoSensorMsg myDataTemp = myData;
    // strcpy(myData.a, data);
  
    esp_err_t result = esp_now_send(peerMAC, (uint8_t *) &myData, sizeof(myData));
    // Serial.print(sizeof(myData));
    if (result == ESP_OK) {
      // Serial.println("Sent with success");
    }
    else {
      // Serial.println("Error sending the data");
    }
}

void addPeer(const uint8_t*  peerMAC, esp_now_peer_info_t &peerInfo) {
    // Register peer
    memcpy(peerInfo.peer_addr, peerMAC, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
  
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer");
      return;
    }
}


