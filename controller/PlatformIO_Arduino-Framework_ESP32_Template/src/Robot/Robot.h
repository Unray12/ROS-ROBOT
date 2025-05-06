#include <Constant/Constant.h>
#include <MotorDriverV2/MotorDriverV2.h>

#ifndef ROBOT_
#define ROBOT_

class Robot
{
private:
    
    /* 
    0: go forward
    1: go backward
    2: rotate right
    3: rotate left
    4: stop
    */



public:
    MotorDriverV2 motorDriver;
    bool isAutoMode = 0;
    int currentLinearState = 0;
    int nextLinearState = 0;
    int currentAngularState = 0;
    int nextAngularState = 0;
    bool isActive = 0;
    int id = 1;

    Robot()
    {
    }

    void stop()
    {
        this->motorDriver.stop();
    }

    void goForward(int speed)
    {
        motorDriver.set_motors(M1, speed);
        motorDriver.set_motors(M2, speed);
        motorDriver.set_motors(M3, speed);
        motorDriver.set_motors(M4, speed * -1);
    }

    void goBackward(int speed)
    {
        motorDriver.set_motors(M1, speed * -1);
        motorDriver.set_motors(M2, speed * -1);
        motorDriver.set_motors(M3, speed * -1);
        motorDriver.set_motors(M4, speed);
    }

    void goLeft(int speed)
    {
        motorDriver.set_motors(M1, speed * -1);
        motorDriver.set_motors(M2, speed);
        motorDriver.set_motors(M3, speed);
        motorDriver.set_motors(M4, speed);
    }

    void goRight(int speed)
    {
        motorDriver.set_motors(M1, speed);
        motorDriver.set_motors(M2, speed * -1);
        motorDriver.set_motors(M3, speed * -1);
        motorDriver.set_motors(M4, speed* -1);
    }

    void turnLeft(int speed)
    {
        motorDriver.set_motors(M1, speed);
        motorDriver.set_motors(M2, speed * -1);
        motorDriver.set_motors(M3, speed);
        motorDriver.set_motors(M4, speed);
    }

    void turnRight(int speed)
    {
        motorDriver.set_motors(M1, speed * -1);
        motorDriver.set_motors(M2, speed );
        motorDriver.set_motors(M3, speed * -1);
        motorDriver.set_motors(M4, speed * -1);
    }

};

#endif