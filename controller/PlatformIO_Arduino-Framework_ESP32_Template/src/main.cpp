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
            Serial.println("hellio");
    }
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &twistMessage);
std_msgs::String strMsg;
ros::Publisher chatter("chatter", &strMsg);
char hello[13] = "Hello world!";



void testTask(void *pvParameters)
{
    mecanumRobot.goForward(50);
    delay(1000);
    mecanumRobot.stop();
    delay(100);

    mecanumRobot.goBackward(50);
    delay(1000);
    mecanumRobot.stop();
    delay(100);

    mecanumRobot.goLeft(50);
    delay(1000);
    mecanumRobot.stop();
    delay(100);

    mecanumRobot.goRight(50);
    delay(1000);
    mecanumRobot.stop();
    delay(100);

    mecanumRobot.turnLeft(50);
    delay(1000);
    mecanumRobot.stop();
    delay(100);

    mecanumRobot.turnRight(50);
    delay(1000);
    mecanumRobot.stop();
    delay(100);

    mecanumRobot.stop();
    vTaskDelete(NULL);
}

void wifiTask(void *pvParameters) {
    Serial.begin(115200);
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


void setup()
{
    Serial.begin(115200);

    pinMode(48, OUTPUT);
    WiFi.mode(WIFI_STA);
    xTaskCreate(wifiTask, "WiFiTask", 4096, NULL, 1, NULL);
    
    
    // mecanumRobot.stop();
    // xTaskCreate(testTask, "testTask", 4096, NULL, 1, NULL);
    

    nodeHandle.initNode();
    nodeHandle.advertise(chatter);
    nodeHandle.subscribe(sub);
    Serial.println("Hello");

    // delay(10000);
}

void loop()
{
    digitalWrite(48, HIGH);
    // Serial.println("Hello");
    strMsg.data = hello;
    chatter.publish(&strMsg);
    nodeHandle.spinOnce();
    delay(1000);
}
