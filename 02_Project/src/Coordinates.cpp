

// Koordinatensystem

#include <math.h>

#include "Coordinates.hpp"

#define PI 3.14159265
#define IMAGE_HEIGHT 480
#define IMAGE_WIDTH 640


// Constructor
Coordinates::Coordinates(){
	camera_fieldview_horizontal = 62.2; // src: https://www.raspberrypi.org/documentation/hardware/camera/
	camera_fieldview_vertical = 48.8;

	// config
	tower_feed = 			0.053;	// Höhe Tilt drehung (jetzt mit Holzplatte)
	tower_side_arm_1 = 		0.028; 	// ausleger 1 (abstand zwischen drehachen)
	tower_side_arm_2 = 		0.010; 	// ausleger 2 (abstand zwischen zweiter drehachse und camera)
	tower_side_arm_gun = 	0.048; 	// (abstand zwischen zweiter drehachse und gun)
	tower_side_arm_3 = 		0.028; 	// ausleger 3 (dicke der camera)
}


// Destructor
Coordinates::~Coordinates(){
	// do nothing
}


void Coordinates::updateCamPos(float panAngle, float tiltAngle) {
	alpha = panAngle;	// zweimal verdreht
	beta = -tiltAngle; //-tilt_correction;	// einmal verdreht, korrigierter Wert wird übergeben
	camera_position_x = (tower_side_arm_1 + tower_side_arm_2 * sin(beta / 180.0 * PI) + tower_side_arm_3 * cos(beta / 180.0 * PI)) * cos(alpha / 180.0 * PI);
	camera_position_y = (tower_side_arm_1 + tower_side_arm_2 * sin(beta / 180.0 * PI) + tower_side_arm_3 * cos(beta / 180.0 * PI)) * sin(alpha / 180.0 * PI);
	camera_position_z = tower_feed + tower_side_arm_2 * cos(beta / 180.0 * PI);
}


void Coordinates::updateGunPos(float extra_beta) {
	gun_position_x = (tower_side_arm_1 + tower_side_arm_gun * sin((beta+extra_beta) / 180.0 * PI) + tower_side_arm_3 * cos((beta+extra_beta) / 180.0 * PI)) * cos(alpha / 180.0 * PI);
	gun_position_y = (tower_side_arm_1 + tower_side_arm_gun * sin((beta+extra_beta) / 180.0 * PI) + tower_side_arm_3 * cos((beta+extra_beta) / 180.0 * PI)) * sin(alpha / 180.0 * PI);
	gun_position_z = tower_feed + tower_side_arm_gun * cos((beta+extra_beta) / 180.0 * PI);
}


// berechne aus den raum-koordinaten die bildposition
void Coordinates::calcPicPosXY(int &pic_x, int &pic_y, float pos_x, float pos_y, float pos_z) {
	// was sieht camera ?
/*	float links 	= alpha + camera_fieldview_horizontal / 2;
	float rechts 	= alpha - camera_fieldview_horizontal / 2;
	float oben 		= beta + camera_fieldview_vertical / 2;
	float unten 	= beta - camera_fieldview_vertical / 2;
*/
	float links 	= + camera_fieldview_horizontal / 2;
	float rechts 	= - camera_fieldview_horizontal / 2;
	float oben 		= + camera_fieldview_vertical / 2;
	float unten 	= - camera_fieldview_vertical / 2;

	// position von Objekt relativ zur Kamera
/*	pos_x = (pos_x - camera_position_x);// / cos(alpha / 180.0 * PI);
	pos_y = (pos_y - camera_position_y);// / sin(alpha / 180.0 * PI);
	pos_z = (pos_z - camera_position_z);// / cos(beta / 180.0 * PI);
*/

	// jetzt mit koordinaten transformation
	pos_x = (pos_x - camera_position_x); // * cos(alpha / 180.0 * PI) + (pos_y - camera_position_y)* sin(alpha / 180.0 * PI) ;
	pos_y = (pos_y - camera_position_y); // / sin(alpha / 180.0 * PI);
	pos_z = (pos_z - camera_position_z); // / cos(beta / 180.0 * PI);

	// drehen um alpha
	float pos_xs = pos_x * cos(alpha / 180.0 * PI) + pos_y * sin(alpha / 180.0 * PI);
	float pos_ys = -pos_x * sin(alpha / 180.0 * PI) + pos_y * cos(alpha / 180.0 * PI);
	float pos_zs = pos_z;

	// drehen um beta
	float pos_xss = pos_xs * cos(beta / 180.0 * PI) + pos_zs * sin(beta / 180.0 * PI);
	float pos_yss = pos_ys;
	float pos_zss = pos_xs * -sin(beta / 180.0 * PI) + pos_zs * cos(beta / 180.0 * PI);

	// um division durch 0 zu verhindern ...
	// x-Werte sind immer positiv
	if(pos_xss <= 0) {
		pos_xss = 0.001;
	}

	// alpha und beta vom Punkt aus sicht der Kamera
	float object_alpha = atan(pos_yss / pos_xss) * 180.0 / PI; // + alpha;
	float object_beta = atan(pos_zss / pos_xss) * 180.0 / PI; // + beta;
//	float object_alpha = atan(pos_y / pos_x) * 180.0 / PI;
//	float object_beta = atan(pos_z / sqrt(pos_x*pos_x + pos_y*pos_y)) * 180.0 / PI;

	// korrektur ?

/*
	float temp_local_alpha = tan(local_alpha / 180.0 * PI);
	if(pos_y < 0 && pos_x <0) {
		local_alpha = local_alpha - 180;
		temp_local_alpha = -tan(local_alpha / 180.0 * PI);
	}

	float temp_local_beta = tan(local_beta / 180.0 * PI);
	if(pos_z < 0 && pos_x <0) {
		local_beta = local_beta - 180;
		temp_local_beta = -tan(local_beta / 180.0 * PI);
	}


	pic_x = IMAGE_WIDTH * (temp_local_alpha-tan(links / 180.0 * PI)) / (tan(rechts / 180.0 * PI) - tan(links / 180.0 * PI));
	pic_y = IMAGE_HEIGHT * (temp_local_beta-tan(oben / 180.0 * PI)) / (tan(unten / 180.0 * PI) - tan(oben / 180.0 * PI));
*/

	// bildpunkte beginnen jeweils von links und von oben ...
/*	pic_x = IMAGE_WIDTH * (tan(object_alpha / 180.0 * PI)-tan(links / 180.0 * PI)) / (tan(rechts / 180.0 * PI)-tan(links / 180.0 * PI));
	pic_y = IMAGE_HEIGHT * (tan(object_beta / 180.0 * PI)-tan(oben / 180.0 * PI)) / (tan(unten / 180.0 * PI)-tan(oben / 180.0 * PI));
*/
	pic_x = IMAGE_WIDTH * (tan(object_alpha / 180.0 * PI)-tan(links / 180.0 * PI)) / (tan(rechts / 180.0 * PI)-tan(links / 180.0 * PI));
	pic_y = IMAGE_HEIGHT * (tan(object_beta / 180.0 * PI)-tan(oben / 180.0 * PI)) / (tan(unten / 180.0 * PI)-tan(oben / 180.0 * PI));
	// pic_y = IMAGE_HEIGHT/2 + IMAGE_HEIGHT/2 * tan(object_beta / 180.0 * PI) / tan(camera_fieldview_vertical / 2); // FALSCH ...
	return;
}


// berechne aus der bildposition die raumkoordinaten
void Coordinates::calcCoordinates(float &pos_x, float &pos_y, float &pos_z, int pic_x, int pic_y, float distance) {
	// ballgrösse gegeben (dann brauche ich aber noch den bestimmten durchmesser)
/*
	// Winkel unter welchem Ballradius zu sehen ist
	float angle = camera_fieldview_horizontal * measured_radius / IMAGE_WIDTH;

	// distanz zum ball
	float distance = ball_radius / sin(angle / 180.0 * PI);
*/
	// unter welchem exakten winkel ist der ball zu sehen
	// (hier kann mit reinen Winkeln gerechnet werden, ohne sin, tan und so, da nicht auf fläche projuziert)
	float corrected_alpha = alpha - camera_fieldview_horizontal/2 * (pic_x-(IMAGE_WIDTH/2)) / (IMAGE_WIDTH/2);
	float corrected_beta = beta - camera_fieldview_vertical/2 * (pic_y-(IMAGE_HEIGHT/2)) / (IMAGE_HEIGHT/2);

	// koordinaten vom ball
/*	pos_x = camera_position_x + distance * cos(corrected_alpha / 180.0 * PI) * cos(-corrected_beta / 180.0 * PI);
	pos_y = camera_position_y - distance * sin(corrected_alpha / 180.0 * PI) * cos(-corrected_beta / 180.0 * PI);
	pos_z = camera_position_z + distance * sin(-corrected_beta / 180.0 * PI); // geht nur wenn ball in mitte vom bild
*/
	pos_x = camera_position_x + distance * cos(corrected_alpha / 180.0 * PI) * cos(corrected_beta / 180.0 * PI);
	pos_y = camera_position_y + distance * sin(corrected_alpha / 180.0 * PI) * cos(corrected_beta / 180.0 * PI);
	pos_z = camera_position_z + distance * sin(corrected_beta / 180.0 * PI); // geht nur wenn ball in mitte vom bild

	return;
}

// berechnet den Abstand von einem Objekt zur Kamera anhand der wirklichen Grösse (in Meter) und der Grösse auf dem Bild (in Pixel)
float Coordinates::getObjectDistance(float real_size, float measured_size) {
	return real_size / sin(camera_fieldview_horizontal * measured_size / IMAGE_WIDTH / 180.0 * PI);
}

float Coordinates::getTargetDistance(float distance_in_cs_flat) {
	float distance_to_cs, distance_to_gun;
	distance_to_cs = distance_in_cs_flat / cos(beta / 180.0 * PI);
	if(beta > 0){
		distance_to_gun = distance_to_cs - (tower_feed + tower_side_arm_gun * cos(beta / 180.0 * PI));
	} else {
		distance_to_gun = distance_to_cs + (tower_feed + tower_side_arm_gun * cos(beta / 180.0 * PI));
	}
	return distance_to_cs;
}


float Coordinates::get_best_alpha(int pic_x, int pic_y){
	return alpha + camera_fieldview_horizontal/2 * (pic_x-(IMAGE_WIDTH/2)) / (IMAGE_WIDTH/2);
}
float Coordinates::get_best_beta(int pic_x, int pic_y){
	return beta + camera_fieldview_vertical/2 * (pic_y-(IMAGE_HEIGHT/2)) / (IMAGE_HEIGHT/2);
}

