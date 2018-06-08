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
	BallDetection ballDetector;
	double panAxisCorrection;
	double tiltAxisCorrection;
	int distanceBetweenCenterAndBall = 0;
	cv::Mat originalImage;

	Servo panServo(Servo::PAN);
	panServo.enable();
	Servo tiltServo(Servo::TILT);
	tiltServo.enable();

	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		printf("ERROR: Unable to open the camera\n\r");
	}

	// Get actual image (640 x 480)
	cap >> originalImage;


	// Business logic test
	BallTracker balltracking(originalImage);
	balltracking.SetTargetWindowSize(100, 70);

	ballDetector.SetLow_H(0);
	ballDetector.SetLow_S(0);
	ballDetector.SetLow_V(175);

	ballDetector.SetHigh_H(28);
	ballDetector.SetHigh_S(186);
	ballDetector.SetHigh_V(238);


	// Init servos
	panServo.setAngle(0);
	tiltServo.setAngle(-8);
	usleep(3000000);


  while(1)
  {
	// Get actual image
	cap >> originalImage;

	ballDetector.ExecuteDetecionCycle(originalImage);

	if(ballDetector.NumberOfDetectedBalls() >= 1)
	{
		balltracking.DrawTargetWindow(originalImage);

		circle( originalImage, Point(ballDetector.GetCoordinatesOfBall().x, ballDetector.GetCoordinatesOfBall().y), 15, Scalar(0,0,255), 3, LINE_AA);
		circle( originalImage, Point(ballDetector.GetCoordinatesOfBall().x, ballDetector.GetCoordinatesOfBall().y), 2, Scalar(0,255,0), 3, LINE_AA);

		imshow("Processd image", originalImage);


		// Do a process cycle and get coordinates of the ball in this frame
		// Evaluate pan correction
		panAxisCorrection = 0;
		if (ballDetector.GetCoordinatesOfBall().x < balltracking.GetUpperLeftCornerOfTargetWindow().x)
		{
			distanceBetweenCenterAndBall =   abs(balltracking.GetCenterOfImage().x - ballDetector.GetCoordinatesOfBall().x);
			panAxisCorrection = 0.4124*exp(0.0102 * distanceBetweenCenterAndBall);
		}
		if (ballDetector.GetCoordinatesOfBall().x > balltracking.GetLowerRightCornerOfTargetWindow().x)
		{
			distanceBetweenCenterAndBall =   abs(balltracking.GetCenterOfImage().x - ballDetector.GetCoordinatesOfBall().x);
			panAxisCorrection = -(0.4124*exp(0.0102 * distanceBetweenCenterAndBall));
		}


		// Evaluate tilt correction
		tiltAxisCorrection = 0;
		if (ballDetector.GetCoordinatesOfBall().y > balltracking.GetLowerRightCornerOfTargetWindow().y)
		{
			distanceBetweenCenterAndBall =   abs(balltracking.GetCenterOfImage().y - ballDetector.GetCoordinatesOfBall().y);
			tiltAxisCorrection = 0.2572*exp(0.0139 * distanceBetweenCenterAndBall);
		}
		if (ballDetector.GetCoordinatesOfBall().y < balltracking.GetUpperLeftCornerOfTargetWindow().y)
		{
			distanceBetweenCenterAndBall =   abs(balltracking.GetCenterOfImage().y - ballDetector.GetCoordinatesOfBall().y);
			tiltAxisCorrection = -(0.2572*exp(0.0139 * distanceBetweenCenterAndBall));
		}


		// Check movement range of pan axis
		panAxisCorrection = panServo.getAngle() + panAxisCorrection;
		if(panAxisCorrection > 90)
		{
			panAxisCorrection = 90;
		}
		if(panAxisCorrection < -90)
		{
			panAxisCorrection = -90;
		}

		// Check movement range of tilt axis
		tiltAxisCorrection = tiltServo.getAngle() + tiltAxisCorrection;
		if(tiltAxisCorrection > 40)
		{
			tiltAxisCorrection = 40;
		}
		if(tiltAxisCorrection < -90)
		{
			tiltAxisCorrection = -90;
		}

		// Execute correction
		panServo.setAngle(panAxisCorrection);
		tiltServo.setAngle(tiltAxisCorrection);
	}
	else
	{
		imshow("Processd image", originalImage);
		balltracking.DrawTargetWindow(originalImage);
	}

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


