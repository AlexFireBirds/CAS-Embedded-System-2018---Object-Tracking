/*
 * Test.cpp
 *
 *  Created on: 2 Jun 2018
 *      Author: pi
 */

// Control_Test main

//#include <stdio.h>
#include <unistd.h>	// usleep()
//#include <fstream>
#include <stdio.h>	// printf()


#include "GPIO.hpp"

// using namespace cv;
// using namespace std;

int main(int argc, char** argv)
{
	int res;
	GPIO GPIO23(23);
	GPIO GPIO24(24);
	res = GPIO23.setup();
	res = GPIO24.setup();
	if(res != 0) {
		printf("Error: Can't setup GPIO-Pin");
		return -1;
	}

	for (int i=0; i<100; i++)
	{
		res = GPIO23.activate();
		res = GPIO24.activate();
		usleep(5000000);
		res = GPIO23.deactivate();
		res = GPIO24.deactivate();
		usleep(5000000);
	}

	printf("finished\n");
	return 0;
}

