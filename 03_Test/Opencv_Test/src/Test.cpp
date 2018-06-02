/*
 * Test.cpp
 *
 *  Created on: 1 Jun 2018
 *      Author: pi
 */

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>


using namespace cv;
int main( int argc, char** argv )
{
  Mat image;
  image = imread( argv[1], 1 );
  if( argc != 2 || !image.data )
    {
      printf( "No image data \n" );
      return -1;
    }
  namedWindow( "Display Image", WINDOW_AUTOSIZE );
  imshow( "Display Image", image );


  // Raspberry Pi Camera Test

  VideoCapture cap(0);
  if (!cap.isOpened())
  {
	  printf("ERROR: Unable to open the camera\n\r");
  }

  Mat frame;
  Mat hsvimage;
  Mat thresholded1;
  Mat thresholded2;
  Mat thresholded3;

  Mat channels[3];


  printf("Start grabbing, press a key on Live window to terminate\n\r");

  while(1)
  {
	// Get actual image
	cap >> frame;

    // Convert image to HSV space
    cvtColor(frame, hsvimage, COLOR_BGR2HSV,0);
    //imshow("hsvimage",hsvimage);

    // Split in 3 different images
    split(hsvimage, channels);
    imshow("hsvimage1",channels[0]);
    imshow("hsvimage2",channels[1]);
    imshow("hsvimage3",channels[2]);


    //inRange(channels[0], Scalar(low_H, low_S, low_V), Scalar(low_H, low_S, low_V),channels[0])



    // Check exit condition
    if (cv::waitKey(5)>=0)
    {
     break;
    }
  }

  printf("Closing the camera\n\r");
  cap.release();
  destroyAllWindows();


  waitKey(0);
  return 0;
}


