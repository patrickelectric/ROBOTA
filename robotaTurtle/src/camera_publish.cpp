#include "../include/robotalib.h"    /* the include file */

using namespace cv;
using namespace std;

//=================================================================================================
//=================================================================================================
int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("camera/image", 1);
  sleep(3);
  printf("taking image camera");
  //VideoCapture cap(0); // open the default camera

  const unsigned int CAPTURE_WIDTH = 640;
  const unsigned int CAPTURE_HEIGHT = 480;
  const string& cvOutputWindowName = "CV Output";  

  cap.set(CV_CAP_PROP_FRAME_WIDTH,  CAPTURE_WIDTH);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAPTURE_HEIGHT);

  // check if we succeeded
  
  if ( !cap.isOpened() )
  {
    cout << "Could not open capture device\n";
    return -1;
  }
  

  Mat frame;

  ros::Rate loop_rate(100);

  sensor_msgs::ImagePtr msg;

  while (nh.ok())
  {
    cap >> frame; // get a new frame from camera

    IplImage frm = _IplImage(frame); //Unfortunately must convert to old format
    msg = sensor_msgs::CvBridge::cvToImgMsg(&frm, "bgr8");

    pub.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();
  }
}