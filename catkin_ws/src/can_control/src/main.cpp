#include <ros/ros.h>
#include <can_control/MecanumControl.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <unordered_map>
#include <array>
#include <termios.h>
#include <unistd.h>
#include "Robot/Robot.h"

using can_control::MecanumControl;

static const std::unordered_map<char, std::array<float, 4>> DIR = {
    {'8', {+1, +1, +1, +1}}, // F
    {'2', {-1, -1, -1, -1}}, // B
    {'4', {+1, -1, +1, -1}}, // R
    {'6', {-1, +1, -1, +1}}, // L
    {'7', {+1, 0, +1, 0}},   // FR
    {'9', {0, +1, 0, +1}},   // FL
    {'1', {0, -1, 0, -1}},   // BR
    {'3', {-1, 0, -1, 0}},   // BL
    {'5', {0, 0, 0, 0}}      // STOP
};

const std::unordered_map<std::string, std::array<float, 4>> DIR_VR = {
    {"Forward",  {+1, +1, +1, +1}},
    {"Backward", {-1, -1, -1, -1}},
    {"Left",     {-1, +1, -1, +1}},
    {"Right",    {+1, -1, +1, -1}},
    {"Stop",     {0, 0, 0, 0}}
};

Robot mecanumRobot;



void VRcontrolCallback(const std_msgs::String::ConstPtr& msg)
{
    mecanumRobot.robotDirection(msg->data);
}


void twistMessage(const geometry_msgs::Twist& msg) {
    double angular_z = msg.angular.z;
    double linear_x = msg.linear.x;
    int vel = 50;
    if (angular_z > 0) {
        mecanumRobot.robotDirection("Left");
    }
    else if (angular_z < 0) {
        mecanumRobot.robotDirection("Right");
    }
    else 
        mecanumRobot.robotDirection("Forward");
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "main");
    ros::NodeHandle nh;
    mecanumRobot.mecanumCmdPub = nh.advertise<MecanumControl>("mecanum_cmd", 10);
    ros::Subscriber vr_sub = nh.subscribe("VR_control", 10, VRcontrolCallback);
    //ros::Subscriber lidarSub = nh.subscribe("cmd_vel", 10, twistMessage);

    float vel = 8.0f;

    ros::Rate rate(100); // To avoid spinning too fast


    while (ros::ok())
    {
        ros::spinOnce();
    }

    return 0;
}
