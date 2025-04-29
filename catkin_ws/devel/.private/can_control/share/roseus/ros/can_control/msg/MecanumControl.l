;; Auto-generated. Do not edit!


(when (boundp 'can_control::MecanumControl)
  (if (not (find-package "CAN_CONTROL"))
    (make-package "CAN_CONTROL"))
  (shadow 'MecanumControl (find-package "CAN_CONTROL")))
(unless (find-package "CAN_CONTROL::MECANUMCONTROL")
  (make-package "CAN_CONTROL::MECANUMCONTROL"))

(in-package "ROS")
;;//! \htmlinclude MecanumControl.msg.html


(intern "*MODE_CALIB*" (find-package "CAN_CONTROL::MECANUMCONTROL"))
(shadow '*MODE_CALIB* (find-package "CAN_CONTROL::MECANUMCONTROL"))
(defconstant can_control::MecanumControl::*MODE_CALIB* 0)
(intern "*MODE_IDLE*" (find-package "CAN_CONTROL::MECANUMCONTROL"))
(shadow '*MODE_IDLE* (find-package "CAN_CONTROL::MECANUMCONTROL"))
(defconstant can_control::MecanumControl::*MODE_IDLE* 1)
(intern "*MODE_CLOSE_LOOP*" (find-package "CAN_CONTROL::MECANUMCONTROL"))
(shadow '*MODE_CLOSE_LOOP* (find-package "CAN_CONTROL::MECANUMCONTROL"))
(defconstant can_control::MecanumControl::*MODE_CLOSE_LOOP* 2)
(intern "*MODE_CLEAR_ERROR*" (find-package "CAN_CONTROL::MECANUMCONTROL"))
(shadow '*MODE_CLEAR_ERROR* (find-package "CAN_CONTROL::MECANUMCONTROL"))
(defconstant can_control::MecanumControl::*MODE_CLEAR_ERROR* 3)
(intern "*MODE_HOMING*" (find-package "CAN_CONTROL::MECANUMCONTROL"))
(shadow '*MODE_HOMING* (find-package "CAN_CONTROL::MECANUMCONTROL"))
(defconstant can_control::MecanumControl::*MODE_HOMING* 4)
(intern "*MODE_SET_TARGET*" (find-package "CAN_CONTROL::MECANUMCONTROL"))
(shadow '*MODE_SET_TARGET* (find-package "CAN_CONTROL::MECANUMCONTROL"))
(defconstant can_control::MecanumControl::*MODE_SET_TARGET* 5)

(defun can_control::MecanumControl-to-symbol (const)
  (cond
        ((= const 0) 'can_control::MecanumControl::*MODE_CALIB*)
        ((= const 1) 'can_control::MecanumControl::*MODE_IDLE*)
        ((= const 2) 'can_control::MecanumControl::*MODE_CLOSE_LOOP*)
        ((= const 3) 'can_control::MecanumControl::*MODE_CLEAR_ERROR*)
        ((= const 4) 'can_control::MecanumControl::*MODE_HOMING*)
        ((= const 5) 'can_control::MecanumControl::*MODE_SET_TARGET*)
        (t nil)))

(defclass can_control::MecanumControl
  :super ros::object
  :slots (_device_id _mode _value ))

(defmethod can_control::MecanumControl
  (:init
   (&key
    ((:device_id __device_id) 0)
    ((:mode __mode) 0)
    ((:value __value) 0.0)
    )
   (send-super :init)
   (setq _device_id (round __device_id))
   (setq _mode (round __mode))
   (setq _value (float __value))
   self)
  (:device_id
   (&optional __device_id)
   (if __device_id (setq _device_id __device_id)) _device_id)
  (:mode
   (&optional __mode)
   (if __mode (setq _mode __mode)) _mode)
  (:value
   (&optional __value)
   (if __value (setq _value __value)) _value)
  (:serialization-length
   ()
   (+
    ;; uint8 _device_id
    1
    ;; uint8 _mode
    1
    ;; float32 _value
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint8 _device_id
       (write-byte _device_id s)
     ;; uint8 _mode
       (write-byte _mode s)
     ;; float32 _value
       (sys::poke _value (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint8 _device_id
     (setq _device_id (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint8 _mode
     (setq _mode (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float32 _value
     (setq _value (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get can_control::MecanumControl :md5sum-) "cc8cc5d29cc98dd7c83c44e13887d465")
(setf (get can_control::MecanumControl :datatype-) "can_control/MecanumControl")
(setf (get can_control::MecanumControl :definition-)
      "uint8 MODE_CALIB       = 0
uint8 MODE_IDLE        = 1
uint8 MODE_CLOSE_LOOP  = 2
uint8 MODE_CLEAR_ERROR = 3
uint8 MODE_HOMING      = 4
uint8 MODE_SET_TARGET  = 5

uint8 device_id
uint8 mode
float32 value

")



(provide :can_control/MecanumControl "cc8cc5d29cc98dd7c83c44e13887d465")


