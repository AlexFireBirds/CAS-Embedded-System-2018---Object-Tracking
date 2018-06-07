/*
 * Servo.hpp
 *
 *  Created on: 1 Jun 2018
 *      Author: pi
 */

#ifndef SRC_SERVO_HPP_
#define SRC_SERVO_HPP_

#include "PWM.hpp"

class Servo
{
public:
    Servo(unsigned char Num);  // Constructor for a Servo object
    ~Servo(); // Destructor for a Servo object
    unsigned int setAngle(double deg);
    double getAngle(void);
    unsigned int enable(){return myPWM.enable();};
    unsigned int disable(){return myPWM.disable();};
    static const char PAN = 0; //channel 0
    static const char TILT = 1; // channel 1


private:
    static const int MINANGLE = -90;
    static const int MAXANGLE = 90;
    double angle; // Servo angle
    PWM myPWM;
};



#endif /* SRC_SERVO_HPP_ */
