/*
 * PWM.hpp
 *
 *  Created on: 1 Jun 2018
 *      Author: pi
 */

#ifndef SRC_PWM_HPP_
#define SRC_PWM_HPP_

using namespace std;

class PWM
{
public:
    PWM(unsigned char pwmNumb);  // Constructor for a PWM object
    PWM(unsigned char pwmNumb, double hz, double duty);
    ~PWM(); // Destructor for a PWM object
    unsigned int setFrequency(double hz);
    unsigned int setDutyCycle(double duty);
    unsigned int enable();
    unsigned int disable();
private:
    static const char PWM0 = 0; //channel 0
    static const char PWM1 = 1; // channel 1
    static const char ENABLE = 1; // Enable
    static const char DISABLE = 0; // Disable
    unsigned char PWMchannel; //channel of this object
    double frequency; // PWM frequency
    double dutyCycle; //PWM duty Cycle (%)
};


#endif /* SRC_PWM_HPP_ */
