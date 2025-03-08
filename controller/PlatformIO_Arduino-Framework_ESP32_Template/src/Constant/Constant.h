#include <cstdint>
#ifndef CONSTANT_
#define CONSTANT_

// Constants
const uint8_t MDV2_STEPPER1 = 0;
const uint8_t MDV2_STEPPER2 = 1;
const uint8_t MDV2_DEFAULT_I2C_ADDRESS = 0x54;

// Version command
const uint8_t CMD_FIRMWARE_INFO = 0x00;

const uint8_t MDV2_REG_RESET_ENC = 0;
const uint8_t MDV2_REG_MOTOR_INDEX = 16; // set motor speed - motor index
const uint8_t MDV2_REG_MOTOR_SPEED = 18; // set motor speed - speed
const uint8_t MDV2_REG_MOTOR_BRAKE = 22;
const uint8_t MDV2_REG_REVERSE = 23;

const uint8_t MDV2_REG_SERVO1 = 24;
const uint8_t MDV2_REG_SERVO2 = 26;
const uint8_t MDV2_REG_SERVO3 = 28;
const uint8_t MDV2_REG_SERVO4 = 30;
const uint8_t MDV2_REG_SERVOS[] = {MDV2_REG_SERVO1, MDV2_REG_SERVO2, MDV2_REG_SERVO3, MDV2_REG_SERVO4};

// Read-only registers
const uint8_t MDV2_REG_FW_VERSION = 40;
const uint8_t MDV2_REG_WHO_AM_I = 42;
const uint8_t MDV2_REG_BATTERY = 43;
const uint8_t MDV2_REG_ENCODER1 = 44;
const uint8_t MDV2_REG_ENCODER2 = 48;
const uint8_t MDV2_REG_SPEED_E1 = 52;
const uint8_t MDV2_REG_SPEED_E2 = 54;

const uint16_t MOTOR_FREQ = 1000;

const uint8_t M3_IN1_PIN = 2;
const uint8_t M3_IN2_PIN = 3;
const uint8_t M4_IN1_PIN = 4;
const uint8_t M4_IN2_PIN = 5;

const uint8_t ALL = 63;
const uint8_t M3 = 4;
const uint8_t M4 = 8;
const uint8_t M1 = 1;
const uint8_t M2 = 2;
const uint8_t E1 = 16;
const uint8_t E2 = 32;

const uint8_t SCL_PIN = 12;
const uint8_t SDA_PIN = 11;
#endif