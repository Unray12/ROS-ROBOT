#include <WiFi.h>
// #include <esp_wifi.h>
// #include <esp_now.h>
#include "Esp32Now/Esp32Now.h"

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
esp_now_peer_info_t peerInfo;


void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("[DEFAULT] ESP32 Board MAC Address: ");
  readMacAddress();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  // esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  addPeer(broadcastAddress, peerInfo);
}


void loop(){
  char text1[] = "THIS";
  sendEspNow(broadcastAddress, text1, 3);
  Serial.println("hello");
  delay(1000);
}
