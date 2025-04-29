#include <ros/ros.h>
#include <can_control/MecanumControl.h>
#include <iostream>
#include <string>
#include <algorithm>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "ctrl_manual");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<can_control::MecanumControl>("mecanum_cmd", 10);

    ros::Rate loop_rate(10); 

    std::cout << "Manual mode: [id] [cmd] [value?]\n"
              << "  cmd=calib|idle|clc|clear|target\n";
    
    while (ros::ok())
    {
        uint32_t id;
        std::string cmd;
        float v = 0.0f;
        if (!(std::cin >> id >> cmd))
            break;
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        can_control::MecanumControl msg;
        msg.device_id = id;

        if (cmd == "target")
        {
            if (!(std::cin >> v))
            {
                std::cerr << "Need value\n";
                continue;
            }
            msg.mode = can_control::MecanumControl::MODE_SET_TARGET;
            msg.value = v;
        }
        else
        {
            msg.value = 0.0f;
            if (cmd == "calib")
                msg.mode = can_control::MecanumControl::MODE_CALIB;
            else if (cmd == "idle")
                msg.mode = can_control::MecanumControl::MODE_IDLE;
            else if (cmd == "clc")
                msg.mode = can_control::MecanumControl::MODE_CLOSE_LOOP;
            else if (cmd == "clear")
                msg.mode = can_control::MecanumControl::MODE_CLEAR_ERROR;
            else
            {
                std::cerr << "Unknown cmd\n";
                continue;
            }
        }

        pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}