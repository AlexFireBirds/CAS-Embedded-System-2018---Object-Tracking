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
#include "PWM.hpp"

using namespace std;


PWM::PWM(unsigned char pwmNumb)
{
	this->dutyCycle = 0;
	this->frequency = 0;

	if(pwmNumb == 0){

		/*Export pwm0*/
		string export_str = "/sys/class/pwm/pwmchip0";
	    ofstream exportgpio(export_str.c_str()); // Open "export" file.

	    if (!exportgpio.is_open()){
	        cout << " OPERATION FAILED: Unable to export PWM0"<< endl;
	    }

	    exportgpio << this->PWM0 ; //write PWM number to export
	    exportgpio.close(); //close export file

		this->PWMchannel = 0;
	}
	if(pwmNumb == 1){

		/*Export pwm1*/
		string export_str = "/sys/class/pwm/pwmchip0";
	    ofstream exportgpio(export_str.c_str()); // Open "export" file.

	    if (!exportgpio.is_open()){
	        cout << " OPERATION FAILED: Unable to export PWM1"<< endl;
	    }

	    exportgpio << this->PWM1 ; //write PWM number to export
	    exportgpio.close(); //close export file

		this->PWMchannel = 1;
	}
}

PWM::PWM(unsigned char pwmNumb, double hz, double duty)
{
	if(pwmNumb == 0){

		/*Export pwm0*/
		string export_str = "/sys/class/pwm/pwmchip0";
	    ofstream exportgpio(export_str.c_str()); // Open "export" file.

	    if (!exportgpio.is_open()){
	        cout << " OPERATION FAILED: Unable to export PWM0"<< endl;
	    }

	    exportgpio << this->PWM0 ; //write PWM number to export
	    exportgpio.close(); //close export file

	    /*Set period*/
	    string setper_str = "/sys/class/pwm/pwmchip0/pwm0/period";
	    ofstream setvalper(setper_str.c_str());
	    if (!setvalper.is_open()){
	    	cout << " OPERATION FAILED: Unable to write periode"<< endl;
	    }
	    setvalper << (1/hz) * 1e+9 ;
	    setvalper.close();
	    this->frequency = hz;

	    /*Set duty cycle*/
	    string setduty_str = "/sys/class/pwm/pwmchip0/pwm0/duty_cycle";
	    ofstream setvalduty(setduty_str.c_str());
	    if (!setvalduty.is_open()){
	    	cout << " OPERATION FAILED: Unable to write duty_cycle"<< endl;
	    }
	    setvalduty << ((1/hz) * 1e+9) / (100 / duty) ;
	    setvalduty.close();
	    this->dutyCycle = duty;

		this->PWMchannel = 0;
	}
	if(pwmNumb == 1){

		/*Export pwm1*/
		string export_str = "/sys/class/pwm/pwmchip0";
	    ofstream exportgpio(export_str.c_str()); // Open "export" file.

	    if (!exportgpio.is_open()){
	        cout << " OPERATION FAILED: Unable to export PWM1"<< endl;
	    }

	    exportgpio << this->PWM1 ; //write PWM number to export
	    exportgpio.close(); //close export file

	    /*Set period*/
	    string setper_str = "/sys/class/pwm/pwmchip0/pwm1/period";
	    ofstream setvalper(setper_str.c_str());
	    if (!setvalper.is_open()){
	    	cout << " OPERATION FAILED: Unable to write periode"<< endl;
	    }
	    setvalper << (1/hz) * 1e+9 ;
	    setvalper.close();
	    this->frequency = hz;

	    /*Set duty cycle*/
	    string setduty_str = "/sys/class/pwm/pwmchip0/pwm1/duty_cycle";
	    ofstream setvalduty(setduty_str.c_str());
	    if (!setvalduty.is_open()){
	    	cout << " OPERATION FAILED: Unable to write duty_cycle"<< endl;
	    }
	    setvalduty << ((1/hz) * 1e+9) / (100 / duty) ;
	    setvalduty.close();
	    this->dutyCycle = duty;

		this->PWMchannel = 1;
	}
}

unsigned int PWM::setFrequency(double hz)
{
	this->frequency = hz;

	if(this->PWMchannel == 0){

	    /*Set frequency*/
	    string setper_str = "/sys/class/pwm/pwmchip0/pwm0/period";
	    ofstream setvalper(setper_str.c_str());
	    if (!setvalper.is_open()){
	    	cout << " OPERATION FAILED: Unable to write periode"<< endl;
	    	return -1;
	    }
	    setvalper << ((1/hz) * 1e+9);
	    setvalper.close();
	    return 0;
	}
	if(this->PWMchannel == 1){

	    /*Set frequency*/
	    string setper_str = "/sys/class/pwm/pwmchip0/pwm1/period";
	    ofstream setvalper(setper_str.c_str());
	    if (!setvalper.is_open()){
	    	cout << " OPERATION FAILED: Unable to write periode"<< endl;
	    	return -1;
	    }
	    setvalper << ((1/hz) * 1e+9);
	    setvalper.close();
	    return 0;
	}
	else{
		return -1;
	}

}

unsigned int PWM::setDutyCycle(double duty)
{
	this->dutyCycle = duty;

	if(this->PWMchannel == 0){

	    /*Set frequency*/
	    string setduty_str = "/sys/class/pwm/pwmchip0/pwm0/duty_cycle";
	    ofstream setvalduty(setduty_str.c_str());
	    if (!setvalduty.is_open()){
	    	cout << " OPERATION FAILED: Unable to write duty cycle"<< endl;
	    	return -1;
	    }
	    setvalduty << ((1/this->frequency) * 1e+9) / (100 / duty);
	    setvalduty.close();
	    return 0;
	}
	if(this->PWMchannel == 1){

	    /*Set frequency*/
	    string setduty_str = "/sys/class/pwm/pwmchip0/pwm1/duty_cycle";
	    ofstream setvalduty(setduty_str.c_str());
	    if (!setvalduty.is_open()){
	    	cout << " OPERATION FAILED: Unable to write duty cycle"<< endl;
	    	return -1;
	    }
	    setvalduty << ((1/this->frequency) * 1e+9) / (100 / duty);
	    setvalduty.close();
	    return 0;
	}
	else{
		return -1;
	}

}

unsigned int PWM::enable()
{
	if(this->PWMchannel == 0){

	    /*Enable channel 0*/
	    string setEn_str = "/sys/class/pwm/pwmchip0/pwm0/enable";
	    ofstream setvalEn(setEn_str.c_str());
	    if (!setvalEn.is_open()){
	    	cout << " OPERATION FAILED: Unable to enable"<< endl;
	    	return -1;
	    }
	    setvalEn << ENABLE;
	    setvalEn.close();
	    return 0;
	}
	if(this->PWMchannel == 1){

	    /*Enable channel 0*/
	    string setEn_str = "/sys/class/pwm/pwmchip0/pwm1/enable";
	    ofstream setvalEn(setEn_str.c_str());
	    if (!setvalEn.is_open()){
	    	cout << " OPERATION FAILED: Unable to enable"<< endl;
	    	return -1;
	    }
	    setvalEn << ENABLE;
	    setvalEn.close();
	    return 0;
	}
	else{
		return -1;
	}

}

unsigned int PWM::disable()
{
	if(this->PWMchannel == 0){

	    /*Enable channel 0*/
	    string setEn_str = "/sys/class/pwm/pwmchip0/pwm0/enable";
	    ofstream setvalEn(setEn_str.c_str());
	    if (!setvalEn.is_open()){
	    	cout << " OPERATION FAILED: Unable to enable"<< endl;
	    	return -1;
	    }
	    setvalEn << DISABLE;
	    setvalEn.close();
	    return 0;
	}
	if(this->PWMchannel == 1){

	    /*Enable channel 0*/
	    string setEn_str = "/sys/class/pwm/pwmchip0/pwm1/enable";
	    ofstream setvalEn(setEn_str.c_str());
	    if (!setvalEn.is_open()){
	    	cout << " OPERATION FAILED: Unable to enable"<< endl;
	    	return -1;
	    }
	    setvalEn << DISABLE;
	    setvalEn.close();
	    return 0;
	}
	else{
		return -1;
	}

}
