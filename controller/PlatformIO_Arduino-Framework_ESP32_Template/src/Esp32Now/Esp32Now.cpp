#include "Esp32Now.h"


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

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void sendEspNow(uint8_t* peerMAC, char* data, size_t dataSize) {

    struct_message myData;
    strcpy(myData.a, data);
    myData.b = random(1,20);
    myData.c = 1.2;
    myData.d = false;
  
    esp_err_t result = esp_now_send(peerMAC, (uint8_t *) &myData, dataSize);

    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
}

void addPeer(uint8_t*  peerMAC, esp_now_peer_info_t peerInfo) {
    // Register peer
    memcpy(peerInfo.peer_addr, peerMAC, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
  
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer");
      return;
    }
}


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    struct_message myData;
    memcpy(&myData, incomingData, sizeof(myData));
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Char: ");
    Serial.println(myData.a);
    Serial.print("Int: ");
    Serial.println(myData.b);
    Serial.print("Float: ");
    Serial.println(myData.c);
    Serial.print("Bool: ");
    Serial.println(myData.d);
    Serial.println();
  }
