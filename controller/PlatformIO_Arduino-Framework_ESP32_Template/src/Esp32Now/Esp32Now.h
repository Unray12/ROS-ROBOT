#ifndef ESP32NOW_
#define ESP32NOW_

#include <esp_now.h>
#include <esp_wifi.h>
#include <Arduino.h>

// extern esp_now_peer_info_t peerInfo;
typedef struct infoSensorMsg {
    uint8_t typeMessage;
    //take info sensor type: 0
    uint8_t infoSensorType;
    //request sensor info: 0
    //reply sensor info: 1
    uint16_t humidityValue;
    uint16_t temperatureValue;

    infoSensorMsg(uint8_t typeMsg = 0, uint8_t sensorType = 0, uint16_t humidity = 0, uint16_t temperature = 0) 
    : typeMessage(typeMsg), infoSensorType(sensorType), humidityValue(humidity), temperatureValue(temperature) {}
} infoSensorMsg;



void intEsp32Now(void (*OnDataSentCB)(const uint8_t *mac_addr, esp_now_send_status_t status), void (*OnDataRcvCB)(const uint8_t * mac, const uint8_t *incomingData, int len));
void readMacAddress();
void sendEspNow(uint8_t *peerMAC, infoSensorMsg myData);
void addPeer(uint8_t* peerMAC, esp_now_peer_info_t &peerInfo);
#endif