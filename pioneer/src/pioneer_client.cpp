/*Author:NavatejaReddy*/
#include "ros/ros.h"
#include "pioneer/MovePioneer.h"
#include <geometry_msgs/Twist.h>
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pioneer_client"); 
  if(argc !=3)
  {
   ROS_INFO("usage: pioneer_client xpos zpos");
   return 1;
  }

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<pioneer::MovePioneer>("move_pioneer");
 
  pioneer::MovePioneer srv;
  srv.request.xpos = atoll(argv[1]);
  srv.request.zpos = atoll(argv[2]);
  
  if(client.call(srv))
  {
    ROS_INFO("%s",srv.response.result.c_str());
  }
  else
  {
    ROS_ERROR("failed to call the service pioneer_server ");
    return 1;
  }
 return 0;
}

