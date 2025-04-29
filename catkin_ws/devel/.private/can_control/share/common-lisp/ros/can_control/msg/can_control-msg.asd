
(cl:in-package :asdf)

(defsystem "can_control-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "EncoderEstimates" :depends-on ("_package_EncoderEstimates"))
    (:file "_package_EncoderEstimates" :depends-on ("_package"))
    (:file "MecanumControl" :depends-on ("_package_MecanumControl"))
    (:file "_package_MecanumControl" :depends-on ("_package"))
  ))