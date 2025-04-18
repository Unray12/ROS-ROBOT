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

// #define SENSOR
#define GATEWAY
const char *ssid = "ACLAB";
const char *password = "ACLAB2023";
IPAddress IPRosSerialServer(172, 28, 182, 162); //34 162
const uint16_t rosSerialserverPort = 11411;

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
esp_now_peer_info_t peerInfo;

ros::NodeHandle_<ArduinoHardware> nodeHandle;
Robot mecanumRobot;
DHT20 dht20;


// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
// callback when data is receive
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  infoSensorMsg myData;
  memcpy(&myData, incomingData, sizeof(infoSensorMsg));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Humid: ");
  Serial.println(myData.humidityValue);
  // Serial.print("Temp: ");
  Serial.println(myData.temperatureValue);
}
/* 
Linear state
0: stop
1: forward
2: backward

Angular state
0: stop
1: left
2: right
*/
void robotAction(int val) {
    mecanumRobot.stop();
    if (mecanumRobot.currentAngularState == 1) {
        mecanumRobot.turnLeft(val);
        Serial.println("1 left");
    } 
    else if (mecanumRobot.currentAngularState == 2) {
        mecanumRobot.turnRight(val);
        Serial.println("1 right");
    }
    else if (mecanumRobot.currentLinearState == 1) {
        mecanumRobot.goForward(val);
    }
    else if (mecanumRobot.currentLinearState == 2) {
        mecanumRobot.goBackward(val);
    }
    else
        mecanumRobot.stop();
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

void processVRMessage(const std_msgs::String &msg) {
    if (!mecanumRobot.isAutoMode) {
        if (msg.data == "Right") {
            mecanumRobot.currentAngularState = 2;
            mecanumRobot.currentLinearState = 0;
        } else if (msg.data == "Left") {
            mecanumRobot.currentAngularState = 1;
            mecanumRobot.currentLinearState = 0;
        } else if (msg.data == "Forward") {
            mecanumRobot.currentLinearState = 1;
            mecanumRobot.currentAngularState = 0;
        } else if (msg.data == "Backward") {
            mecanumRobot.currentLinearState = 2;
            mecanumRobot.currentAngularState = 0;
        }
    }
}

ros::Subscriber<geometry_msgs::Twist> lidarSub("cmd_vel", &twistMessage);
ros::Subscriber<std_msgs::String> VRcontrolSub("VR_control", &processVRMessage);

void testTask(void *pvParameters) {
    mecanumRobot.stop();
    // mecanumRobot.goForward(50);
    // delay(1000);
    // mecanumRobot.stop();
    // delay(100);

    // mecanumRobot.goBackward(50);
    // delay(1000);
    // mecanumRobot.stop();
    // delay(100);

    // mecanumRobot.goLeft(50);
    // delay(1000);
    // mecanumRobot.stop();
    // delay(100);

    // mecanumRobot.goRight(50);
    // delay(1000);
    // mecanumRobot.stop();
    // delay(100);

    // mecanumRobot.turnLeft(50);
    // delay(1000);
    // mecanumRobot.stop();
    // delay(100);

    // mecanumRobot.turnRight(50);
    // delay(1000);
    // mecanumRobot.stop();
    // delay(100);

    // mecanumRobot.stop();
    // vTaskDelete(NULL);
}

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
    #ifdef GATEWAY
        addPeer(broadcastAddress, peerInfo);
    #endif

    // connect to rosserial server
    nodeHandle.getHardware()->setConnection(IPRosSerialServer, rosSerialserverPort);
    vTaskDelete(NULL); // Delete the task when done
}

void esp32PublishTask()
{
    std_msgs::String strMsg;
    json_gen_str_t jstr;
    json_gen_start_object(&jstr);
    json_gen_obj_set_string(&jstr, "key", "value");
    json_gen_end_object(&jstr);
    ros::Publisher chatter("Sensors", &strMsg);
    char hello[13] = "Hello world!";
    
    nodeHandle.advertise(chatter);
    while (true)
    {
        strMsg.data = hello;
        Serial.println("hello");
        chatter.publish(&strMsg);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    
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
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void esp32SubcribeTask(void *pvParameter) {

}

void spinOnceTask(void *pvParameter) {
    while (true)
    {
        nodeHandle.spinOnce();
        // Serial.print("[DEFAULT] ESP32 Board MAC Address: ");
        // readMacAddress();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void espNowGwTask(void *pvParamater) {
    while (true) {
        if (true) {
            infoSensorMsg myData;
            if (WiFi.status() == WL_CONNECTED) {
                sendEspNow(broadcastAddress, myData);
            }
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
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
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);

}

void setup()
{
    Serial.begin(115200);
    pinMode(48, OUTPUT);
    xTaskCreate(wifiTask, "WiFiTask", 4096, NULL, 3, NULL);
    

#ifdef SENSOR
    dht20.begin();
    xTaskCreate(espNowSensorTask, "espNowSensorTask", 4096, NULL, 1, NULL);
#endif

#ifdef GATEWAY
    nodeHandle.initNode();
    nodeHandle.subscribe(lidarSub);
    nodeHandle.subscribe(VRcontrolSub);
    // mecanumRobot.stop();

    Serial.print("[DEFAULT] ESP32 Board MAC Address: ");
    readMacAddress();
    // xTaskCreate(esp32PublishTask, "esp32PublishTask", 4096, NULL, 1, NULL);
    xTaskCreate(robotActionTask, "robotActionTask", 4096, NULL, 1, NULL);
    xTaskCreate(espNowGwTask, "espNowGwTask", 4096, NULL, 1, NULL);
    xTaskCreate(spinOnceTask, "spinOnceTask", 4096, NULL, 1, NULL);
#endif
}

void loop()
{
}
