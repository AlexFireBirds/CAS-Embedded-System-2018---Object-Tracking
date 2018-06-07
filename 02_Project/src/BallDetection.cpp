/*
 * BallDetection.cpp
 *
 *  Created on: 2 Jun 2018
 *      Author: pi
 */

#include "BallDetection.hpp"

using namespace cv;

BallDetection::BallDetection()
:numberOfDetectedBalls(0), isDetectionRunning(false),
 low_H(0),
 low_S(140),
 low_V(128),
 high_H(73),
 high_S(255),
 high_V(250)
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
	inRange(processdImage, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), processdImage);

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

void BallDetection::SetLow_H(int value)
{
	low_H = value;
}

void BallDetection::SetLow_S(int value)
{
	low_S = value;
}

void BallDetection::SetLow_V(int value)
{
	low_V = value;
}

void BallDetection::SetHigh_H(int value)
{
	high_H = value;
}

void BallDetection::SetHigh_S(int value)
{
	high_S = value;
}

void BallDetection::SetHigh_V(int value)
{
	high_V = value;
}
