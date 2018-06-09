/*
 * GPIO.cpp
 *
 *  Created on: 2 Jun 2018
 *      Author: pi
 */

#include <stdlib.h>	// system();
#include <stdio.h>	// printf()
#include <unistd.h>	// usleep()

#include <string>	// to_string

#include "GPIO.hpp"

using namespace std;

// Constructor
GPIO::GPIO(unsigned num)
: pulseTime(100000)
{
	//GPIO::GPIO::num = num;
	this->num = num;
	// echo "23" > /sys/class/gpio/export
	string s = "echo " + to_string(this->num) + " > /sys/class/gpio/export";
	system(s.c_str());
	//string res = do_console_command_get_result( const_cast<char *>(s.c_str()));
	//s = "result1 = " + res + "\n";
	//printf(s.c_str());
	usleep(300000); // braucht offenbar etwas zeit
}

// Destructor
GPIO::~GPIO()
{
	// echo "23" > /sys/class/gpio/unexport
	string s = "echo \"" + to_string(this->num) + "\" > /sys/class/gpio/unexport";
	system(s.c_str());
}

int GPIO::setup()
{
	// echo "out" > /sys/class/gpio/gpio23/direction
	string s = "echo \"out\" > /sys/class/gpio/gpio" + to_string(this->num) + "/direction";
	system(s.c_str());
	// echo "0" > /sys/class/gpio/gpio23/active_low
	s = "echo \"0\" > /sys/class/gpio/gpio" + to_string(this->num) + "/active_low";
	system(s.c_str());
	return 0;
}

int GPIO::activate()
{
	// echo "1" > /sys/class/gpio/gpio23/value
	string s = "echo \"1\" > /sys/class/gpio/gpio" + to_string(this->num) + "/value";
	system(s.c_str());
	return 0;
}


int GPIO::deactivate()
{
	// echo "0" > /sys/class/gpio/gpio23/value
	string s = "echo \"0\" > /sys/class/gpio/gpio" + to_string(this->num) + "/value";
	system(s.c_str());
	return 0;
}

// Set pulseTime in ms
void GPIO::setPulseTime(unsigned int value)
{
	pulseTime = value*1000;
}

void GPIO::pulseOutput(void)
{
	// echo "1" > /sys/class/gpio/gpio23/value
	string s = "echo \"1\" > /sys/class/gpio/gpio" + to_string(this->num) + "/value";
	system(s.c_str());

	// sleep with pulseTime [ms]
	usleep(pulseTime);

	// echo "0" > /sys/class/gpio/gpio23/value
	s = "echo \"0\" > /sys/class/gpio/gpio" + to_string(this->num) + "/value";
	system(s.c_str());
}





