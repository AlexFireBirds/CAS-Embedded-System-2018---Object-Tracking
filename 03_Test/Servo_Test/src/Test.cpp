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
#include "Servo.hpp"
#include <unistd.h>

using namespace cv;
int main( int argc, char** argv )
{

	Servo panServo(Servo::PAN);
	panServo.enable();
	Servo tiltServo(Servo::TILT);
	tiltServo.enable();

	panServo.setAngle(90);//Pan -90 to 90
	tiltServo.setAngle(45);//Tilt -90 to 45



//	for(int i = 0; i <= 90; i++){
//
//		panServo.setAngle(-i);
//		tiltServo.setAngle(i);
//		usleep(100000);
//	}

	return 0;
}


