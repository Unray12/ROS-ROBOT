#!/usr/bin/python3
import cv2
import numpy as np
from pyzbar.pyzbar import decode
import threading
import queue
import time
import std_msgs.msg
import sensor_msgs.msg
import rospy
from cv_bridge import CvBridge, CvBridgeError
from pynput import keyboard
import json

class RTSPQRScanner:
    def __init__(self, rtsp_url):

        self.rtsp_url = rtsp_url
        self.frame_queue = queue.Queue(maxsize=5)
        self.result_queue = queue.Queue()
        self.running = False
        self.cap = None
        
    def start(self):
        """Scanning QR"""
        self.running = True

        self.capture_thread = threading.Thread(target=self._capture_frames)
        self.process_thread = threading.Thread(target=self._process_frames)
        
        self.capture_thread.start()
        self.process_thread.start()
        
    def stop(self):
        """Stop scanning QR"""
        self.running = False
        if self.cap is not None:
            self.cap.release()
        self.capture_thread.join()
        self.process_thread.join()

    def publishImage(self, img):
        imagePub = rospy.Publisher("Image", sensor_msgs.msg.Image, queue_size=100)
        bridge = CvBridge()
        
        

        # while not rospy.is_shutdown():
            # capture = cv2.VideoCapture("http://172.28.181.168/capture")
        # capture = cv2Capture
        # ret, img = capture.read()

        imageMsg = bridge.cv2_to_imgmsg(img, "bgr8")
        imagePub.publish(imageMsg)
        
    def _capture_frames(self):
        """Capture frames"""
        self.cap = cv2.VideoCapture(self.rtsp_url)
        # self.publishImage(self.cap)

        self.cap.set(cv2.CAP_PROP_BUFFERSIZE, 3)
        self.cap.set(cv2.CAP_PROP_FPS, 60)  # Đặt FPS tối đa
        
        while self.running:
            ret, frame = self.cap.read()
            if not ret:
                print("Không thể đọc frame từ RTSP")
                continue
                
            self.publishImage(frame)
            frame = cv2.resize(frame, (640, 480), interpolation=cv2.INTER_LINEAR)
            
            if self.frame_queue.full():
                try:
                    self.frame_queue.get_nowait()
                except queue.Empty:
                    pass
                    
            self.frame_queue.put(frame)
            
        self.cap.release()
        
    def _process_frames(self):
        """Process frames and scan QR"""
        while self.running:
            try:
                frame = self.frame_queue.get(timeout=1.0)
                
                gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
                
                qr_codes = decode(gray)
                
                for qr in qr_codes:
                    qr_data = qr.data.decode('utf-8')
                    self.result_queue.put(qr_data)
                    
                    points = qr.polygon
                    if len(points) > 4:
                        hull = cv2.convexHull(np.array([point for point in points], dtype=np.float32))
                        points = hull
                        
                    n = len(points)
                    for j in range(0, n):
                        cv2.line(frame, (points[j].x, points[j].y), 
                               (points[(j+1) % n].x, points[(j+1) % n].y), 
                               (0, 255, 0), 2)
                
                cv2.imshow('QR Scanner', frame)
                if cv2.waitKey(1) & 0xFF == ord('q'):
                    self.running = False
                    
                self.frame_queue.task_done()
                
            except queue.Empty:
                continue


    def QRresultJson(self, raw_text):
        result_dict = {}
        for line in raw_text.split("\n"):
            if ": " in line:
                key, value = line.split(": ", 1)
                result_dict[key.strip()] = value.strip()
        return result_dict

    def get_result(self):
        """Taking the newest QR result"""
        try:
            # return self.result_queue.get_nowait()
            resultJson = self.QRresultJson(self.result_queue.get_nowait())
            QR_pub = rospy.Publisher("/information", std_msgs.msg.String, queue_size = 1)
            QR_pub.publish(json.dumps(resultJson))

        except queue.Empty:
            return None

def main():
    rtsp_url = "rtsp://admin:ACLAB2023@172.28.182.200/ISAPI/Streaming/channels/1"
    
    scanner = RTSPQRScanner(rtsp_url)
    scanner.start()
    
    try:
        while True:
            result = scanner.get_result()
            if result:
                print(f"QR Code detected: {result}")
            time.sleep(0.01)
            
    except KeyboardInterrupt:
        print("Stopping")
        
    scanner.stop()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    rospy.init_node("esp32_cam", anonymous=True)
    print("Publishing...")
    # publishImage()
    
    main()
    rospy.spin()