CountessQunata Robot Control with ROS

pioneer  = Moving pioneer base based on depth(z) and horizontal distance(x). Implemeted using ROSARIA package. Copy complete folder to catkin workspace and  run catkin_make to build the package.

motor_calibrator = Arduino code to calibrate the motors in shoulder and upper arm

servo_calibrator = Arduino code to calibrate the servos in the lower arm

i2cmotor_servo_contoller_ros = ROS node subscribed to the topic "run_arm_seq". Any node which publishes empty message on topic "run_arm_seq" runs the arm sequence of picking up the object


![Alt text](https://github.com/ntej/CountessQuanta-ROS/blob/master/Images/rosgraph.png "Countess Qunata")

![Alt text](https://github.com/ntej/CountessQuanta-ROS/blob/master/Images/countess_quanta.jpeg "Countess Qunata")

