#include "ros/ros.h"
#include "pioneer/MovePioneer.h"
#include <geometry_msgs/Twist.h>
#include <std_msgs/Empty.h>
#include <unistd.h> //header for time delay function usleep()

#define LINEAR_VELOCITY 0.1 //sets linear velocity
#define RIGHT_ANGULAR_VELOCITY -0.1 //sets angular velocity
#define LEFT_ANGULAR_VELOCITY 0.1 //sets angular velocity
#define ANGLE_IN_DEGREES 1.5708 //defines the angle(in degrees) to turn the pioneer
#define SYSTAMATIC_ERROR 1.03  //to correct error of distance covered with defined linear velocity by multiplying with temptime measued during runtime 

ros::Publisher pub;
ros::Publisher arduino_pub;


bool move(pioneer::MovePioneer::Request &req,pioneer::MovePioneer::Response &res)
{
 float x_position = req.xpos;
 float z_position = req.zpos;
 double temptime; 

 geometry_msgs::Twist msg;
 std_msgs::Empty arduino_msg;
 std::cerr << "Attempting to move: "; 

/*-------------------------object is on right side of robot----------------------------------*/
if(x_position>0)
{
 std::cerr << "X > 0" << std::endl;
 
 //step 1 : turn 90 towards right
 msg.angular.z = RIGHT_ANGULAR_VELOCITY ; //setting the pioneer angular velocity
 pub.publish(msg); //publishing the msg
 usleep(abs(ANGLE_IN_DEGREES/RIGHT_ANGULAR_VELOCITY)*1000000); //calculated time to turn given degrees with given angular velocity
 msg.angular.z = 0 ; //setting the pioneer angular velocity to 0 i.e stopping
 pub.publish(msg);
 
 usleep(500000); //0.5 sec delay

 //step 2 : move along x_position
 msg.linear.x = LINEAR_VELOCITY ; //setting the pioneer linear velocity
 pub.publish(msg); 
 temptime = x_position/LINEAR_VELOCITY ; //calculating the time to travel the x_position by pioneer base
 usleep(abs(temptime*SYSTAMATIC_ERROR*1000000)); //dealy to allow pioneer to travel x_position
 msg.linear.x = 0; //setting the pioneer linear velocity to 0 i.e stopping
 pub.publish(msg); 
 
 usleep(500000); //0.5 sec delay
 
 //step 3 : turn 90 towards left
 msg.angular.z = LEFT_ANGULAR_VELOCITY ;
 pub.publish(msg);
 usleep(abs(ANGLE_IN_DEGREES/LEFT_ANGULAR_VELOCITY)*1000000);
 msg.angular.z = 0 ;
 pub.publish(msg);
 
 usleep(500000); //0.5 sec delay

 //step 4 : move along z_position
 msg.linear.x = LINEAR_VELOCITY ;
 pub.publish(msg);
 temptime = z_position/LINEAR_VELOCITY;
 usleep(abs(temptime*SYSTAMATIC_ERROR*1000000));
 msg.linear.x = 0;
 pub.publish(msg);
}

/*-------------------------object is on left side of robot----------------------------------*/
else if(x_position < 0)
{
 std::cerr << "X < 0 " << std::endl;
  
 //step 1 : turn 90 towards left
 msg.angular.z = LEFT_ANGULAR_VELOCITY ;
 pub.publish(msg);
 usleep(abs(ANGLE_IN_DEGREES/LEFT_ANGULAR_VELOCITY)*1000000);
 msg.angular.z = 0 ;
 pub.publish(msg);
 
 usleep(500000); //0.5 sec delay
 
 //step 2 : move along x_position
 msg.linear.x = LINEAR_VELOCITY ; 
 pub.publish(msg); 
 temptime = x_position/LINEAR_VELOCITY ; 
 usleep(abs(temptime*SYSTAMATIC_ERROR*1000000)); 
 msg.linear.x = 0; 
 pub.publish(msg); 
 
 usleep(500000); //0.5 sec delay

 //step 3 : turn 90 towards right
 msg.angular.z = RIGHT_ANGULAR_VELOCITY ;
 pub.publish(msg);
 usleep(abs(ANGLE_IN_DEGREES/RIGHT_ANGULAR_VELOCITY)*1000000);
 msg.angular.z = 0 ;
 pub.publish(msg);
 
 usleep(500000); //0.5 sec delay

 //step 4 : move along z_position
 msg.linear.x = LINEAR_VELOCITY ;
 pub.publish(msg);
 temptime = z_position/LINEAR_VELOCITY;
 usleep(abs(temptime*SYSTAMATIC_ERROR*1000000));
 msg.linear.x = 0;
 pub.publish(msg);
}

/*-------------------------object is facing stright to the robot----------------------------------*/
else
{
 std::cerr << "X == 0" << std::endl;
 msg.linear.x = LINEAR_VELOCITY ;
 pub.publish(msg);
 temptime = z_position/LINEAR_VELOCITY;
 usleep(abs(temptime*SYSTAMATIC_ERROR*1000000));
 msg.linear.x = 0;
 pub.publish(msg);
}

 res.result = "Success! Moved the pioneer base to the object,now running Arm Sequence";
 arduino_pub.publish(arduino_msg);
 return true;
}

int main(int argc, char **argv)
{
 ros::init(argc, argv, "pioneer_server");
 ros::NodeHandle n; 
 pub = n.advertise<geometry_msgs::Twist>("/RosAria/cmd_vel",100); // "packagename*/cmd_vel" topic with message type "geometry_msgs/twist"|subscribing topic
 arduino_pub=n.advertise<std_msgs::Empty>("run_arm_seq",100); //subscribing to arduino node
 ros::ServiceServer service = n.advertiseService("move_pioneer",move);
 ROS_INFO("Ready to move pioneer base, Give xpos and zpos of the detected object");
 ros::spin();
 
 return 0;
}
