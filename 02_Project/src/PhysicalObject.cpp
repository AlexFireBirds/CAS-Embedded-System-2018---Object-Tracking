/*
 * PhysicalObject.cpp
 *
 *  Created on: 20 Jun 2018
 *      Author: pi
 */

#include <math.h>
#include <iostream>

#include "PhysicalObject.hpp"


	float acceleration_of_graviy = 9.81; 	// erdbeschleunigung in m/s²
	float friction = 0.02; 					// reibungskoeffizient, verlust pro?
	float bounce_lost = 0.2;				// verlust beim Aufspringen
//	float blow_delay = 0.010;				// verzögerung bis zum Anblasen in ms
//	float blow_time = 0.050;				// wie lange angeblasen wird in ms (oder detektieren?)


// Constructor
PhysicalObject::PhysicalObject(float new_x, float new_y, float new_z) {
	activ = false;
	x = new_x;
	y = new_y;
	z = new_z;
	//last_ticks = clock();
}

// Destructor
PhysicalObject::~PhysicalObject(){
	// do nothing
}

void PhysicalObject::update_position(float new_x, float new_y, float new_z, unsigned time_diff) {
	last_x = x;
	last_y = y;
	last_z = z;
	x = new_x;
	y = new_y;
	z = new_z;
	v_x = (x - last_x) / ((time_diff*1.0) / 1000);
	v_y = (y - last_y) / ((time_diff*1.0) / 1000);
	v_z = (z - last_z) / ((time_diff*1.0) / 1000);
	if(last_x > 0.01) {
		activ = true;
	}
}

void PhysicalObject::reset_forecast() {
	f_x = x;
	f_y = y;
	f_z = z;
	f_v_x = v_x;
	f_v_y = v_y;
	f_v_z = v_z;
}

void PhysicalObject::forecast_position(float &next_x, float &next_y, float &next_z, unsigned diff_ticks, bool gravity) {
	float calc_time = diff_ticks *1.0 / 1000;
	next_x = f_x + f_v_x * calc_time;
	next_y = f_y + f_v_y * calc_time;
	if(gravity) {
		next_z = f_z + f_v_z * calc_time - 1/2 * acceleration_of_graviy * calc_time * calc_time ;
	} else {
		next_z = f_z + f_v_z * calc_time;
	}
	//  aufspringen
	if(next_z < 0) {
		next_z = -next_z + 2*radius;
		f_v_z = -f_v_z * (1-bounce_lost);
	}
	// position für nächste runde
	f_x = next_x;
	f_y = next_y;
	f_z = next_z;
	// geschwindigkeit für nächste runde
	f_v_x = f_v_x * (1-friction);
	f_v_y = f_v_y * (1-friction);
	f_v_z = f_v_z * (1-friction);
	return;
}

/*
bool PhysicalObject::forecast_y_collision(unsigned &collision_ticks) {
	// wenn in nächsten 100 ms y_ebene auf Höhe von 50 cm durchquert wird
	return false;
}
*/




