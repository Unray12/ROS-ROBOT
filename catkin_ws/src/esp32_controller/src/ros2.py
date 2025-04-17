import rclpy
from Avoider_1 import Avoider
from rclpy.node import Node

from geometry_msgs.msg import Twist #ros msg that deals with moving the robot
from sensor_msgs.msg import LaserScan #ros msg that gets the laser scans

def callback_cmd_vel(msg):
    linear_x = msg.linear.x
    angular_z = msg.angular.z
    print("Linear X: %.2f, Angular Z: %.2f", linear_x, angular_z)


def main():

    vel = Twist()
    # Instanciate our avoider object
    avoider = Avoider(vel)
    # Initialize our node
    rclpy.init()
    node = rclpy.create_node('esp32_controller')
    # Subscribe to the "/scan" topic in order to read laser scans data from it
    node.create_subscription(LaserScan, '/scan', avoider.identify_regions, 10)
    #create our publisher that'll publish to the "/cmd_vel" topic
    pub = node.create_publisher(Twist, '/cmd_vel', 10)
    #ros will try to run this code 10 times/second
    rate = node.create_rate(10)   #10Hz
    
    #keep running while the ros-master isn't shutdown
    try:
        while rclpy.ok():
            rclpy.spin_once(node, timeout_sec=0.1)
            vel = avoider.avoid()
            pub.publish(vel_msg)
            rate.sleep()
    except KeyboardInterrupt:
        pass

if __name__ == "__main__":
    main()