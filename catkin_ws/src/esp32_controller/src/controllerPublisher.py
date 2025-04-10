#!/usr/bin/python3
# # -*- coding: utf-8 -*-

# import rospy
# from sensor_msgs.msg import LaserScan
# from geometry_msgs.msg import Twist

# class LidarObstacleAvoidance:
#     def __init__(self):
#         # Khởi tạo node
#         rospy.init_node('esp32_controller', anonymous=True)

#         # Subscriber để lấy dữ liệu Lidar từ topic /scan
#         self.scan_subscriber = rospy.Subscriber('/scan', LaserScan, self.scan_callback)

#         # Publisher để gửi lệnh vận tốc cho robot
#         self.cmd_vel_publisher = rospy.Publisher('/cmd_vel', Twist, queue_size=10)

#         # Khởi tạo tốc độ mặc định
#         self.linear_speed = 0.2  # m/s
#         self.angular_speed = 0.5  # rad/s

#         # Khoảng cách an toàn để tránh vật cản
#         self.min_safe_distance = 0.5  # m

#         # Biến lưu giá trị quét mới nhất
#         self.regions = {
#             'right': float('inf'),
#             'front': float('inf'),
#             'left': float('inf')
#         }

#     def scan_callback(self, msg):
#         # Chia khoảng quét thành 3 vùng: phải, trước, trái
#         ranges = msg.ranges
#         length = len(ranges)

#         # Cắt vùng quét (theo góc)
#         right_ranges = ranges[:length//3]
#         front_ranges = ranges[length//3: 2*length//3]
#         left_ranges = ranges[2*length//3:]

#         # Tính khoảng cách gần nhất trong mỗi vùng
#         self.regions = {
#             'right': min(min(right_ranges), 10),  # Giới hạn khoảng cách tối đa để tránh lỗi
#             'front': min(min(front_ranges), 10),
#             'left': min(min(left_ranges), 10)
#         }

#         rospy.loginfo(f"Vùng quét: {self.regions}")

#         # Xử lý logic tránh vật cản
#         self.avoid_obstacles()

#     def avoid_obstacles(self):
#         twist = Twist()

#         if self.regions['front'] < self.min_safe_distance:
#             rospy.logwarn("Vật cản phía trước! Quay đầu...")
#             twist.linear.x = 0.0
#             # Nếu khoảng bên phải rộng hơn thì quay phải, ngược lại quay trái
#             if self.regions['right'] > self.regions['left']:
#                 twist.angular.z = -self.angular_speed  # quay phải
#             else:
#                 twist.angular.z = self.angular_speed  # quay trái
#         else:
#             rospy.loginfo("Đường thông thoáng. Tiếp tục tiến.")
#             twist.linear.x = self.linear_speed
#             twist.angular.z = 0.0

#         # Publish lệnh vận tốc
#         self.cmd_vel_publisher.publish(twist)

#     def run(self):
#         rospy.spin()


# if __name__ == '__main__':
#     try:
#         node = LidarObstacleAvoidance()
#         node.run()
#     except rospy.ROSInterruptException:
#         pass


from Avoider_1 import Avoider # type: ignore

import rospy
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
    rospy.init_node("esp32_controller")
    # Subscribe to the "/scan" topic in order to read laser scans data from it
    rospy.Subscriber("/scan", LaserScan, avoider.identify_regions)
    #create our publisher that'll publish to the "/cmd_vel" topic
    pub = rospy.Publisher("/cmd_vel", Twist, queue_size = 1)
    #ros will try to run this code 10 times/second
    rate = rospy.Rate(10) #10Hz
    
    #keep running while the ros-master isn't shutdown
    while not rospy.is_shutdown():
        vel = avoider.avoid()
        pub.publish(vel)
        rate.sleep()

if __name__ == "__main__":
    try:
        main()
    except rospy.ROSInterruptException:
        pass