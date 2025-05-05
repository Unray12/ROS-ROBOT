run master node: roscore
run rosserial_server node: rosrun rosserial_server socket_node
run lidar node: (go to lidar folder) -> roslaunch ./launch/rplidar_a2m8.launch

run rviz

run avoider node: rosrun esp32_controller controllerPublisher.py
run ros bridge websocket: roslaunch rosbridge_server rosbridge_websocket.launch
monitor topic from esp32: rostopic echo chatter
monitor topic cmd_vel: rostopic echo cmd_vel
show all topic: rostopic list

ros2
colcon build 
//colcon build --packages-select esp32_controller
source install/setup.bash
//in setup.py at "entry point" -> 'esp32_controller = esp32_controller.main:main'
ros2 run esp32_controller esp32_controller

ros2 launch rplidar_ros rplidar_a2m8_launch.py serial_port:=/dev/ttyUSB0

// can robot
rosrun can_control drivetrai
rosrun can_control ctrl_manual 
// send cmd for unlock mode: 0 -> 3 clc
rosrun can_control ctrl_auto
rosrun can_control main


sudo slcand -o -c -s6 /dev/ttyACMx can0
sudo ip link set can0 up type can bitrate 500000
sudo ip link set can0 up
source ~/catkin_ws/devel/setup.bash

ls /dev/ttyACM* |  
roscore
cd ~/catkin_ws/src/rplidar_ros/launch/
roslauch rplidar_a2m8.launch
rosrun esp32_controller controllerPublisher.py
rosrun can_control drivetrain
rosrun can_control main
rosrun can_control ctrl_manual


#!/bin/bash
tty_device=$(ls /dev/ttyACM* 2>/dev/null | head -n 1)
sudo slcand -o -c -s6 "$tty_device" can0
sudo ip link set can0 up type can bitrate 500000
sudo ip link set can0 up

lxterminal -e "bash -c 'source /opt/ros/noetic/setup.bash; roscore; exec bash'" &
sleep 4

lxterminal -e "bash -c 'source /opt/ros/noetic/setup.bash; source ~/catkin_ws/devel/setup.bash; cd ~/catkin_ws/src/rplidar_ros/launch/ && roslaunch rplidar_a2m8.launch; exec bash'" &
sleep 1

lxterminal -e "bash -c 'source /opt/ros/noetic/setup.bash; source ~/catkin_ws/devel/setup.bash; rosrun esp32_controller controllerPublisher.py; exec bash'" &
sleep 1
 
lxterminal -e "bash -c 'source /opt/ros/noetic/setup.bash; source ~/catkin_ws/devel/setup.bash; rosrun can_control drivetrain; exec bash'" &
sleep 1

lxterminal -e "bash -c 'source /opt/ros/noetic/setup.bash; source ~/catkin_ws/devel/setup.bash; rosrun can_control main; exec bash'" &
sleep 1

lxterminal -e "bash -c 'source /opt/ros/noetic/setup.bash; source ~/catkin_ws/devel/setup.bash; rosrun can_control ctrl_manual; exec bash'"