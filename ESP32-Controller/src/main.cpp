#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Chân kết nối L298N
// 2 front wheel
const int ENA = 4;   // Chân PWM điều khiển tốc độ
const int ENB = 13;
const int IN1 = 18;  // Chân điều khiển chiều quay
const int IN2 = 19;
const int IN3 = 5;
const int IN4 = 21;

// 2 back wheel
const int ENA_BACK = 4;
const int ENB_BACK = 5;
const int IN1_BACK = 18;
const int IN2_BACK = 19;
const int IN3_BACK = 20;
const int IN4_BACK = 21;

const int freq = 30000; 
const int pwmChannel0 = 0; 
const int pwmChannel1 = 1;
const int pwmChannel0_BACK = 0; 
const int pwmChannel1_BACK = 1;

const int resolution = 8; // Độ phân giải 8 bit (0-255)

//speed from 0 to 100 -> convert any value out of range into range
int convert100toResolution(int speed) {
  if (speed >= 0)
    return (speed * 255) / 100;
  // else if (speed > 100)
  //   return 255;
  else return 0;
}

void clockwiseRotate(int inPin) {

}

void goForward(int speed) {
  ledcWrite(pwmChannel0, convert100toResolution(speed));
  ledcWrite(pwmChannel1, convert100toResolution(speed));
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);


  ledcWrite(pwmChannel0_BACK, convert100toResolution(speed));
  ledcWrite(pwmChannel1_BACK, convert100toResolution(speed));
  digitalWrite(IN1_BACK, HIGH);
  digitalWrite(IN2_BACK, LOW);
  digitalWrite(IN3_BACK, LOW);
  digitalWrite(IN4_BACK, HIGH);
}

void goBackward(int speed) {
  ledcWrite(pwmChannel0, convert100toResolution(speed));
  ledcWrite(pwmChannel1, convert100toResolution(speed));
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);


  ledcWrite(pwmChannel0_BACK, convert100toResolution(speed));
  ledcWrite(pwmChannel1_BACK, convert100toResolution(speed));
  digitalWrite(IN1_BACK, LOW);
  digitalWrite(IN2_BACK, HIGH);
  digitalWrite(IN3_BACK, HIGH);
  digitalWrite(IN4_BACK, LOW);
}

void goLeft(int speed) {

}

void goRight(int speed) {

}

void rotateLeft(int speed) {

}

void rotateRight(int speed) {

}

void setup() {
  // clockwise, see from back, red -> out1
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA_BACK, OUTPUT);
  pinMode(ENB_BACK, OUTPUT);
  pinMode(IN1_BACK, OUTPUT);
  pinMode(IN2_BACK, OUTPUT);
  pinMode(IN3_BACK, OUTPUT);
  pinMode(IN4_BACK, OUTPUT);

  //turn off all motors
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);


  digitalWrite(IN1_BACK, LOW);
  digitalWrite(IN2_BACK, LOW);
  digitalWrite(IN3_BACK, LOW);
  digitalWrite(IN4_BACK, LOW);


  // PWM config
  ledcSetup(pwmChannel0, freq, resolution);
  ledcAttachPin(ENA, pwmChannel0);
  ledcSetup(pwmChannel1, freq, resolution);
  ledcAttachPin(ENB, pwmChannel1);


  ledcSetup(pwmChannel0_BACK, freq, resolution);
  ledcAttachPin(ENA_BACK, pwmChannel0_BACK);
  ledcSetup(pwmChannel1_BACK, freq, resolution);
  ledcAttachPin(ENB_BACK, pwmChannel1_BACK);

  Serial.begin(115200);
}

void loop() {
  // clockwise, see from back, red -> out1
  // analogWrite(ENA, convert100toResolution(100));
  goForward(100);
  delay(1000);
  goBackward(100);
  delay(1000);
}