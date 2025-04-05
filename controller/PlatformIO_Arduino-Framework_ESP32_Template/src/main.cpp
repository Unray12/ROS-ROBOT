#include <WiFi.h>
#include "Esp32Now/Esp32Now.h"

uint8_t broadcastAddress[] = {0xCC, 0xBA, 0x97, 0x0D, 0xE4, 0xA0};
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
// callback when data is receive
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  infoSensorMsg myData;
  memcpy(&myData, incomingData, sizeof(myData));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Humid: ");
  Serial.println(myData.humidityValue);
  // Serial.print("Temp: ");
  Serial.println(myData.temperatureValue);
}

void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("[DEFAULT] ESP32 Board MAC Address: ");
  readMacAddress();
  intEsp32Now(OnDataSent, OnDataRecv);
  addPeer(broadcastAddress, peerInfo);
}


void loop(){
  infoSensorMsg myData;
  myData.temperatureValue = 10;
  myData.humidityValue = 20;
  myData.typeMessage = 0;
  sendEspNow(broadcastAddress, myData);
  Serial.println("hello");
  delay(1000);
}
