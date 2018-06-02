/*
 * ThresholdEvaluator.hpp
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
    void ShowImage(cv::Mat image);

private:

};



#endif /* SRC_THRESHOLDEVALUATOR_HPP_ */
