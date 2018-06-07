/*
 * ThresholdEvaluator.hpp
 *
 * Use id like:
 * 		thresholdEvalutaion.ProcessImage(originalImage);
		thresholdEvalutaion.ShowProcessdImage();
 *
 *
 *
 *  Created on: 2 Jun 2018
 *      Author: pi
 */

#ifndef SRC_THRESHOLDEVALUATOR_HPP_
#define SRC_THRESHOLDEVALUATOR_HPP_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

class ThresholdEvaluator
{
public:
	ThresholdEvaluator(void);  		// Constructor
    ~ThresholdEvaluator(); 			// Destructor
    void ProcessImage(cv::Mat image);
    void ShowProcessdImage(void);
    // Getter for HSV threshold values
    int GetLow_H(void);
    int GetLow_S(void);
    int GetLow_V(void);
    int GetHigh_H(void);
    int GetHigh_S(void);
    int GetHigh_V(void);

private:
};



#endif /* SRC_THRESHOLDEVALUATOR_HPP_ */
