#include <ros/ros.h>
#include <can_control/MecanumControl.h>
#include <iostream>
#include <unordered_map>
#include <array>
#include <termios.h>
#include <unistd.h>

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

class RawMode
{
    termios orig_;

public:
    RawMode()
    {
        tcgetattr(STDIN_FILENO, &orig_);
        termios raw = orig_;
        raw.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    }

    ~RawMode()
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_);
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "ctrl_auto");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<MecanumControl>("mecanum_cmd", 10);

    float vel = 8.0f;
    RawMode _raw;

    ros::Rate rate(100); // To avoid spinning too fast

    while (ros::ok())
    {
        char c;
        if (::read(STDIN_FILENO, &c, 1) != 1)
            continue;

        if (c == '+' || c == '=')
        {
            vel += 5;
            std::cout << "vel=" << vel << "\n";
            continue;
        }

        if (c == '-')
        {
            vel -= 5;
            if (vel < 0)
                vel = 0;
            std::cout << "vel=" << vel << "\n";
            continue;
        }

        if (c == 'q')
            break;

        if (c == '0' || c == '\n')
        {
            uint8_t mode = (c == '0') ? MecanumControl::MODE_IDLE : MecanumControl::MODE_CLOSE_LOOP;
            for (uint8_t id = 0; id < 4; ++id)
            {
                MecanumControl m;
                m.device_id = id;
                m.mode = mode;
                m.value = 0;
                pub.publish(m);
            }
            ros::spinOnce();
            continue;
        }

        auto it = DIR.find(c);
        if (it == DIR.end())
            continue;

        for (uint8_t id = 0; id < 4; ++id)
        {
            MecanumControl m;
            m.device_id = id;
            m.mode = MecanumControl::MODE_SET_TARGET;
            m.value = it->second[id] * vel;
            pub.publish(m);
        }

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
