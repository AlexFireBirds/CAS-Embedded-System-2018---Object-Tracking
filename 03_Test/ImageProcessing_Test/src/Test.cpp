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
#include "ThresholdEvaluator.hpp"

using namespace cv;


int main( int argc, char** argv )
{
  ThresholdEvaluator thresholdEvalutaion;

  // Raspberry Pi Camera Test
  VideoCapture cap(0);
  if (!cap.isOpened())
  {
	  printf("ERROR: Unable to open the camera\n\r");
  }

  Mat originalImage;
  Mat hsvimage;

  printf("Start grabbing, press a key on Live window to terminate\n\r");

  // Evaluate Thresholds
  while(1)
  {
		// Get actual image
		cap >> originalImage;

		thresholdEvalutaion.ProcessImage(originalImage);
		thresholdEvalutaion.ShowProcessdImage();

	    // Check exit condition for endless loop
	    if (cv::waitKey(5)>=0)
	    {
	     break;
	    }
  }
  //thresholdEvalutaion.~ThresholdEvaluator();

  while(1)
  {
	// Get actual image
	cap >> originalImage;

    // Convert image from camera to HSV space
    cvtColor(originalImage, hsvimage, COLOR_BGR2HSV,0);
    //imshow("hsvimage",hsvimage);

    // Apply thresholds
    //inRange(hsvimage, Scalar(0, 0, 0), Scalar(139, 196, 243), hsvimage);
    inRange(hsvimage, Scalar(0, 140, 128), Scalar(73, 255, 250), hsvimage);
    //inRange(hsvimage, Scalar(thresholdEvalutaion.GetLow_H(), thresholdEvalutaion.GetLow_S(), thresholdEvalutaion.GetLow_V()), Scalar(thresholdEvalutaion.GetHigh_H(), thresholdEvalutaion.GetHigh_S(), thresholdEvalutaion.GetHigh_V()), hsvimage);
    imshow("hsvimage",hsvimage);

    // Smooth image
    GaussianBlur(hsvimage, hsvimage, Size(9,9), 0, 0);

    // Search for circles
    std::vector<Vec3f> circles;
    HoughCircles(hsvimage, circles, HOUGH_GRADIENT, 1, 50, 100, 30, 25, 100);

    // Highlight detected circles
    Mat cimg = originalImage;
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        circle( cimg, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA);
        circle( cimg, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, LINE_AA);
    }
    imshow("detected circles", cimg);

    // Check exit condition for endless loop
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


