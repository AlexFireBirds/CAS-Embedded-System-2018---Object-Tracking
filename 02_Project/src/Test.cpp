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
#include <unistd.h>
#include <iostream>
#include <string>

#include "ThresholdEvaluator.hpp"
#include "BallDetection.hpp"
#include "BallTracker.hpp"
#include "Servo.hpp"
#include "GPIO.hpp"

using namespace cv;

int main( int argc, char** argv )
{
	// Setup of modules
	BallDetection ballDetector;
	double panAxisCorrection;
	double tiltAxisCorrection;
	int distanceBetweenCenterAndBall = 0;
	bool isTargetLocked = false;
	cv::Mat originalImage;

	// Setup servos
	Servo panServo(Servo::PAN);
	panServo.enable();
	Servo tiltServo(Servo::TILT);
	tiltServo.enable();

	// Setup GPIOs
	GPIO GPIO23(23);
	GPIO23.setup();
	GPIO23.setPulseTime(100);

	// Open camera
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		printf("ERROR: Unable to open the camera\n\r");
	}

	// Get actual image (640 x 480)
	cap >> originalImage;

	// Setup BallTracker
	BallTracker balltracking(originalImage);
	balltracking.SetTargetWindowSize(40, 40);

	// Set thresholds of ball detector
	ballDetector.SetLow_H(5);
	ballDetector.SetLow_S(94);
	ballDetector.SetLow_V(189);

	ballDetector.SetHigh_H(21);
	ballDetector.SetHigh_S(229);
	ballDetector.SetHigh_V(255);

	// Init servos
	panServo.setAngle(0);
	tiltServo.setAngle(-10);
	usleep(5000000);

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
			tiltAxisCorrection = 0.1049*exp(0.0172 * distanceBetweenCenterAndBall);
		}
		if (ballDetector.GetCoordinatesOfBall().y < balltracking.GetUpperLeftCornerOfTargetWindow().y)
		{
			distanceBetweenCenterAndBall =   abs(balltracking.GetCenterOfImage().y - ballDetector.GetCoordinatesOfBall().y);
			tiltAxisCorrection = -(0.1049*exp(0.0172 * distanceBetweenCenterAndBall));
		}

		// Is target locked?
		if(panAxisCorrection == 0 && tiltAxisCorrection == 0)
		{
			isTargetLocked = true;
		}
		else
		{
			isTargetLocked = false;
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

//	// Is target locked?
//	if(isTargetLocked)
//	{
////		cv::putText(originalImage,
////		            "Target locked",
////		            cv::Point(20,5), // Coordinates
////		            cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
////		            1.0, // Scale. 2.0 = 2x bigger
////		            cv::Scalar(255,255,255), // BGR Color
////		            1, // Line Thickness
////		            2); // Anti-alias
//
//		imshow("Processd image", originalImage);
//
//		if (cv::waitKey(5)>=0)
//		{
//			GPIO23.pulseOutput();
//		}
//	}
//	else
//	{
////		cv::putText(originalImage,
////		            "             ",
////		            cv::Point(20,5), // Coordinates
////		            cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
////		            1.0, // Scale. 2.0 = 2x bigger
////		            cv::Scalar(255,255,255), // BGR Color
////		            1, // Line Thickness
////		            2); // Anti-alias
//
//		imshow("Processd image", originalImage);
//	}

	// Check exit condition for endless loop
	if (cv::waitKey(5)>=0)
	{
	 break;
	}

  }



  printf("Closing the camera\n\r");
  cap.release();
  destroyAllWindows();


  GPIO23.~GPIO();

  waitKey(0);
  return 0;
}


