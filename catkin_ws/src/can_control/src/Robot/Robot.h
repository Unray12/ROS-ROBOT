#ifndef ROBOT_
#define ROBOT_

#include <can_control/MecanumControl.h>
#include <iostream>
#include <ros/ros.h>
#include <unordered_map>
#include <array>
using can_control::MecanumControl;

class Robot {

    private:
        const std::unordered_map<std::string, std::array<float, 4>> DIR_VR = {
            {"Forward",  {+1, +1, +1, +1}},
            {"Backward", {-1, -1, -1, -1}},
            {"Left",     {-1, +1, -1, +1}},
            {"Right",    {+1, -1, +1, -1}},
            {"Stop",     {0, 0, 0, 0}}
        };

        const std::unordered_map<std::string, int> DIR_STATE = {
            {"Forward",  1},
            {"Backward", 2},
            {"Left",     3},
            {"Right",    4},
            {"Stop",     0}
        };

    public:
        Robot() {}
        int currentState;
        int nextState;
        bool isAuto = 0;
        bool isActive = 1;
        int id = 0;
        /*
        0: Stop
        1: Forward
        2: Backward
        3: Left
        4: Right
        */
       ros::Publisher mecanumCmdPub;
        void robotDirection(std::string direction) {
            this->nextState = this->DIR_STATE.find(direction)->second;
            if (this->nextState != this->currentState) {
                this->currentState = this->nextState;
                for (uint8_t id = 0; id < 4; ++id) {
                    MecanumControl m;
                    m.device_id = id;
                    m.mode = MecanumControl::MODE_CLOSE_LOOP;
                    m.value = 0;
                    this->mecanumCmdPub.publish(m);
                }
            
                auto it = this->DIR_VR.find(direction);
                // std::cout << direction;
                
                
                if (it == this->DIR_VR.end()) {
                    ROS_WARN("Unknown VR command: %s", direction.c_str());
                    return;
                }
            
                for (uint8_t id = 0; id < 4; ++id) {
                    MecanumControl m;
                    m.device_id = id;
                    m.mode = MecanumControl::MODE_SET_TARGET;
                    m.value = it->second[id] * 15.0f;
                    this->mecanumCmdPub.publish(m);
                }
            
            }
        }

        void processModeManual(std::string direction) {
            this->robotDirection(direction);
        }

        void processModeAuto(const geometry_msgs::Twist& msg) {
            double angular_z = msg.angular.z;
            double linear_x = msg.linear.x;
            int vel = 50;
            if (angular_z > 0) {
                this->robotDirection("Left");
            }
            else if (angular_z < 0) {
                this->robotDirection("Right");
            }
            else 
                this->robotDirection("Forward");
        }

};

#endif