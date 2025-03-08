#!/usr/bin/python3

import rospy
import sensor_msgs.msg
import time
import cv2
from urllib.request import urlopen
import numpy as np
from cv_bridge import CvBridge, CvBridgeError
import std_msgs.msg

STREAM_URL = "http://172.28.181.168:81/stream"
def publishImage():
    imagePub = rospy.Publisher("Image", sensor_msgs.msg.Image, queue_size=100)
    bridge = CvBridge()
    
    

    while not rospy.is_shutdown():
        capture = cv2.VideoCapture("http://172.28.181.168/capture")
        ret, img = capture.read()

        imageMsg = bridge.cv2_to_imgmsg(img, "bgr8")
        imagePub.publish(imageMsg)


if __name__ == "__main__":
    rospy.init_node("esp32_cam", anonymous=True)
    print("Publishing...")
    publishImage()
    rospy,spin()
    