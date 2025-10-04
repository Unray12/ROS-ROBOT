import paho.mqtt.client as mqtt
import sys
import os
import termios
import tty
import rospy
import std_msgs.msg
import threading
# MQTT settings
BROKER_ADDRESS = "172.28.182.68"  # Free public broker
MQTT_PORT = 1883
TOPICS = ['VR_control']
os.environ['ROS_MASTER_URI'] = 'http://172.28.182.68:11311'
os.environ['ROS_IP'] = '172.28.182.165'
client = mqtt.Client()

# Hàm đọc phím 1 ký tự
def get_key():
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    try:
        tty.setraw(fd)
        ch = sys.stdin.read(1)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    return ch

# Callback when connected to broker
def on_connect(client, userdata, flags, rc):
    print("Connected to broker " + BROKER_ADDRESS)
    if rc == 0:
        for topic in TOPICS:
            client.subscribe(topic)
    else:
        print(f"Failed to connect, return code {rc}")

def on_message(client, userdata, msg):
    print(f"Received message: '{msg.payload.decode()}' on topic '{msg.topic}'")
    if(msg.topic == "VR_control"):
        ros_pub = rospy.Publisher("/VR_control", std_msgs.msg.String, queue_size=10)
        ros_pub.publish(msg.payload.decode())

def on_subscribe(mosq, obj, mid, granted_qos):
    print("Subscribed: " "include message: "+ str(mid))

def on_publish(mosq, obj, mid):
    print(f"Mesage {mid} has been sent to broker")

def mqtt_thread():
    global client
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(BROKER_ADDRESS, MQTT_PORT, 60)
    client.subscribe(TOPICS[0], 0)
    # client.publish(TOPICS[0], "Right")
    client.loop_forever()   # Block ở đây nhưng nằm trong thread riêng


    



def main():
    # ROS setup
    # global ros_pub
    ros_pub = rospy.Publisher("/VR_control", std_msgs.msg.String, queue_size=10)
    rate = rospy.Rate(100)


    # vel_msg = Twist()
    print("Điều khiển robot bằng phím WASD, nhấn 'q' để thoát")
    MQTT_Thread = threading.Thread(target=mqtt_thread, daemon=True)
    MQTT_Thread.start()

    while not rospy.is_shutdown():
        key = get_key().lower()
        cmd = ""

        if key == "w":
            cmd = "Forward"
            print("➡️  Gửi: forward")
        elif key == "s":
            cmd = "Backward"
            print("⬅️  Gửi: backward")
        elif key == "a":
            cmd = "Left"
            print("⬅️  Gửi: left")
        elif key == "d":
            cmd = "Right"
            print("➡️  Gửi: right")
        elif key == " ":
            cmd = "Stop"
            print("⏹️  Gửi: stop")
        elif key == "j":
            print("Rotate left")
            cmd = "RotateLeft"
        elif key == "k":
            print("Rotate right")
            cmd = "RotateRight"
        elif key == "q":
            print("❌ Thoát chương trình.")
            break
        
        if cmd:

            # ros_pub = rospy.Publisher("/VR_control", std_msgs.msg.String, queue_size=10)
            # ros_pub.publish(cmd)
            # Publish MQTT
            
            client.publish(TOPICS[0], cmd)
            # rospy.loginfo(f"Gửi: {cmd}")
        rate.sleep()
         
    client.loop_stop()
    client.disconnect()


if __name__ == "__main__":
    rospy.init_node("MQTTBridgeROS", anonymous=True)
    print("Publishing...")
    main()
    rospy.spin()