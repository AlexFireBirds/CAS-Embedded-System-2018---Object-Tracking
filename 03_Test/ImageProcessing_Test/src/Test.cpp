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
#include "BallDetection.hpp"
#include "BallTracker.hpp"
#include "Servo.hpp"
#include <unistd.h>


using namespace cv;

int main( int argc, char** argv )
{
  ThresholdEvaluator thresholdEvalutaion;
  BallDetection ballDetector;

	Servo panServo(Servo::PAN);
	panServo.enable();
	Servo tiltServo(Servo::TILT);
	tiltServo.enable();


  // Raspberry Pi Camera Test
  VideoCapture cap(0);
  if (!cap.isOpened())
  {
	  printf("ERROR: Unable to open the camera\n\r");
  }

  Mat originalImage;
  Mat hsvimage;

  // Evaluate Thresholds
  while(1)
  {
		// Get actual image (640 x 480)
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


  // Business logic test
  BallTracker balltracking(originalImage);

	// Init tour for servos
	// to default position;
/*	panServo.setAngle(-90);
	tiltServo.setAngle(-90);
	usleep(2000000);
	tiltServo.setAngle(90);
	usleep(2000000);
	panServo.setAngle(90);
	usleep(2000000);
	tiltServo.setAngle(-90);
	usleep(2000000);
	panServo.setAngle(45);
	usleep(1000000);
	tiltServo.setAngle(45);
*/


  double panAxisCorrection;
  double tiltAxisCorrection;

  while(1)
  {
	// Get actual image
	cap >> originalImage;

	balltracking.DrawTargetWindow(originalImage);

    imshow("Processd image", originalImage);

    // Do a process cycle and get coordinates of the ball in this frame
    // Evaluate pan correction
    if (ballDetector.GetCoordinatesOfBall().x < balltracking.GetupperLeftCornerOfTargetWindow().x)
    {
    	panAxisCorrection = -2;
    }
    else if (ballDetector.GetCoordinatesOfBall().x > balltracking.GetLowerRightCornerOfTargetWindow().x)
	{
    	panAxisCorrection = 2;
	}
    else
    {
    	panAxisCorrection = 0;
    }

    // Evaluate tilt correction
    if (ballDetector.GetCoordinatesOfBall().y > balltracking.GetupperLeftCornerOfTargetWindow().y)
    {
    	tiltAxisCorrection = 2;
    }
    else if (ballDetector.GetCoordinatesOfBall().y < balltracking.GetLowerRightCornerOfTargetWindow().y)
	{
    	tiltAxisCorrection = 2;
	}
    else
    {
    	tiltAxisCorrection = 0;
    }

    // Check movement range of pan axis
    //panAxisCorrection = panServo.getAngle() + panAxisCorrection;
    if(panAxisCorrection > 90)
    {
    	panAxisCorrection = 90;
    }
    if(panAxisCorrection < -90)
    {
    	panAxisCorrection = -90;
    }

    // Check movement range of tilt axis
    //tiltAxisCorrection = tiltServo.getAngle() + tiltAxisCorrection;
    if(tiltAxisCorrection > 90)
    {
    	tiltAxisCorrection = 90;
    }
    if(tiltAxisCorrection < -90)
    {
    	tiltAxisCorrection = -90;
    }

    // Execute correction
    panServo.setAngle(panAxisCorrection);
    tiltServo.setAngle(tiltAxisCorrection);

	// Check exit condition for endless loop
	if (cv::waitKey(5)>=0)
	{
	 break;
	}

  }


  while(1)
  {
		// Get actual image
		cap >> originalImage;

		ballDetector.ExecuteDetecionCycle(originalImage);

        circle( originalImage, Point(ballDetector.GetCoordinatesOfBall().x, ballDetector.GetCoordinatesOfBall().y), 15, Scalar(0,0,255), 3, LINE_AA);
        circle( originalImage, Point(ballDetector.GetCoordinatesOfBall().x, ballDetector.GetCoordinatesOfBall().y), 2, Scalar(0,255,0), 3, LINE_AA);
        imshow("Processd image", originalImage);

//        panServo.setAngle((ballDetector.GetCoordinatesOfBall().x) * 0.28125f - 90);
//		  tiltServo.setAngle((ballDetector.GetCoordinatesOfBall().y) * 0.375f - 90);

//		  printf("X: %d\r\nY: %d\r\n\r\n", ballDetector.GetCoordinatesOfBall().x, ballDetector.GetCoordinatesOfBall().y);

	    // Check exit condition for endless loop
	    if (cv::waitKey(5)>=0)
	    {
	     break;
	    }
  }


  while(1)
  {
	// Get actual image
	cap >> originalImage;

    // Convert image from camera to HSV space
    cvtColor(originalImage, hsvimage, COLOR_BGR2HSV,0);
    //imshow("hsvimage",hsvimage);

    // Apply thresholds
    inRange(hsvimage, Scalar(thresholdEvalutaion.GetLow_H(), thresholdEvalutaion.GetLow_S(), thresholdEvalutaion.GetLow_V()), Scalar(thresholdEvalutaion.GetHigh_H(), thresholdEvalutaion.GetHigh_S(), thresholdEvalutaion.GetHigh_V()), hsvimage);
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


