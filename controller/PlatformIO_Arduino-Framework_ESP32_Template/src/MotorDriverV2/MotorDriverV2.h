#include <cstdint>
#include <Wire.h>
#include <Constant/Constant.h>
#include <Arduino.h>
#ifndef MOTORDRIVERV2_
#define MOTORDRIVERV2_

class MotorDriverV2
{
private:
    uint8_t _addr;
    int32_t _encoders[2];
    int16_t _speeds[2];
    uint8_t _reverse[2];

    // PWM channels (using Arduino's LEDC for ESP32)
    uint8_t m3_1_channel = 0;
    uint8_t m3_2_channel = 1;
    uint8_t m4_1_channel = 2;
    uint8_t m4_2_channel = 3;

    bool m3_speed;
    bool m4_speed;

    // Utility function to map values
    int translate(int value, int in_min, int in_max, int out_min, int out_max)
    {
        return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

public:
    MotorDriverV2(uint8_t address = MDV2_DEFAULT_I2C_ADDRESS)
    {
        _addr = address;
        _encoders[0] = 0;
        _encoders[1] = 0;
        _speeds[0] = 0;
        _speeds[1] = 0;
        _reverse[0] = 0;
        _reverse[1] = 0;
        m3_speed = false;
        m4_speed = false;

        // Initialize I2C
        Wire.begin(SDA_PIN, SCL_PIN);

        // Setup PWM channels for ESP32
        // ledcSetup(m3_1_channel, MOTOR_FREQ, 10); // 10-bit resolution
        // ledcSetup(m3_2_channel, MOTOR_FREQ, 10);
        // ledcSetup(m4_1_channel, MOTOR_FREQ, 10);
        // ledcSetup(m4_2_channel, MOTOR_FREQ, 10);

        // ledcAttachPin(M3_IN1_PIN, m3_1_channel);
        // ledcAttachPin(M3_IN2_PIN, m3_2_channel);
        // ledcAttachPin(M4_IN1_PIN, m4_1_channel);
        // ledcAttachPin(M4_IN2_PIN, m4_2_channel);

        // Check I2C connection
        // uint8_t who_am_i = _read_8(MDV2_REG_WHO_AM_I);
        // if (who_am_i != MDV2_DEFAULT_I2C_ADDRESS) {
        //     Serial.println("Motor driver not found. Expected: " + String(address) +
        //                   ", scanned: " + String(who_am_i));
        //     while(1); // Equivalent to raising RuntimeError
        // }
        // stop(ALL);
    }

    String fw_version()
    {
        uint8_t minor = _read_8(MDV2_REG_FW_VERSION);
        uint8_t major = _read_8(MDV2_REG_FW_VERSION + 1);
        return String(major) + "." + String(minor);
    }

    float battery()
    {
        uint8_t battery = _read_8(MDV2_REG_BATTERY);
        return round(battery / 10.0 * 10) / 10.0;
    }

private:
    void _set_motors_esp(uint8_t index, int value = 0)
    {
        value = constrain(value, -100, 100);

        if (index == M3 && !m3_speed)
        {
            ledcWrite(m3_1_channel, 0);
            ledcWrite(m3_2_channel, 0);
        }
        if (index == M4 && !m4_speed)
        {
            ledcWrite(m4_1_channel, 0);
            ledcWrite(m4_2_channel, 0);
        }

        uint16_t duty = translate(abs(value), 0, 100, 0, 1023);

        if (value >= 0)
        {
            if (index == M3)
            {
                ledcWrite(m3_2_channel, 0);
                ledcWrite(m3_1_channel, duty);
                m3_speed = true;
            }
            if (index == M4)
            {
                ledcWrite(m4_2_channel, 0);
                ledcWrite(m4_1_channel, duty);
                m4_speed = true;
            }
        }
        else
        {
            if (index == M3)
            {
                ledcWrite(m3_1_channel, 0);
                ledcWrite(m3_2_channel, duty);
                m3_speed = false;
            }
            if (index == M4)
            {
                ledcWrite(m4_1_channel, 0);
                ledcWrite(m4_2_channel, duty);
                m4_speed = false;
            }
        }
    }

    void _brake_motors_esp(uint8_t index)
    {
        if (index == M3)
        {
            pinMode(M3_IN1_PIN, OUTPUT);
            pinMode(M3_IN2_PIN, OUTPUT);
            digitalWrite(M3_IN1_PIN, HIGH);
            digitalWrite(M3_IN2_PIN, HIGH);
            m3_speed = false;
        }
        if (index == M4)
        {
            pinMode(M4_IN1_PIN, OUTPUT);
            pinMode(M4_IN2_PIN, OUTPUT);
            digitalWrite(M4_IN1_PIN, HIGH);
            digitalWrite(M4_IN2_PIN, HIGH);
            m4_speed = false;
        }
    }

public:
    void set_motors(uint8_t motors, int speed)
    {
        uint16_t data[] = {motors, (uint16_t)(speed * 10)};
        _write_16_array(MDV2_REG_MOTOR_INDEX, data, 2);
        // for (uint8_t i : {M3, M4}) {
        //     if (motors & i) {
        //         _set_motors_esp(i, speed);
        //     }
        // }
    }

    void stop(uint8_t motors = ALL)
    {
        // _set_motors_esp(motors, 0);
        set_motors(motors, 0);
    }

    void brake(uint8_t motors = ALL)
    {
        _write_8(MDV2_REG_MOTOR_BRAKE, motors);
        for (uint8_t i : {M3, M4})
        {
            if (motors & i)
            {
                _brake_motors_esp(i);
            }
        }
    }

    void set_servo(uint8_t index, int angle, int max = 180)
    {
        angle = (angle * 180) / max;
        _write_16(MDV2_REG_SERVOS[index], angle);
    }

    int32_t get_encoder(uint8_t motors = ALL)
    {
        _read_32_array(MDV2_REG_ENCODER1, _encoders, 2);
        if (motors == ALL)
            return _encoders[0]; // Note: only returning first encoder
        else if (motors & E1)
            return _encoders[0];
        else if (motors & E2)
            return _encoders[1];
        return 0;
    }

    void reset_encoder(uint8_t motors = ALL)
    {
        _write_8(MDV2_REG_RESET_ENC, motors);
    }

    void reverse_encoder(uint8_t motors)
    {
        _write_8(MDV2_REG_REVERSE, motors);
    }

    int16_t get_speed(uint8_t motor = ALL)
    {
        _read_16_array(MDV2_REG_SPEED_E1, _speeds, 2);
        if (motor == ALL)
            return _speeds[0]; // Note: only returning first speed
        else if (motor & E1)
            return _speeds[0];
        else if (motor & E2)
            return _speeds[1];
        return 0;
    }

    // I2C Commands
private:
    void _write_8(uint8_t register_addr, uint8_t data)
    {
        Wire.beginTransmission(_addr);
        Wire.write(register_addr);
        Wire.write(data);
        Wire.endTransmission();
    }

    void _write_16(uint8_t register_addr, uint16_t data)
    {
        Wire.beginTransmission(_addr);
        Wire.write(register_addr);
        Wire.write(data & 0xFF);
        Wire.write((data >> 8) & 0xFF);
        Wire.endTransmission();
    }

    void _write_16_array(uint8_t register_addr, uint16_t *data, size_t length)
    {
        Wire.beginTransmission(_addr);
        Wire.write(register_addr);
        for (size_t i = 0; i < length; i++)
        {
            Wire.write(data[i] & 0xFF);
            Wire.write((data[i] >> 8) & 0xFF);
        }
        Wire.endTransmission();
        int status = Wire.endTransmission();
        Serial.println("hellllllllllllllllllllllo");
        Serial.println(status);
    }

    //   void _write_16_array(uint8_t register_addr, uint16_t* data, size_t length) {
    //     uint8_t buffer[2 * length];  // Tạo bộ đệm chứa dữ liệu

    //     // Chuyển đổi dữ liệu 16-bit thành dạng Little Endian
    //     for (size_t i = 0; i < length; i++) {
    //         buffer[2 * i] = data[i] & 0xFF;         // Byte thấp (LSB)
    //         buffer[2 * i + 1] = (data[i] >> 8) & 0xFF; // Byte cao (MSB)
    //     }

    //     Wire.beginTransmission(_addr);
    //     Wire.write(register_addr);
    //     Wire.write(buffer, 2 * length);
    //     int status = Wire.endTransmission();
    //     Serial.println("hellllllllllllllllllllllo");
    //     Serial.println(status);
    // }

    uint8_t _read_8(uint8_t register_addr)
    {
        Wire.beginTransmission(_addr);
        Wire.write(register_addr);
        Wire.endTransmission(false);
        Wire.requestFrom(_addr, (uint8_t)1);
        return Wire.read();
    }

    int16_t _read_16(uint8_t register_addr)
    {
        Wire.beginTransmission(_addr);
        Wire.write(register_addr);
        Wire.endTransmission(false);
        Wire.requestFrom(_addr, (uint8_t)2);
        uint16_t raw = (Wire.read() << 8) | Wire.read();
        return (raw & (1 << 15)) ? (raw - (1 << 16)) : raw;
    }

    void _read_16_array(uint8_t register_addr, int16_t *result_array, size_t length)
    {
        Wire.beginTransmission(_addr);
        Wire.write(register_addr);
        Wire.endTransmission(false);
        Wire.requestFrom(_addr, (uint8_t)(2 * length));
        for (size_t i = 0; i < length; i++)
        {
            uint16_t raw = (Wire.read() << 8) | Wire.read();
            result_array[i] = (raw & (1 << 15)) ? (raw - (1 << 16)) : raw;
        }
    }

    int32_t _read_32(uint8_t register_addr)
    {
        Wire.beginTransmission(_addr);
        Wire.write(register_addr);
        Wire.endTransmission(false);
        Wire.requestFrom(_addr, (uint8_t)4);
        uint32_t raw = (Wire.read() << 24) | (Wire.read() << 16) |
                       (Wire.read() << 8) | Wire.read();
        return (raw & (1 << 31)) ? (raw - (1 << 32)) : raw;
    }

    void _read_32_array(uint8_t register_addr, int32_t *result_array, size_t length)
    {
        Wire.beginTransmission(_addr);
        Wire.write(register_addr);
        Wire.endTransmission(false);
        Wire.requestFrom(_addr, (uint8_t)(4 * length));
        for (size_t i = 0; i < length; i++)
        {
            uint32_t raw = (Wire.read() << 24) | (Wire.read() << 16) |
                           (Wire.read() << 8) | Wire.read();
            result_array[i] = (raw & (1 << 31)) ? (raw - (1 << 32)) : raw;
        }
    }
};



#endif