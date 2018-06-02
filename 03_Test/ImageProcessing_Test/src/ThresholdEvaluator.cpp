/*
 * ThresholdEvaluator.cpp
 *
 *  Created on: 2 Jun 2018
 *      Author: pi
 */

#include "ThresholdEvaluator.hpp"

using namespace cv;

const int max_value_H = 360/2;
const int max_value = 255;
const cv::String _windowName = "Object Detection";
int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;

cv::Mat processdImage;

// Callbacks for trackbars
static void on_low_H_thresh_trackbar(int, void *)
{
	low_H = min(high_H-1, low_H);
    setTrackbarPos("Low H", _windowName, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
    high_H = max(high_H, low_H+1);
    setTrackbarPos("High H", _windowName, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
    low_S = min(high_S-1, low_S);
    setTrackbarPos("Low S", _windowName, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
    high_S = max(high_S, low_S+1);
    setTrackbarPos("High S", _windowName, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
    low_V = min(high_V-1, low_V);
    setTrackbarPos("Low V", _windowName, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
    high_V = max(high_V, low_V+1);
    setTrackbarPos("High V", _windowName, high_V);
}


ThresholdEvaluator::ThresholdEvaluator(){

	namedWindow(_windowName);

    // Trackbars to set thresholds for HSV values
    createTrackbar("Low H", _windowName, &low_H, max_value_H, on_low_H_thresh_trackbar);
    createTrackbar("High H", _windowName, &high_H, max_value_H, on_high_H_thresh_trackbar);
    createTrackbar("Low S", _windowName, &low_S, max_value, on_low_S_thresh_trackbar);
    createTrackbar("High S", _windowName, &high_S, max_value, on_high_S_thresh_trackbar);
    createTrackbar("Low V", _windowName, &low_V, max_value, on_low_V_thresh_trackbar);
    createTrackbar("High V", _windowName, &high_V, max_value, on_high_V_thresh_trackbar);
}

ThresholdEvaluator::~ThresholdEvaluator(){
	destroyAllWindows();
}

void ThresholdEvaluator::ProcessImage(Mat image)
{
	// Convert image from camera to HSV space
    cvtColor(image, processdImage, COLOR_BGR2HSV,0);

    inRange(processdImage, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), processdImage);
}

void ThresholdEvaluator::ShowProcessdImage(void)
{
    imshow(_windowName, processdImage);
}

void ThresholdEvaluator::ShowImage(cv::Mat image)
{
	imshow(_windowName, image);
}


// Getter for HSV threshold values
int ThresholdEvaluator::GetLow_H(void)
{
	return low_H;
}

int ThresholdEvaluator::GetLow_S(void)
{
	return low_S;
}

int ThresholdEvaluator::GetLow_V(void)
{
	return low_V;
}

int ThresholdEvaluator::GetHigh_H(void)
{
	return high_H;
}

int ThresholdEvaluator::GetHigh_S(void)
{
	return high_S;
}

int ThresholdEvaluator::GetHigh_V(void)
{
	return high_V;
}






