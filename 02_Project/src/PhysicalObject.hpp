/*
 * PhysicalObject.hpp
 *
 *  Created on: 20 Jun 2018
 *      Author: pi
 */

#ifndef SRC_PHYSICALOBJECT_HPP_
#define SRC_PHYSICALOBJECT_HPP_


class PhysicalObject
{
public:
	PhysicalObject(float new_x, float new_y, float new_z);
	~PhysicalObject();
	void update_position(float new_x, float new_y, float new_z, unsigned time_diff);
	void reset_forecast();
	void forecast_position(float &next_x, float &next_y, float &next_z, unsigned diff_ticks, bool gravity);
//	void forecast_positions(unsigned num, unsigned diff_ticks);
//	void get_forecasted_position(unsigned num, float &f_x, float &f_y, float &f_z);
//	bool forecast_y_collision(unsigned &collision_ticks);
	const float radius = 0.02;
	bool activ;
	// aktuelle pos
	float x;
	float y;
	float z;

private:
	// velocity
	float v_x;
	float v_y;
	float v_z;
	float last_x;
	float last_y;
	float last_z;
	//forecast
	float f_x;
	float f_y;
	float f_z;
	float f_v_x;
	float f_v_y;
	float f_v_z;
};


#endif /* SRC_PHYSICALOBJECT_HPP_ */
