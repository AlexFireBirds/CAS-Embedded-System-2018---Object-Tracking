/*
 * Coordinates.hpp
 *
 *  Created on: 16 Jun 2018
 *      Author: pi
 */

#ifndef SRC_COORDINATES_HPP_
#define SRC_COORDINATES_HPP_

class Coordinates
{
public:
	Coordinates(); 	// constructor
	~Coordinates();	// destructor
	void updateCamPos(float panAngle, float tiltAngle); // hier verdreht
	void updateGunPos(float extra_beta);
	void calcPicPosXY(int &pic_x, int &pic_y, float pos_x, float pos_y, float pos_z);
	void calcCoordinates(float &pos_x, float &pos_y, float &pos_z, int pic_x, int pic_y, float distance);
	float getObjectDistance(float real_size, float measured_size);
	float getTargetDistance(float distance_in_cs);
	float get_best_alpha(int pic_x, int pic_y);
	float get_best_beta(int pic_x, int pic_y);
	float alpha; 	// panAxis
	float beta;		// tiltAxis
//	float tilt_correction;
	float camera_position_x; // = (tower_side_arm_1 + tower_side_arm_2 * sin(beta / 180.0 * PI) + tower_side_arm_3 * cos(beta / 180.0 * PI)) * cos(alpha / 180.0 * PI);
	float camera_position_y; // = (tower_side_arm_1 + tower_side_arm_2 * sin(beta / 180.0 * PI) + tower_side_arm_3 * cos(beta / 180.0 * PI)) * sin(alpha / 180.0 * PI);
	float camera_position_z; // = tower_feed + tower_side_arm_2 * cos(beta / 180.0 * PI);
	float gun_position_x;
	float gun_position_y;
	float gun_position_z;

private:
	float camera_fieldview_horizontal; // = 62.2; // src: https://www.raspberrypi.org/documentation/hardware/camera/
	float camera_fieldview_vertical; // = 48.8;
	float tower_feed; // = 			0.041;	// Höhe Tilt drehung
	float tower_side_arm_1; // = 	0.028; 	// ausleger 1 (abstand zwischen drehachen)
	float tower_side_arm_2; // = 	0.010; 	// ausleger 2 (abstand zwischen zweiter drehachse und camera)
	float tower_side_arm_gun; // = 	0.048; 	// (abstand zwischen zweiter drehachse und gun)
	float tower_side_arm_3; //= 	0.028; 	// ausleger 3 (dicke der camera)
};


#endif /* SRC_COORDINATES_HPP_ */
