run master node: roscore
run rosserial_server node: rosrun rosserial_server socket_node
run lidar node: (go to lidar folder) -> roslaunch ./launch/rplidar_a2m8.launch

run rviz
monitor topic from esp32: rostopic echo chatter
show all topic: rostopic list
