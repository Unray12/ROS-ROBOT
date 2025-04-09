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

const char *ssid = "RD-SEAI_2.4G";
const char *password = "";
IPAddress IPRosSerialServer(172, 28, 182, 162);
const uint16_t rosSerialserverPort = 11411;

ros::NodeHandle_<ArduinoHardware> nodeHandle;
Robot mecanumRobot;

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
    else 
        mecanumRobot.nextAngularState = 0;


    // if (mecanumRobot.nextState != mecanumRobot.currentState) {
    //     mecanumRobot.currentState = mecanumRobot.nextState;

    //     // liX > 0 & angZ > 0 -> forward, left
    //     // liX > 0 & angZ < 0 -> forward, right
    //     // liX < 0 & angZ > 0 -> back, left
    //     // liX < 0 & angZ < 0 -> back, right
    //     if (linear_x > 0 && angular_z > 0) {

    //     } 
    //     else if (linear_x > 0 && angular_z < 0) {

    //     }
    //     else if (linear_x < 0 && angular_z > 0) {

    //     }
    //     else if (linear_x < 0 && angular_z < 0) {
            
    //     }
        
    //     mecanumRobot.nextState = 4;
    //     Serial.println("hello");
    // }
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
    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        Serial.println("Connecting to WiFi..");
    }

    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());

    // connect to rosserial server
    nodeHandle.getHardware()->setConnection(IPRosSerialServer, rosSerialserverPort);
    vTaskDelete(NULL); // Delete the task when done
}

void esp32PublishTask(void *pvParameter)
{
    std_msgs::String strMsg;
    ros::Publisher chatter("chatter", &strMsg);
    char hello[13] = "Hello world!";
    nodeHandle.initNode();
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
        }

        if (mecanumRobot.currentAngularState > 0) {
            mecanumRobot.turnLeft(30);
        } 
        else if (mecanumRobot.currentAngularState < 0) {
            mecanumRobot.turnRight(30);
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
        vTaskDelay(10 / portTICK_PERIOD_MS);
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
    xTaskCreate(robotActionTask, "robotActionTask", 4096, NULL, 1, NULL);
    xTaskCreate(spinOnceTask, "spinOnceTask", 4096, NULL, 1, NULL);
}

void loop()
{
}
