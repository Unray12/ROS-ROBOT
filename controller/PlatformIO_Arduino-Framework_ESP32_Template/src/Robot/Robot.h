#include <Constant/Constant.h>
#include <MotorDriverV2/MotorDriverV2.h>

#ifndef ROBOT_
#define ROBOT_

class Robot
{
private:
    MotorDriverV2 motorDriver;

public:
    Robot()
    {
    }

    void stop()
    {
        this->motorDriver.stop();
    }

    void goForward(int speed)
    {
        motorDriver.set_motors(M1, speed * -1);
        motorDriver.set_motors(M2, speed);
        motorDriver.set_motors(M3, speed * -1);
        motorDriver.set_motors(M4, speed);
    }

    void goBackward(int speed)
    {
        motorDriver.set_motors(M1, speed);
        motorDriver.set_motors(M2, speed * -1);
        motorDriver.set_motors(M3, speed);
        motorDriver.set_motors(M4, speed * -1);
    }

    void goLeft(int speed)
    {
        motorDriver.set_motors(M1, speed);
        motorDriver.set_motors(M2, speed);
        motorDriver.set_motors(M3, speed * -1);
        motorDriver.set_motors(M4, speed * -1);
    }

    void goRight(int speed)
    {
        motorDriver.set_motors(M1, speed * -1);
        motorDriver.set_motors(M2, speed * -1);
        motorDriver.set_motors(M3, speed);
        motorDriver.set_motors(M4, speed);
    }

    void turnLeft(int speed)
    {
        motorDriver.set_motors(M1, speed);
        motorDriver.set_motors(M2, speed);
        motorDriver.set_motors(M3, speed);
        motorDriver.set_motors(M4, speed);
    }

    void turnRight(int speed)
    {
        motorDriver.set_motors(M1, speed * -1);
        motorDriver.set_motors(M2, speed * -1);
        motorDriver.set_motors(M3, speed * -1);
        motorDriver.set_motors(M4, speed * -1);
    }
};

#endif