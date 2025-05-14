// #include <Wire.h>
#include <WiFi.h>
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Robot/Robot.h"
#include "ros.h"
#include <std_msgs/Float64.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include "Esp32Now/Esp32Now.h"
#include "DHT20.h"
#include <json_generator.h>
#include <string.h>
#include "std_msgs/Int32.h"

// #define SENSOR
// #define EVALUAE
#define GATEWAY
const char *ssid = "ACLAB1";
const char *password = "ACLAB2023";
IPAddress IPRosSerialServer(172, 28, 181, 211); //34 162
const uint16_t rosSerialserverPort = 11411;

const uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// #ifdef GATEWAY
esp_now_peer_info_t peerInfo;
std_msgs::String strMsg;
ros::Publisher chatter("Sensors", &strMsg);
// #endif

ros::NodeHandle_<ArduinoHardware> nodeHandle;
Robot mecanumRobot;
DHT20 dht20;

#ifdef EVALUATE
const long gmtOffset_sec = 7 * 3600;      // GMT+7 = 7 hours * 3600 seconds
const int daylightOffset_sec = 0;        // No daylight saving time
const char* ntpServer = "pool.ntp.org";
#endif

void esp32PublishTask(void *pvParameter)
{
    std_msgs::String strMsg;
    ros::Publisher chatter("Sensors", &strMsg);

    char buffer[256] = {'\0'};
    json_gen_str_t jsonString;
    json_gen_str_start(&jsonString, buffer, sizeof(buffer), NULL, NULL);

    json_gen_start_object(&jsonString);
    json_gen_obj_set_float(&jsonString, "Humidity", 10.0);
    json_gen_obj_set_float(&jsonString, "Temperature", 10.0);
    json_gen_end_object(&jsonString);

    nodeHandle.advertise(chatter);
    
    while (true)
    {
        strMsg.data = buffer;
        chatter.publish(&strMsg);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void sendSensorInfo(const uint8_t *gwMAC) {
    // esp_now_peer_info_t gwPeerInfo;
    // Serial.println("Send sensor info");
    dht20.read();
    infoSensorMsg myData(0, 1, dht20.getHumidity(), dht20.getTemperature());
    if (WiFi.status() == WL_CONNECTED) {
        // Serial.printf("Humidity: %f, Temperature: %f\n", myData.humidityValue, myData.temperatureValue);
        sendEspNow(broadcastAddress, myData);
    }
}

//publish to ROS
void publishSensorInfo(infoSensorMsg mySensorData) {
    // Serial.println("Publish ...");
    char buffer[256] = {'\0'};
    json_gen_str_t jsonString;
    json_gen_str_start(&jsonString, buffer, sizeof(buffer), NULL, NULL);
    json_gen_start_object(&jsonString);
    json_gen_obj_set_float(&jsonString, "Humidity", mySensorData.humidityValue);
    json_gen_obj_set_float(&jsonString, "Temperature", mySensorData.temperatureValue);
    json_gen_end_object(&jsonString);

    strMsg.data = buffer;
    chatter.publish(&strMsg);
}

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   Serial.print("\r\nLast Packet Send Status:\t");
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    // Serial.println("ESP NOW send");
}

// callback when data is receive
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    // Serial.println("hello receive");
    infoSensorMsg myData;
    memcpy(&myData, incomingData, sizeof(infoSensorMsg));

    if (myData.typeMessage == 0) {
    #ifdef SENSOR
        if (myData.infoSensorType == 0) {
            sendSensorInfo(mac);
        }
    #endif

    #ifdef GATEWAY 
        if (myData.infoSensorType == 1) {
            publishSensorInfo(myData);
        }
    #endif
    }
}


/* 
Linear state
0: stop
1: forward
2: backward
3: left
4: right

Angular state
0: stop
1: turn left
2: turn right
*/

void robotAction(int val) {
    // mecanumRobot.stop();
    Serial.println(mecanumRobot.nextAngularState);
    Serial.println(mecanumRobot.currentAngularState);
    if (mecanumRobot.nextAngularState != mecanumRobot.currentAngularState || mecanumRobot.nextLinearState != mecanumRobot.currentLinearState) {
        mecanumRobot.currentAngularState= mecanumRobot.nextAngularState;
        mecanumRobot.currentLinearState = mecanumRobot.nextLinearState;
        if (mecanumRobot.currentLinearState == 4) {
            mecanumRobot.goRight(val);
        } 
        else if (mecanumRobot.currentLinearState == 3) {
            mecanumRobot.goLeft(val);
            Serial.println("Hello");
        }
        else if (mecanumRobot.currentLinearState == 1) {
            mecanumRobot.goForward(val);
        }
        else if (mecanumRobot.currentLinearState == 2) {
            mecanumRobot.goBackward(val);
        }
        else if (mecanumRobot.currentAngularState == 1) {
            mecanumRobot.turnLeft(val);
        }
        else if (mecanumRobot.currentAngularState == 2) {
            mecanumRobot.turnRight(val);
        }
        else
            mecanumRobot.stop();
    }
}

void twistMessage(const geometry_msgs::Twist &msg) {
    double angular_z = msg.angular.z;
    double linear_x = msg.linear.x;
    int vel = 50;
    // Serial.println(linear_x);
    if (angular_z > 0) {
        mecanumRobot.nextAngularState = 1;
    }
    else if (angular_z < 0) {
        mecanumRobot.nextAngularState = 2;    
    }
    else {
        mecanumRobot.nextAngularState = 0;
        mecanumRobot.currentLinearState;
    }

    if (mecanumRobot.nextAngularState != mecanumRobot.currentAngularState) {
        mecanumRobot.currentAngularState = mecanumRobot.nextAngularState;
        robotAction(30);
    }
}

#ifdef EVALUATE
int sequence = 1;
#endif

void processVRMessage(const std_msgs::String &msg) {
#ifdef EVALUATE
    Serial.printf("%d %s\n", sequence, msg.data);
    sequence++;
#endif
    // mecanumRobot.goRight(30);
    if (mecanumRobot.isActive) {
        if (strcmp(msg.data, "Right") == 0) {
            mecanumRobot.nextLinearState = 4;
            mecanumRobot.nextAngularState = 0;
            Serial.println("Robot 1");
        } else if (strcmp(msg.data, "Left") == 0) {
            mecanumRobot.nextLinearState  = 3;
            mecanumRobot.nextAngularState = 0;
        } else if (strcmp(msg.data, "RotateLeft") == 0) {
            mecanumRobot.nextAngularState  = 1;
            mecanumRobot.nextLinearState = 0;
        } else if (strcmp(msg.data, "RotateRight") == 0) {
            mecanumRobot.nextAngularState  = 2;
            mecanumRobot.nextLinearState = 0;
        } else if (strcmp(msg.data, "Forward") == 0) {
            mecanumRobot.nextLinearState  = 1;
            mecanumRobot.nextAngularState = 0;
        } else if (strcmp(msg.data, "Backward") == 0) {
            mecanumRobot.nextLinearState = 2;
            mecanumRobot.nextAngularState = 0;
        } else if (strcmp(msg.data, "Stop") == 0) {
            mecanumRobot.nextLinearState = 0;
            mecanumRobot.nextAngularState = 0;
        }
        robotAction(50);
    }
}

void VRpickCallback(const std_msgs::Int32& msg) {
    if (msg.data == 0) {
        mecanumRobot.isActive = 0;
        mecanumRobot.stop();
    }
    else if (msg.data == 1)
        mecanumRobot.isActive = 1;
    else if (msg.data == 2) {
        mecanumRobot.isActive = 0;
        mecanumRobot.stop();
    }
}
ros::Subscriber<geometry_msgs::Twist> lidarSub("cmd_vel", &twistMessage);
ros::Subscriber<std_msgs::String> VRcontrolSub("VR_control", &processVRMessage);
ros::Subscriber<std_msgs::Int32> VRpickSub("pick_robot", &VRpickCallback);


void wifiTask(void *pvParameters) {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // delay(100);
        Serial.println("Connecting to WiFi..");
    }
    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());

    //init esp32 now
    intEsp32Now(OnDataSent, OnDataRecv);

    //gw add broadcast peer, sensor add gw peer
    addPeer(broadcastAddress, peerInfo);


    //take time from NTP
#ifdef EVALUATE
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    while (!getLocalTime(&timeinfo)) {
        Serial.println("Waiting for time...");
        delay(10);
      }
#endif
    // connect to rosserial server
    nodeHandle.getHardware()->setConnection(IPRosSerialServer, rosSerialserverPort);
    vTaskDelete(NULL); // Delete the task when done
}

void robotActionTask(void *pvParameter) {
    while (true) {
        if (mecanumRobot.nextAngularState != mecanumRobot.currentAngularState) {
            mecanumRobot.currentAngularState = mecanumRobot.nextAngularState;
            mecanumRobot.nextAngularState = 0;
            if (mecanumRobot.currentAngularState == 1) {
                mecanumRobot.turnLeft(30);
                Serial.println("1 left");
            } 
            else if (mecanumRobot.currentAngularState == 2) {
                mecanumRobot.turnRight(30);
                Serial.println("1 right");
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void esp32SubcribeTask(void *pvParameter) {

}

void spinOnceTask(void *pvParameter) {
    while (true)
    {
        nodeHandle.spinOnce();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

void espNowGwTask(void *pvParamater) {
    while (true) {
        Serial.print("hello");
        if (true) {
            infoSensorMsg myData;
            if (WiFi.status() == WL_CONNECTED) {
                sendEspNow(broadcastAddress, myData);
            }
        }
        #ifdef EVALUATE
        mecanumRobot.motorDriver.printTime();
        #endif
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    

    vTaskDelete(NULL);
}

void espNowSensorTask(void * pvParameter) {
    while (true) {
        if (true) {
            infoSensorMsg myData;
            dht20.read();
            myData.temperatureValue = dht20.getTemperature();
            myData.infoSensorType = dht20.getHumidity();
            if (WiFi.status() == WL_CONNECTED) {
                sendEspNow(broadcastAddress, myData);
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void setup()
{
    Serial.begin(115200);
    // xTaskCreate(wifiTask, "WiFiTask", 4096, NULL, 3, NULL);
    xTaskCreatePinnedToCore(wifiTask, "WiFiTask", 4096, NULL, 3, NULL, 1);

#ifdef SENSOR
    dht20.begin();
    xTaskCreate(espNowSensorTask, "espNowSensorTask", 4096, NULL, 1, NULL);
#endif

#ifdef GATEWAY
    nodeHandle.initNode();
    // dht20.begin();
    nodeHandle.subscribe(lidarSub);
    nodeHandle.subscribe(VRcontrolSub);
    nodeHandle.subscribe(VRpickSub);
    nodeHandle.advertise(chatter);
    mecanumRobot.stop();

    Serial.print("[DEFAULT] ESP32 Board MAC Address: ");
    readMacAddress();
    // xTaskCreate(esp32PublishTask, "esp32PublishTask", 4096, NULL, 1, NULL);
    // xTaskCreate(robotActionTask, "robotActionTask", 4096, NULL, 1, NULL);
    xTaskCreate(espNowGwTask, "espNowGwTask", 4096, NULL, 1, NULL);
    // xTaskCreate(spinOnceTask, "spinOnceTask", 4096, NULL, 1, NULL);
    xTaskCreatePinnedToCore(spinOnceTask, "spinOnceTask", 4096, NULL, 1, NULL, 1);
#endif
}

void loop()
{

}
