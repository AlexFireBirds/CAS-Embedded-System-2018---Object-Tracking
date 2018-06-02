/*
 * Test.cpp
 *
 *  Created on: 1 Jun 2018
 *      Author: pi
 */

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "PWM.hpp"

using namespace cv;
int main( int argc, char** argv )
{

	PWM myPWM0(0, 50, 7);

	PWM myPWM1(1, 1000, 20);

	myPWM0.enable();

	myPWM1.enable();

	waitKey(5000);

	myPWM1.setDutyCycle(80);

	waitKey(5000);

	myPWM1.setFrequency(20);

	waitKey(5000);

	myPWM1.disable();

	return 0;
}


