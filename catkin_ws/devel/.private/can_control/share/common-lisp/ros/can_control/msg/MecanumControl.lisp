; Auto-generated. Do not edit!


(cl:in-package can_control-msg)


;//! \htmlinclude MecanumControl.msg.html

(cl:defclass <MecanumControl> (roslisp-msg-protocol:ros-message)
  ((device_id
    :reader device_id
    :initarg :device_id
    :type cl:fixnum
    :initform 0)
   (mode
    :reader mode
    :initarg :mode
    :type cl:fixnum
    :initform 0)
   (value
    :reader value
    :initarg :value
    :type cl:float
    :initform 0.0))
)

(cl:defclass MecanumControl (<MecanumControl>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <MecanumControl>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'MecanumControl)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name can_control-msg:<MecanumControl> is deprecated: use can_control-msg:MecanumControl instead.")))

(cl:ensure-generic-function 'device_id-val :lambda-list '(m))
(cl:defmethod device_id-val ((m <MecanumControl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can_control-msg:device_id-val is deprecated.  Use can_control-msg:device_id instead.")
  (device_id m))

(cl:ensure-generic-function 'mode-val :lambda-list '(m))
(cl:defmethod mode-val ((m <MecanumControl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can_control-msg:mode-val is deprecated.  Use can_control-msg:mode instead.")
  (mode m))

(cl:ensure-generic-function 'value-val :lambda-list '(m))
(cl:defmethod value-val ((m <MecanumControl>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can_control-msg:value-val is deprecated.  Use can_control-msg:value instead.")
  (value m))
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql '<MecanumControl>)))
    "Constants for message type '<MecanumControl>"
  '((:MODE_CALIB . 0)
    (:MODE_IDLE . 1)
    (:MODE_CLOSE_LOOP . 2)
    (:MODE_CLEAR_ERROR . 3)
    (:MODE_HOMING . 4)
    (:MODE_SET_TARGET . 5))
)
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql 'MecanumControl)))
    "Constants for message type 'MecanumControl"
  '((:MODE_CALIB . 0)
    (:MODE_IDLE . 1)
    (:MODE_CLOSE_LOOP . 2)
    (:MODE_CLEAR_ERROR . 3)
    (:MODE_HOMING . 4)
    (:MODE_SET_TARGET . 5))
)
(cl:defmethod roslisp-msg-protocol:serialize ((msg <MecanumControl>) ostream)
  "Serializes a message object of type '<MecanumControl>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'device_id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'mode)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'value))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <MecanumControl>) istream)
  "Deserializes a message object of type '<MecanumControl>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'device_id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'mode)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'value) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<MecanumControl>)))
  "Returns string type for a message object of type '<MecanumControl>"
  "can_control/MecanumControl")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'MecanumControl)))
  "Returns string type for a message object of type 'MecanumControl"
  "can_control/MecanumControl")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<MecanumControl>)))
  "Returns md5sum for a message object of type '<MecanumControl>"
  "cc8cc5d29cc98dd7c83c44e13887d465")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'MecanumControl)))
  "Returns md5sum for a message object of type 'MecanumControl"
  "cc8cc5d29cc98dd7c83c44e13887d465")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<MecanumControl>)))
  "Returns full string definition for message of type '<MecanumControl>"
  (cl:format cl:nil "uint8 MODE_CALIB       = 0~%uint8 MODE_IDLE        = 1~%uint8 MODE_CLOSE_LOOP  = 2~%uint8 MODE_CLEAR_ERROR = 3~%uint8 MODE_HOMING      = 4~%uint8 MODE_SET_TARGET  = 5~%~%uint8 device_id~%uint8 mode~%float32 value~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'MecanumControl)))
  "Returns full string definition for message of type 'MecanumControl"
  (cl:format cl:nil "uint8 MODE_CALIB       = 0~%uint8 MODE_IDLE        = 1~%uint8 MODE_CLOSE_LOOP  = 2~%uint8 MODE_CLEAR_ERROR = 3~%uint8 MODE_HOMING      = 4~%uint8 MODE_SET_TARGET  = 5~%~%uint8 device_id~%uint8 mode~%float32 value~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <MecanumControl>))
  (cl:+ 0
     1
     1
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <MecanumControl>))
  "Converts a ROS message object to a list"
  (cl:list 'MecanumControl
    (cl:cons ':device_id (device_id msg))
    (cl:cons ':mode (mode msg))
    (cl:cons ':value (value msg))
))
