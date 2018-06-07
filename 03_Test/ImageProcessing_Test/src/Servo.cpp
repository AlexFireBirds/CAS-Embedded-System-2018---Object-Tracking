/*
 * Servo.cpp
 *
 *  Created on: 1 Jun 2018
 *      Author: pi
 */

//3.4% linker Anschlag 7.5% Mitte 11.9% Rechter Anschlag

#include <errno.h>
#include <stdio.h>
#include "Servo.hpp"


Servo::Servo(unsigned char Num) : myPWM(Num, 50, 7.5){

	angle = 0;
}

Servo::~Servo(){

}

unsigned int Servo::setAngle(double deg){

	/*Check if value is in range*/
	if(deg <= MAXANGLE && deg >= MINANGLE){

		/*Map deg to duty cycle*/
		if(this->myPWM.setDutyCycle((0.05 * deg + 7.5)) < 0){

			perror("setting Servo angle");
			return -1;
		}
		return 0;
	}

	return -1;
}

double Servo::getAngle(void){
	return angle;
}
