#include <stdlib.h>
#include <stdio.h>    /* Standard input/output definitions */
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <pthread.h>
#include <math.h>
#include <time.h>

//#include "serial.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "robota_data.h"
#include "robota_opencv.h"

#include "serialArduino.h"
#include "serialArduino.cpp"
#include "filtro.h"

//ROS
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <turtlesim/Velocity.h>
#include <geometry_msgs/Twist.h>
#include "/opt/ros/fuerte/stacks/vision_opencv/cv_bridge/include/cv_bridge/CvBridge.h"

VideoCapture cap(0);                /* declare video capture */
