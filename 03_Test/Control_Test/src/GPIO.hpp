/*
 * GPIO.hpp
 *
 *  Created on: 2 Jun 2018
 *      Author: pi
 */

#ifndef SRC_GPIO_HPP_
#define SRC_GPIO_HPP_


class GPIO
{
public:
	GPIO(unsigned num);	// Constructor
	~GPIO();			// Destructor
	int setup();
	int activate();
	int deactivate();
private:
	unsigned num;
};


#endif /* SRC_GPIO_HPP_ */
