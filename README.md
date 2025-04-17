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

