#ifndef ESP32NOW_
#define ESP32NOW_

#include <esp_now.h>
#include <esp_wifi.h>
#include <Arduino.h>

extern esp_now_peer_info_t peerInfo;
typedef struct struct_message {
    char a[32];
    int b;
    float c;
    bool d;
  } struct_message;
void intEsp32Now(void (*OnDataSentCB)(const uint8_t *mac_addr, esp_now_send_status_t status), void (*OnDataRcvCB)(const uint8_t * mac, const uint8_t *incomingData, int len));
void readMacAddress();
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void sendEspNow(uint8_t *peerMAC, char* data, size_t dataSize);
void addPeer(uint8_t* peerMAC, esp_now_peer_info_t peerInfo);
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
#endif