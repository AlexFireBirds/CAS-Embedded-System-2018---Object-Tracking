/*
 * BallDetection.cpp
 *
 *  Created on: 2 Jun 2018
 *      Author: pi
 */

#include "BallDetection.hpp"

using namespace cv;

BallDetection::BallDetection()
:numberOfDetectedBalls(0), isDetectionRunning(false)
{

}

BallDetection::~BallDetection(){
}

void BallDetection::ExecuteDetecionCycle(cv::Mat image)
{
	cv::Mat processdImage;
	std::vector<Vec3f> circles;

	isDetectionRunning = true;

	// Convert image to HSV space
	cvtColor(image, processdImage, COLOR_BGR2HSV,0);

	// Apply threshold
	inRange(processdImage, Scalar(0, 140, 128), Scalar(73, 255, 250), processdImage);

	// Smooth image
	GaussianBlur(processdImage, processdImage, Size(9,9), 0, 0);

	// Search for circles
	HoughCircles(processdImage, circles, HOUGH_GRADIENT, 1, 50, 100, 30, 25, 100);

	// Update coordinates of detected ball
	numberOfDetectedBalls = circles.size();
	if(numberOfDetectedBalls >= 1)
	{
		coordinatesOfBall.x = circles[0][0];
		coordinatesOfBall.y = circles[0][1];
	}

	isDetectionRunning = false;
}

BallCoordinates BallDetection::GetCoordinatesOfBall(void)
{
	return coordinatesOfBall;
}

unsigned int BallDetection::NumberOfDetectedBalls(void)
{
	return numberOfDetectedBalls;
}

bool BallDetection::IsDetectionRunning(void)
{
	return isDetectionRunning;
}




