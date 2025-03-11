// #include <Wire.h>
#include <WiFi.h>
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Robot/Robot.h"
#include "ros.h"
#include <std_msgs/Float64.h>
#include <std_msgs/String.h>

const char* ssid = "RD-SEAI_2.4G";
const char* password = "";

IPAddress IPserver(172, 28, 182, 162);
const uint16_t serverPort = 11411;
ros::NodeHandle_<ArduinoHardware> nodeHandle;
std_msgs::String strMsg;
ros::Publisher chatter("chatter", &strMsg);
char hello[13] = "Hello world!";

Robot mecanumRobot;
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
