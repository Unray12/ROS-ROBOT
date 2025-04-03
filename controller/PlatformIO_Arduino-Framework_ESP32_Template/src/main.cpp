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

const char* ssid = "RD-SEAI_2.4G";
const char* password = "";

IPAddress IPserver(172, 28, 182, 162);
const uint16_t serverPort = 11411;
ros::NodeHandle_<ArduinoHardware> nodeHandle;
Robot mecanumRobot;

uint8_t robotState = 0;
uint8_t robotNextState = 4;
/* 
0: go forward
1: go backward
2: rotate right
3: rotate left
4: stop
*/
void twistMessage(const geometry_msgs::Twist& msg) {
    double angular_z = msg.angular.z;
    double linear_x = msg.linear.x;
    int vel = 50;
    Serial.println(linear_x);

    if (angular_z > 0) {
        robotNextState = 3;
    }
    else if (angular_z < 0) {
        robotNextState = 2;
    }
    else 
    if (linear_x > 0) {
        robotNextState = 0;
    }
    else if (linear_x < 0) {
        robotNextState = 1;
    }
    else
        robotNextState = 4;

    if (robotNextState != robotState) {
        robotState = robotNextState;
        if (angular_z > 0) {
            mecanumRobot.turnLeft(30);
        }
        else if (angular_z < 0) {
            mecanumRobot.turnRight(30);
        }
        else 
        if (linear_x > 0) {
            mecanumRobot.goForward(30);
            Serial.println("go forward");
        }
        else if (linear_x < 0) {
            mecanumRobot.goBackward(30);
            Serial.println("go backward");
        }
        else
            mecanumRobot.goBackward(0);
            robotNextState = 4;
            Serial.println("hello");
    }
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &twistMessage);




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
    while (WiFi.status() != WL_CONNECTED) {
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      Serial.println("Connecting to WiFi..");
    }
  
    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());

    //connect to rosserial server
    nodeHandle.getHardware()->setConnection(IPserver, serverPort);
    vTaskDelete(NULL);  // Delete the task when done
}

void esp32PublishTask(void *pvParameter) {
    std_msgs::String strMsg;
    ros::Publisher chatter("chatter", &strMsg);
    char hello[13] = "Hello world!";
    nodeHandle.initNode();
    nodeHandle.advertise(chatter);
    while (true) {
        strMsg.data = hello;
        Serial.println("hello");
        chatter.publish(&strMsg);
        nodeHandle.spinOnce();
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }
}

void esp32SubcribeTask(void *pvParameter) {

}

void spinOnceTask(void *pvParameter) {
    while (true) {
        nodeHandle.spinOnce();
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
    
}
void setup()
{
    Serial.begin(115200);

    pinMode(48, OUTPUT);
    nodeHandle.subscribe(sub);
    xTaskCreate(wifiTask, "WiFiTask", 4096, NULL, 1, NULL);
    
    
    // xTaskCreate(testTask, "testTask", 4096, NULL, 1, NULL);
    xTaskCreate(esp32PublishTask, "esp32PublishTask", 4096, NULL, 1, NULL);
    xTaskCreate(spinOnceTask, "spinOnceTask", 4096, NULL, 1, NULL);
}

void loop()
{

}
