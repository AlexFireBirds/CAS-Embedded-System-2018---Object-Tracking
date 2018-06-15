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
	// Setup modules
	BallDetection ballDetector;
	double panAxisCorrection;
	double tiltAxisCorrection;
	int distanceBetweenCenterAndBall = 0;
	bool isTargetLocked = false;
	unsigned int readKey = 0;
	unsigned int numberOfExecutions = 0;
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
		std::cout<<"ERROR: Unable to open the camera\n\r";
	}

	// Get actual image (640 x 480)
	cap >> originalImage;

	// Setup BallTracker
	BallTracker balltracking(originalImage);
	balltracking.SetTargetWindowSize(27, 27);

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
		// Target window is only drawed when ball is detected
		balltracking.DrawTargetWindow(originalImage);

		circle( originalImage, Point(ballDetector.GetCoordinatesOfBall().x, ballDetector.GetCoordinatesOfBall().y), 2, Scalar(0,255,0), 3, LINE_AA);

		// Do a process cycle and get coordinates of the ball in this frame
		// Evaluate pan correction
		panAxisCorrection = 0;
		if (ballDetector.GetCoordinatesOfBall().x < balltracking.GetUpperLeftCornerOfTargetWindow().x)
		{
			distanceBetweenCenterAndBall =   abs(balltracking.GetCenterOfImage().x - ballDetector.GetCoordinatesOfBall().x);
			panAxisCorrection = 0.2752*exp(0.0121 * distanceBetweenCenterAndBall);
		}
		if (ballDetector.GetCoordinatesOfBall().x > balltracking.GetLowerRightCornerOfTargetWindow().x)
		{
			distanceBetweenCenterAndBall =   abs(balltracking.GetCenterOfImage().x - ballDetector.GetCoordinatesOfBall().x);
			panAxisCorrection = -(0.2752*exp(0.0121 * distanceBetweenCenterAndBall));
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

		// Is target locked and therefore corrections required?
		if(panAxisCorrection == 0 && tiltAxisCorrection == 0)
		{
			isTargetLocked = true;
		}
		else
		{
			isTargetLocked = false;

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
	}

	// Arm canon if target is locked
	if(isTargetLocked)
	{
		cv::putText(originalImage,
		            "Target locked",
		            cv::Point(5,15), 				// Coordinates
		            cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
		            1.0, 							// Scale. 2.0 = 2x bigger
		            cv::Scalar(0,0,255), 			// BGR colour
		            1, 								// Line Thickness
		            false);

		// Shoot if 's' is pressed
		if ((cv::waitKey(1) & 0xEFFFFF) == 115)
		{
			GPIO23.pulseOutput();
		}
	}

	// Show picture every fifth cycle
	if(numberOfExecutions % 5 == 0)
	{
		// Show image
		imshow("Processd image", originalImage);
	}

	// Read key every tenth cycle
	if(numberOfExecutions % 10 == 0)
	{
		// read key
		readKey = (cv::waitKey(1) & 0xEFFFFF);
		// 'esc' to quit endless loop
		if (readKey == 27)
		{
		 break;
		}
		// 'r' to move servos to init position
		else if(readKey == 114)
		{
			panServo.setAngle(0);
			tiltServo.setAngle(-10);
		}
		numberOfExecutions=0;
	}

	numberOfExecutions++;
  }

  // Shutdown
  std::cout<<"Closing the camera\n\r";
  cap.release();
  GPIO23.~GPIO();
  destroyAllWindows();

  return 0;
}


