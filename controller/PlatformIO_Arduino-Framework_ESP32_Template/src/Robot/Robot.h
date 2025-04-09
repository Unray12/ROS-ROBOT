#include <Constant/Constant.h>
#include <MotorDriverV2/MotorDriverV2.h>

#ifndef ROBOT_
#define ROBOT_

class Robot
{
private:
    MotorDriverV2 motorDriver;
    /* 
    0: go forward
    1: go backward
    2: rotate right
    3: rotate left
    4: stop
    */



public:
    int currentLinearState = 1;
    int nextLinearState = 1;
    int currentAngularState = 0;
    int nextAngularState = 0;

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