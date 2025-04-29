; Auto-generated. Do not edit!


(cl:in-package can_control-msg)


;//! \htmlinclude EncoderEstimates.msg.html

(cl:defclass <EncoderEstimates> (roslisp-msg-protocol:ros-message)
  ((device_id
    :reader device_id
    :initarg :device_id
    :type cl:fixnum
    :initform 0)
   (position
    :reader position
    :initarg :position
    :type cl:float
    :initform 0.0)
   (velocity
    :reader velocity
    :initarg :velocity
    :type cl:float
    :initform 0.0))
)

(cl:defclass EncoderEstimates (<EncoderEstimates>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <EncoderEstimates>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'EncoderEstimates)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name can_control-msg:<EncoderEstimates> is deprecated: use can_control-msg:EncoderEstimates instead.")))

(cl:ensure-generic-function 'device_id-val :lambda-list '(m))
(cl:defmethod device_id-val ((m <EncoderEstimates>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can_control-msg:device_id-val is deprecated.  Use can_control-msg:device_id instead.")
  (device_id m))

(cl:ensure-generic-function 'position-val :lambda-list '(m))
(cl:defmethod position-val ((m <EncoderEstimates>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can_control-msg:position-val is deprecated.  Use can_control-msg:position instead.")
  (position m))

(cl:ensure-generic-function 'velocity-val :lambda-list '(m))
(cl:defmethod velocity-val ((m <EncoderEstimates>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader can_control-msg:velocity-val is deprecated.  Use can_control-msg:velocity instead.")
  (velocity m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <EncoderEstimates>) ostream)
  "Serializes a message object of type '<EncoderEstimates>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'device_id)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'position))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'velocity))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <EncoderEstimates>) istream)
  "Deserializes a message object of type '<EncoderEstimates>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'device_id)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'position) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'velocity) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<EncoderEstimates>)))
  "Returns string type for a message object of type '<EncoderEstimates>"
  "can_control/EncoderEstimates")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'EncoderEstimates)))
  "Returns string type for a message object of type 'EncoderEstimates"
  "can_control/EncoderEstimates")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<EncoderEstimates>)))
  "Returns md5sum for a message object of type '<EncoderEstimates>"
  "fec274c383b9ce718dfe2666eb480e27")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'EncoderEstimates)))
  "Returns md5sum for a message object of type 'EncoderEstimates"
  "fec274c383b9ce718dfe2666eb480e27")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<EncoderEstimates>)))
  "Returns full string definition for message of type '<EncoderEstimates>"
  (cl:format cl:nil "uint8 device_id~%float32 position~%float32 velocity~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'EncoderEstimates)))
  "Returns full string definition for message of type 'EncoderEstimates"
  (cl:format cl:nil "uint8 device_id~%float32 position~%float32 velocity~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <EncoderEstimates>))
  (cl:+ 0
     1
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <EncoderEstimates>))
  "Converts a ROS message object to a list"
  (cl:list 'EncoderEstimates
    (cl:cons ':device_id (device_id msg))
    (cl:cons ':position (position msg))
    (cl:cons ':velocity (velocity msg))
))
