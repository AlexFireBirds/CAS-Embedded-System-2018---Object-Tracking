/*
 * PWM.cpp
 *
 *  Created on: 1 Jun 2018
 *      Author: pi
 */

#include <fstream>
#include "string"
#include <iostream>
#include <sstream>

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include "PWM.hpp"

using namespace std;


PWM::PWM(unsigned char pwmNumb)
{
	int fd, len;
	char buf[50];
	this->dutyCycle = 0;
	this->frequency = 0;

	if(pwmNumb == 0){

		/*Export pwm0*/
		fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);

		if (fd < 0) {

			perror("pwm0/export");
		  }

		len = snprintf(buf, sizeof(buf), "%d", pwmNumb);
		write(fd, buf, len);
		close(fd);

		this->PWMchannel = 0;
	}
	if(pwmNumb == 1){

		/*Export pwm1*/
		fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);

		if (fd < 0) {

			perror("pwm1/export");
		  }

		len = snprintf(buf, sizeof(buf), "%d", pwmNumb);
		write(fd, buf, len);
		close(fd);

		this->PWMchannel = 1;
	}
}

PWM::PWM(unsigned char pwmNumb, double hz, double duty)
{
	int fd, len;
	char buf[50];

	if(pwmNumb == 0){

		/*Export pwm0*/
		fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);

		if (fd < 0) {

			perror("pwm0/export");
		  }

		len = snprintf(buf, sizeof(buf), "%d", pwmNumb);
		write(fd, buf, len);
		close(fd);

	    /*Set period*/
		fd = open("/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY);

		if (fd < 0) {

			perror("pwm0/period");
		  }

		len = snprintf(buf, sizeof(buf), "%d", (int)((1/hz) * 1e+9));
		write(fd, buf, len);
		close(fd);

	    this->frequency = hz;

	    /*Set duty cycle*/
		fd = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);

		if (fd < 0) {

			perror("pwm0/duty_cycle");
		  }

		len = snprintf(buf, sizeof(buf), "%d", (int)(((1/hz) * 1e+9) / (100 / duty)));
		write(fd, buf, len);
		close(fd);

	    this->dutyCycle = duty;

		this->PWMchannel = 0;
	}
	if(pwmNumb == 1){

		/*Export pwm1*/
		fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);

		if (fd < 0) {

			perror("pwm1/export");
		  }

		len = snprintf(buf, sizeof(buf), "%d", pwmNumb);
		write(fd, buf, len);
		close(fd);

	    /*Set period*/
		fd = open("/sys/class/pwm/pwmchip0/pwm1/period", O_WRONLY);

		if (fd < 0) {

			perror("pwm1/period");
		  }

		len = snprintf(buf, sizeof(buf), "%d", (int)((1/hz) * 1e+9));
		write(fd, buf, len);
		close(fd);

	    this->frequency = hz;

	    /*Set duty cycle*/
		fd = open("/sys/class/pwm/pwmchip0/pwm1/duty_cycle", O_WRONLY);

		if (fd < 0) {

			perror("pwm1/duty_cycle");
		  }

		len = snprintf(buf, sizeof(buf), "%d", (int)(((1/hz) * 1e+9) / (100 / duty)));
		write(fd, buf, len);
		close(fd);

	    this->dutyCycle = duty;

		this->PWMchannel = 1;
	}
}

PWM::~PWM(){

}

unsigned int PWM::setFrequency(double hz)
{
	int fd, len;
	char buf[50];

	this->frequency = hz;

	if(this->PWMchannel == 0){

	    /*Set frequency*/
		fd = open("/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY);

		if (fd < 0) {

			perror("pwm0/period");
			return -1;
		  }

		len = snprintf(buf, sizeof(buf), "%d", (int)((1/hz) * 1e+9));
		write(fd, buf, len);
		close(fd);
	    return 0;
	}
	if(this->PWMchannel == 1){

	    /*Set frequency*/
		fd = open("/sys/class/pwm/pwmchip0/pwm1/period", O_WRONLY);

		if (fd < 0) {

			perror("pwm1/period");
			return -1;
		  }

		len = snprintf(buf, sizeof(buf), "%d", (int)((1/hz) * 1e+9));
		write(fd, buf, len);
		close(fd);
	    return 0;
	}
	else{
		return -1;
	}

}

unsigned int PWM::setDutyCycle(double duty)
{
	int fd, len;
	char buf[50];

	this->dutyCycle = duty;

	if(this->PWMchannel == 0){

	    /*Set duty cycle*/
		fd = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);

		if (fd < 0) {

			perror("pwm0/duty_cycle");
			return -1;
		  }

		len = snprintf(buf, sizeof(buf), "%d", (int)(((1/this->frequency) * 1e+9) / (100 / duty)));
		write(fd, buf, len);
		close(fd);
	    return 0;
	}
	if(this->PWMchannel == 1){

	    /*Set duty cycle*/
		fd = open("/sys/class/pwm/pwmchip0/pwm1/duty_cycle", O_WRONLY);

		if (fd < 0) {

			perror("pwm1/duty_cycle");
			return -1;
		  }

		len = snprintf(buf, sizeof(buf), "%d", (int)(((1/this->frequency) * 1e+9) / (100 / duty)));
		write(fd, buf, len);
		close(fd);
	    return 0;
	}
	else{
		return -1;
	}

}

unsigned int PWM::enable()
{
	int fd, len;
	char buf[50];

	if(this->PWMchannel == 0){

	    /*Enable channel 0*/
		fd = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);

		if (fd < 0) {

			perror("pwm0/enable");
			return -1;
		  }

		len = snprintf(buf, sizeof(buf), "%d", ENABLE);
		write(fd, buf, len);
		close(fd);
	    return 0;
	}
	if(this->PWMchannel == 1){

	    /*Enable channel 1*/
		fd = open("/sys/class/pwm/pwmchip0/pwm1/enable", O_WRONLY);

		if (fd < 0) {

			perror("pwm1/enable");
			return -1;
		  }

		len = snprintf(buf, sizeof(buf), "%d", ENABLE);
		write(fd, buf, len);
		close(fd);
	    return 0;
	}
	else{
		return -1;
	}
}

unsigned int PWM::disable()
{
	int fd, len;
	char buf[50];

	if(this->PWMchannel == 0){

	    /*Enable channel 0*/
		fd = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);

		if (fd < 0) {

			perror("pwm0/Disable");
			return -1;
		  }

		len = snprintf(buf, sizeof(buf), "%d", DISABLE);
		write(fd, buf, len);
		close(fd);
	    return 0;
	}
	if(this->PWMchannel == 1){

	    /*Enable channel 1*/
		fd = open("/sys/class/pwm/pwmchip0/pwm1/enable", O_WRONLY);

		if (fd < 0) {

			perror("pwm1/disable");
			return -1;
		  }

		len = snprintf(buf, sizeof(buf), "%d", DISABLE);
		write(fd, buf, len);
		close(fd);
	    return 0;
	}
	else{
		return -1;
	}
}
