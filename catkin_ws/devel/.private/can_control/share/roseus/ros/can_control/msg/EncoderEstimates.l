;; Auto-generated. Do not edit!


(when (boundp 'can_control::EncoderEstimates)
  (if (not (find-package "CAN_CONTROL"))
    (make-package "CAN_CONTROL"))
  (shadow 'EncoderEstimates (find-package "CAN_CONTROL")))
(unless (find-package "CAN_CONTROL::ENCODERESTIMATES")
  (make-package "CAN_CONTROL::ENCODERESTIMATES"))

(in-package "ROS")
;;//! \htmlinclude EncoderEstimates.msg.html


(defclass can_control::EncoderEstimates
  :super ros::object
  :slots (_device_id _position _velocity ))

(defmethod can_control::EncoderEstimates
  (:init
   (&key
    ((:device_id __device_id) 0)
    ((:position __position) 0.0)
    ((:velocity __velocity) 0.0)
    )
   (send-super :init)
   (setq _device_id (round __device_id))
   (setq _position (float __position))
   (setq _velocity (float __velocity))
   self)
  (:device_id
   (&optional __device_id)
   (if __device_id (setq _device_id __device_id)) _device_id)
  (:position
   (&optional __position)
   (if __position (setq _position __position)) _position)
  (:velocity
   (&optional __velocity)
   (if __velocity (setq _velocity __velocity)) _velocity)
  (:serialization-length
   ()
   (+
    ;; uint8 _device_id
    1
    ;; float32 _position
    4
    ;; float32 _velocity
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _device_id
       (write-byte _device_id s)
     ;; float32 _position
       (sys::poke _position (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _velocity
       (sys::poke _velocity (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _device_id
     (setq _device_id (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float32 _position
     (setq _position (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _velocity
     (setq _velocity (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get can_control::EncoderEstimates :md5sum-) "fec274c383b9ce718dfe2666eb480e27")
(setf (get can_control::EncoderEstimates :datatype-) "can_control/EncoderEstimates")
(setf (get can_control::EncoderEstimates :definition-)
      "uint8 device_id
float32 position
float32 velocity

")



(provide :can_control/EncoderEstimates "fec274c383b9ce718dfe2666eb480e27")


