#include <ros/ros.h>
#include <can_control/MecanumControl.h>
#include <can_control/EncoderEstimates.h>
#include "can_comm.hpp"
#include "odrive_motor.hpp"
#include <memory>
#include <thread>
#include <vector>

class Drivetrain
{
public:
    Drivetrain() : nh_("")
    {
        can_ = std::make_shared<CANInterface>();
        if (!can_->openInterface("can0"))
        {
            ROS_FATAL("open can0 fail");
            ros::shutdown();
            return;
        }
        for (uint8_t id = 0; id < 4; ++id)
            motors_.push_back(std::make_shared<OdriveMotor>(
                id, OdriveMotor::VELOCITY, can_.get()));

        sub_ = nh_.subscribe("mecanum_cmd", 10, &Drivetrain::onCmd, this);
        pub_enc_ = nh_.advertise<can_control::EncoderEstimates>("odrive_encoder", 10);

        can_->registerFeedbackCallback(
            [this](uint32_t can_id, const uint8_t *data)
            {
                uint8_t cmd = can_id & 0x1F;
                if (cmd != OdriveMotor::FEEDBACK_CMD_ID)
                    return;
                this->onFb(can_id, data);
            });

        std::thread([this]
                    { can_->receiveLoop(); })
            .detach();
        ROS_INFO("drivetrain ready.");
    }

private:
    ros::NodeHandle nh_;
    std::shared_ptr<CANInterface> can_;
    std::vector<std::shared_ptr<OdriveMotor>> motors_;
    ros::Subscriber sub_;
    ros::Publisher pub_enc_;

    void onCmd(const boost::shared_ptr<can_control::MecanumControl const> &m)
    {
        if (m->device_id >= motors_.size())
            return;
        auto &mot = motors_[m->device_id];
        switch (m->mode)
        {
        case can_control::MecanumControl::MODE_CALIB:
            mot->fullCalibration();
            break;
        case can_control::MecanumControl::MODE_IDLE:
            mot->idle();
            break;
        case can_control::MecanumControl::MODE_CLOSE_LOOP:
            mot->closeLoopControl();
            break;
        case can_control::MecanumControl::MODE_CLEAR_ERROR:
            mot->clearError();
            break;
        case can_control::MecanumControl::MODE_SET_TARGET:
            mot->setTarget(m->value);
            break;
        }
    }

    void onFb(uint32_t can_id, const uint8_t *data)
    {
        uint8_t device_id = can_id >> 5;

        float pos, vel;
        std::memcpy(&pos, data, 4);
        std::memcpy(&vel, data + 4, 4);

        if (device_id < motors_.size())
            motors_[device_id]->setFeedback(pos, vel);

        can_control::EncoderEstimates msg;
        msg.device_id = device_id;
        msg.position = pos;
        msg.velocity = vel;
        pub_enc_.publish(msg);
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "drivetrain");
    Drivetrain drivetrain;
    ros::spin();
    return 0;
}