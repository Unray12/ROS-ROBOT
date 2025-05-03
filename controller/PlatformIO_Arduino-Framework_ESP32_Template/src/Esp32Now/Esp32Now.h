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
    float humidityValue;
    float temperatureValue;
    uint32_t checksum;

    infoSensorMsg(uint8_t typeMsg = 0, uint8_t sensorType = 0, float humidity = 0, float temperature = 0) 
    : typeMessage(typeMsg), infoSensorType(sensorType), humidityValue(humidity), temperatureValue(temperature){}

    void computeChecksum();
    bool validateChecksum() const;

    static uint32_t fnv1aHash(const void* data, size_t len);
} infoSensorMsg;


void intEsp32Now(void (*OnDataSentCB)(const uint8_t *mac_addr, esp_now_send_status_t status), void (*OnDataRcvCB)(const uint8_t * mac, const uint8_t *incomingData, int len));
void readMacAddress();
void sendEspNow(const uint8_t *peerMAC, infoSensorMsg myData);
void addPeer(const uint8_t* peerMAC, esp_now_peer_info_t &peerInfo);
#endif