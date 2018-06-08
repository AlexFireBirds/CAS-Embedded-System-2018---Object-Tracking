/*
 * BallTracker.cc
 *
 *  Created on: 7 Jun 2018
 *      Author: pi
 */
#include "BallTracker.hpp"



BallTracker::BallTracker(cv::Mat image)
:targetWindowWidth(200),
 targetWindowHeight(135)
{
	imageHeight = image.size().height;
	imageWidth = image.size().width;

	centerOfImage.x = imageWidth/2;
	centerOfImage.y = imageHeight/2;
}

BallTracker::~BallTracker(){
}

void BallTracker::DrawTargetWindow(cv::Mat image)
{
	// Draw target window
	cv::rectangle(image, upperLeftCornerOfTargetWindow, lowerRightCornerOfTargetWindow, cv::Scalar(0, 140, 255),4, -1, 0);
}

void BallTracker::SetTargetWindowSize(unsigned int width, unsigned height)
{
	targetWindowHeight = height;
	targetWindowWidth = width;

	// Calculate corner of target window
	upperLeftCornerOfTargetWindow.x = centerOfImage.x - targetWindowWidth / 2;
	upperLeftCornerOfTargetWindow.y = centerOfImage.y - targetWindowHeight / 2;

	lowerRightCornerOfTargetWindow.x = centerOfImage.x + targetWindowWidth / 2;
	lowerRightCornerOfTargetWindow.y = centerOfImage.y + targetWindowHeight / 2;
}

cv::Point BallTracker::GetUpperLeftCornerOfTargetWindow(void)
{
	return upperLeftCornerOfTargetWindow;
}

cv::Point BallTracker::GetLowerRightCornerOfTargetWindow(void)
{
	return lowerRightCornerOfTargetWindow;
}

Coordinate BallTracker::GetCenterOfImage(void)
{
	return centerOfImage;
}
