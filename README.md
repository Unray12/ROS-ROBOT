run master node: roscore
run rosserial_server node: rosrun rosserial_server socket_node
run lidar node: (go to lidar folder) -> roslaunch ./launch/rplidar_a2m8.launch

run rviz

run avoider node: rosrun esp32_controller controllerPublisher.py
monitor topic from esp32: rostopic echo chatter
monitor topic cmd_vel: rostopic echo cmd_vel
show all topic: rostopic list
