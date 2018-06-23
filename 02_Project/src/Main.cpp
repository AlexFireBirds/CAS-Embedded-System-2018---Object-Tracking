/*
 * Main.cpp
 *
 *  Created on: 16 Jun 2018
 *      Author: pi
 */


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <unistd.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>

#include "ThresholdEvaluator.hpp"
#include "BallDetection.hpp"
#include "BallTracker.hpp"
#include "Servo.hpp"
#include "GPIO.hpp"
#include "Coordinates.hpp"
#include "PhysicalObject.hpp"

#define PI 3.14159265

#define PAN_MAX		 90		// links
#define PAN_CENTER	  0		// geradeaus
#define PAN_MIN		-90		// rechts

#define TILT_MIN	-70		// oben
#define TILT_CENTER	-23		// horizont (korrekturfaktor)
#define TILT_MAX	 20		// unten (schiesst in Boden)


using namespace cv;


int main( int argc, char** argv )
{
	// Setup modules
	BallDetection ballDetector;
	double panAxisCorrection;
	double tiltAxisCorrection;
	int distanceBetweenCenterAndBall = 0;
	bool isTargetLocked = false;
	unsigned int readKey = 0;
	unsigned int counterOfTargetLocks = 0;
	cv::Mat originalImage;

	// Instanz von Koordinatensystem mit Hilfslinien
	Coordinates Coordinates;
	// Coordinates.tilt_correction = TILT_CENTER;
	float help_line_distance = 1.0;
	int help_line_y_x1, help_line_y_y1, help_line_y_x2, help_line_y_y2;

	// Instanz von Physikalischem Objekt als Ball
	PhysicalObject theBall(0.0, 0.0, 0.0);

	// Setup servos
	Servo panServo(Servo::PAN);
	panServo.enable();
	Servo tiltServo(Servo::TILT);
	tiltServo.enable();

	// Setup GPIOs
	GPIO GPIO23(23);
	GPIO23.setup();
	GPIO23.setPulseTime(100);

	// Open camera
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		std::cout<<"ERROR: Unable to open the camera\n\r";
		return -1;
	}

	// Get actual image (640 x 480)
	cap >> originalImage;

	// Setup BallTracker
	BallTracker balltracking(originalImage);
	balltracking.SetTargetWindowSize(27, 27);

	// Set thresholds of ball detector
	ballDetector.SetLow_H(5);
	ballDetector.SetLow_S(94);
	ballDetector.SetLow_V(189);

	ballDetector.SetHigh_H(21);
	ballDetector.SetHigh_S(229);
	ballDetector.SetHigh_V(255);

	// Init servos
	float panAxisAngle = PAN_CENTER;		// pos rechts, neg links (verdreht!!)	// alpha (positive Werte nach links, neg. nach rechts)
	float tiltAxisAngle = TILT_CENTER;		// pos oben, neg unten	 (verdreht!!)	// beta  (negative Werte nach unten)
	panServo.setAngle(panAxisAngle);
	tiltServo.setAngle(tiltAxisAngle);
	Coordinates.updateCamPos(panAxisAngle, tiltAxisAngle-TILT_CENTER);
	usleep(1000000); // faktor 5 reduziert

	// Bild auf Monitor verschieben
	cap >> originalImage;
	imshow("Processd image", originalImage);
	cv::moveWindow("Processd image", 500 , 200);

	// Zeitvariabeln für Vorausberechnungen
	clock_t cpu_time;
	clock_t cpu_time_last;
	int time_diff;

	// variabeln für Steuerung
	bool quit = false;
	bool debug_mode = true;
	bool debug_coord = true;
	bool gravitation = true;
	unsigned mode = 3;

	// variablen zum debuggen - auf bild schreiben
	unsigned line_nr = 1;
	const float text_size = 0.8;
	const unsigned line_dist = 15;
	const unsigned offset = 5;

	// schuss unterwegs ... damit bilder gemacht werden koennen, also kein sleep
	bool shooting = false;
	bool reloading = false;
	bool level5target = false;
	unsigned level5targetTime;
	unsigned ticks_end_shooting = clock() + 100000;

	// alte bilder löschen
	system("rm -r -f /home/pi/campics/*");

	// bilder abspeichern
	bool take_pictures = false;
	unsigned ticks_end_pictures = clock() + 2000000;
	unsigned pic_series = 11;
	unsigned pic_counter = 101;
	// mkdir campics manuell auf konsole ausfuehren
	system(("mkdir /home/pi/campics/serie_" + to_string(pic_series)).c_str());

	// temporär wegen schlechter programmierung
	float curr_pos_x, curr_pos_y, curr_pos_z, curr_distance;

	while(not quit)
	{
		line_nr = 1;

		// umlaufzeit berechnen
		cpu_time_last = cpu_time;
		cpu_time = clock();
		time_diff = (cpu_time - cpu_time_last) / (CLOCKS_PER_SEC / 1000);

		// ventil nach schuss wieder abstellen
		if(shooting) {
			if(cpu_time > ticks_end_shooting) {
				shooting = false;
				GPIO23.deactivate();
			}
		}

		// aufhoeren bilder zu speichern?
		if(take_pictures) {
			if(cpu_time > ticks_end_pictures) {
				take_pictures = false;
				pic_series++;
				system(("mkdir /home/pi/campics/serie_" + to_string(pic_series)).c_str());
				pic_counter = 101;
			}
		}

		// Keyboard abfragen
		readKey = (cv::waitKey(1) & 0xEFFFFF);
		if(readKey != 15728639) { // muss schlaufe nicht durchlaufen
		switch(readKey) {
			// exit mit ESC
			case 27:
				quit = true;
				break;
			// reset mit r
			case 114:
				panAxisAngle = PAN_CENTER;
				tiltAxisAngle = TILT_CENTER;
				panServo.setAngle(panAxisAngle);
				tiltServo.setAngle(tiltAxisAngle);
				Coordinates.updateCamPos(panAxisAngle, tiltAxisAngle-TILT_CENTER);
				theBall.activ = false;
				take_pictures = false;
				shooting = false;
				GPIO23.deactivate();
				help_line_distance = 1.0;
				break;
			// + und - für Hilfslinien
			case 171:
				if(help_line_distance < 0.95) {
					help_line_distance += 0.1;
				} else {
					help_line_distance += 0.5;
				}
				break;
			case 173:
				if(help_line_distance < 1.05) {
					help_line_distance -= 0.1;
				} else {
					help_line_distance -= 0.5;
				}
				if(help_line_distance < 0.1) {
					help_line_distance = 0.1;
				}
				break;

			// c für hilfslinien (Schreiben aufs bild)
			case 99:
				debug_coord = not debug_coord;
				break;
			// d für debug-modus (Schreiben aufs bild)
			case 100:
				debug_mode = not debug_mode;
				break;

			// g für Gravitation
			case 103:
				gravitation = not gravitation;
				break;

			// Zahlen für Modus
			// 1 = 1D (Hilfslinien als Distanz)
			case 49:
				mode = 1;
				break;
			// 2 = 2D (Hilfslinien als Distanz)
			case 50:
				mode = 2;
				break;
			// 3 = 3D Partymodus
			case 51:
				mode = 3;
				theBall.activ = false;
				break;
			// 4 = 3D (wie gehabt)
			case 52:
				mode = 4;
				break;
			// 5 = 3D (Endlösung)
			case 53:
				mode = 5;
				break;

			// Pfeiltasten für Servosteuerung (im Partymodus)
			case 81: // links
				if(mode == 3 && panAxisAngle < PAN_MAX){
					panAxisAngle++;
					panServo.setAngle(panAxisAngle);
					Coordinates.updateCamPos(panAxisAngle, tiltAxisAngle-TILT_CENTER);
				}
				break;
			case 82: // oben
				if(mode == 3 && tiltAxisAngle > TILT_MIN){
					tiltAxisAngle--;
					tiltServo.setAngle(tiltAxisAngle);
					Coordinates.updateCamPos(panAxisAngle, tiltAxisAngle-TILT_CENTER);
				}
				break;
			case 83: // rechts
				if(mode == 3 && panAxisAngle > PAN_MIN){
					panAxisAngle--;
					panServo.setAngle(panAxisAngle);
					Coordinates.updateCamPos(panAxisAngle, tiltAxisAngle-TILT_CENTER);
				}
				break;
			case 84: // unten
				if(mode == 3 && tiltAxisAngle < TILT_MAX){
					tiltAxisAngle++;
					tiltServo.setAngle(tiltAxisAngle);
					Coordinates.updateCamPos(panAxisAngle, tiltAxisAngle-TILT_CENTER);
				}
				break;
			// s zum schiessen (im Partymodus)
			case 115:
				if(mode == 3 && shooting == false && reloading == false) {
					// letztes bild speichern bevor es losgeht .. beschleunigt späteren vorgang
					imwrite("/home/pi/campics/serie_" + to_string(pic_series) + "/pic_100.jpg", originalImage);
					shooting = true;
					ticks_end_shooting = cpu_time + 100000;
					take_pictures = true;
					ticks_end_pictures = cpu_time + 2000000;
 					GPIO23.activate();
 					reloading = true;
				}
				break;

			// leertaste um nachladen zu bestätigen
			case 32:
				reloading = false;
				break;

			case 0:
				// nichts da keine Taste gedrueckt
				break;
			case 15728639:
				// nichts da keine Taste gedrueckt
				break;
			default:
				std::cout << "Unknown key pressed (" << readKey << ")\n\r";
				break;
		}
		} // extra if

		// Get actual image
		cap >> originalImage;


if(mode == 3) {
		// Faden kreuz einzeichnen
		// Zeit bis Pfeil über Ziel
		float gun_extra_beta 			= 3.0;	// Einstellung
		float bullet_velocity 			= 6.0;	// Einstellung
		Coordinates.updateGunPos(gun_extra_beta);
		float bullet_distance			= Coordinates.getTargetDistance(help_line_distance);
		float bullet_flight_time		= bullet_distance / bullet_velocity;
		float bullet_fall_during_flight = 0.5 * 9.81 * bullet_flight_time * bullet_flight_time;
		float bullet_height_at_target	= bullet_distance * sin(Coordinates.beta / 180.0 * PI) + Coordinates.gun_position_z - bullet_fall_during_flight;
		float bullet_extra_beta			= tan(bullet_height_at_target+bullet_fall_during_flight)-tan(bullet_height_at_target);	// wieviel hoher zielen als winkel
		float target_x = help_line_distance * cos(Coordinates.alpha / 180.0 * PI);
		float target_y = help_line_distance * sin(Coordinates.alpha / 180.0 * PI);
		float target_z = bullet_height_at_target;
		int target_px;
		int target_py;
		Coordinates.calcPicPosXY(target_px, target_py, target_x, target_y, target_z);
		line(originalImage, Point(target_px-15, target_py), Point(target_px+15, target_py), Scalar(0, 0, 255));
		line(originalImage, Point(target_px, target_py+15), Point(target_px, target_py-15), Scalar(0, 0, 255));
		if(debug_mode) {
			cv::putText(originalImage,
					"target_x: " + to_string(target_x) + "  /  target_y: " + to_string(target_y) + "  /  target_z: " + to_string(target_z),
					cv::Point(offset, offset+line_dist*line_nr++), 				// Coordinates
					cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
					text_size, 							// Scale. 2.0 = 2x bigger
					cv::Scalar(255,255,255), 		// BGR colour
					1, 								// Line Thickness
					false);
		}

} // mode 3


if(mode == 4) {
		ballDetector.ExecuteDetecionCycle(originalImage);
		if(ballDetector.NumberOfDetectedBalls() >= 1)
		{
			// theBall.activ = true; --> geschieht über update_position
			// Target window is only drawed when ball is detected
			balltracking.DrawTargetWindow(originalImage);

			// grüner punkt und grüner kreis
			circle( originalImage, Point(ballDetector.GetCoordinatesOfBall().x, ballDetector.GetCoordinatesOfBall().y), 2, Scalar(0,255,0), 3, LINE_AA);
			//circle( originalImage, Point(ballDetector.GetCoordinatesOfBall().x, ballDetector.GetCoordinatesOfBall().y), ballDetector.GetCoordinatesOfBall().r, Scalar(0,255,0), 1, LINE_AA);

			// distanz zum ball
			curr_distance = Coordinates.getObjectDistance(theBall.radius, ballDetector.GetCoordinatesOfBall().r);
			Coordinates.calcCoordinates(curr_pos_x, curr_pos_y, curr_pos_z, ballDetector.GetCoordinatesOfBall().x, ballDetector.GetCoordinatesOfBall().y, curr_distance); // ballDetector.GetCoordinatesOfBall().r);
			theBall.update_position(curr_pos_x, curr_pos_y, curr_pos_z, time_diff);

			// Do a process cycle and get coordinates of the ball in this frame
			// Evaluate pan correction
			panAxisCorrection = 0;
			if (ballDetector.GetCoordinatesOfBall().x < balltracking.GetUpperLeftCornerOfTargetWindow().x)
			{
				distanceBetweenCenterAndBall = abs(balltracking.GetCenterOfImage().x - ballDetector.GetCoordinatesOfBall().x);
				panAxisCorrection = 0.2752*exp(0.0121 * distanceBetweenCenterAndBall);
			}
			if (ballDetector.GetCoordinatesOfBall().x > balltracking.GetLowerRightCornerOfTargetWindow().x)
			{
				distanceBetweenCenterAndBall = abs(balltracking.GetCenterOfImage().x - ballDetector.GetCoordinatesOfBall().x);
				panAxisCorrection = -(0.2752*exp(0.0121 * distanceBetweenCenterAndBall));
			}

			// Evaluate tilt correction
			tiltAxisCorrection = 0;
			if (ballDetector.GetCoordinatesOfBall().y > balltracking.GetLowerRightCornerOfTargetWindow().y)
			{
				distanceBetweenCenterAndBall = abs(balltracking.GetCenterOfImage().y - ballDetector.GetCoordinatesOfBall().y);
				tiltAxisCorrection = 0.2752*exp(0.0121 * distanceBetweenCenterAndBall);
			}
			if (ballDetector.GetCoordinatesOfBall().y < balltracking.GetUpperLeftCornerOfTargetWindow().y)
			{
				distanceBetweenCenterAndBall = abs(balltracking.GetCenterOfImage().y - ballDetector.GetCoordinatesOfBall().y);
				tiltAxisCorrection = -(0.2752*exp(0.0121 * distanceBetweenCenterAndBall));
			}

			// Is target locked and therefore corrections required?
			if(panAxisCorrection == 0 && tiltAxisCorrection == 0)
			{
				isTargetLocked = true;
			}
			else
			{
				isTargetLocked = false;

				// Check movement range of pan axis
				panAxisAngle = panServo.getAngle() + panAxisCorrection;
				tiltAxisAngle = tiltServo.getAngle() + tiltAxisCorrection;
				if(panAxisAngle > PAN_MAX)
				{
					panAxisAngle = PAN_MAX;
				}
				if(panAxisAngle < PAN_MIN)
				{
					panAxisAngle = PAN_MIN;
				}

				// Check movement range of tilt axis
				if(tiltAxisAngle > TILT_MAX)
				{
					tiltAxisAngle = TILT_MAX;
				}
				if(tiltAxisAngle < TILT_MIN)
				{
					tiltAxisAngle = TILT_MIN;
				}
				panServo.setAngle(panAxisAngle);
				tiltServo.setAngle(tiltAxisAngle);
				Coordinates.updateCamPos(panAxisAngle, tiltAxisAngle-TILT_CENTER);
			}
		} else {
			theBall.activ = false;
			theBall.update_position(0.0, 0.0, 0.0, time_diff);
		}

		// Arm cannon if target is locked
		if(isTargetLocked)
		{
			cv::putText(originalImage,
		            "Target locked",
					cv::Point(offset, offset+line_dist*line_nr++), 				// Coordinates
		            cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
					text_size, 							// Scale. 2.0 = 2x bigger
		            cv::Scalar(0,0,255), 			// BGR colour
		            1, 								// Line Thickness
		            false);

			counterOfTargetLocks++;

			// Shoot if 's' is pressed
			//if ((cv::waitKey(1) & 0xEFFFFF) == 115)
			if(counterOfTargetLocks >= 20)
			{
				if(reloading == false) {

				// geändert ...
				//GPIO23.pulseOutput();

				// letztes bild speichern bevor es losgeht .. beschleunigt späteren vorgang
				imwrite("/home/pi/campics/serie_" + to_string(pic_series) + "/pic_100.jpg", originalImage);
				shooting = true;
				ticks_end_shooting = cpu_time + 100000;
				take_pictures = true;
				ticks_end_pictures = cpu_time + 2000000;
				GPIO23.activate();
				reloading = true;

				cv::putText(originalImage,
			            //"Shot! Press any key to reset",
			            "Shot! Please reload and press space",
						cv::Point(offset, offset+line_dist*line_nr++), 				// Coordinates
			            cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
						text_size, 							// Scale. 2.0 = 2x bigger
			            cv::Scalar(0,255,0), 			// BGR colour
			            1, 								// Line Thickness
			            false);
				imshow("Processd image", originalImage);
				isTargetLocked = false;
				counterOfTargetLocks = 0;

				// warum?
				//panServo.setAngle(0);
				//tiltServo.setAngle(-10);
				} else {
					cv::putText(originalImage,
				            "press space to enable shooting",
							cv::Point(offset, offset+line_dist*line_nr++), 				// Coordinates
				            cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
							text_size, 							// Scale. 2.0 = 2x bigger
				            cv::Scalar(0,255,0), 			// BGR colour
				            1, 								// Line Thickness
				            false);

				}
			}
		}
		else
		{
			counterOfTargetLocks = 0;
		}
} // mode == 4



if(mode == 5) {

	// Beschleunigen durch Bildzuschneiden?
	// crop image
/*		cv::Rect crop_positions;
	crop_positions.x = (IMAGE_WIDTH-reduced_width)/2;
	crop_positions.y = 0;
	crop_positions.width = reduced_width;
	crop_positions.height = IMAGE_HEIGHT;
*/
		// ball suchen
		ballDetector.ExecuteDetecionCycle(originalImage);
		if(ballDetector.NumberOfDetectedBalls() >= 1) {
			// grüner punkt und grüner kreis
			circle( originalImage, Point(ballDetector.GetCoordinatesOfBall().x, ballDetector.GetCoordinatesOfBall().y), 2, Scalar(0,255,0), 3, LINE_AA);
			circle( originalImage, Point(ballDetector.GetCoordinatesOfBall().x, ballDetector.GetCoordinatesOfBall().y), ballDetector.GetCoordinatesOfBall().r, Scalar(0,255,0), 1, LINE_AA);
			// distanz zum ball
			curr_distance = Coordinates.getObjectDistance(theBall.radius, ballDetector.GetCoordinatesOfBall().r);
			Coordinates.calcCoordinates(curr_pos_x, curr_pos_y, curr_pos_z, ballDetector.GetCoordinatesOfBall().x, ballDetector.GetCoordinatesOfBall().y, curr_distance); // ballDetector.GetCoordinatesOfBall().r);
			theBall.update_position(curr_pos_x, curr_pos_y, curr_pos_z, time_diff);
		} else {
			theBall.activ = false;
			theBall.update_position(0.0, 0.0, 0.0, time_diff);
		}


		// Vorausberechnung ....
		if(theBall.activ) {
			int i, j;
			unsigned forcast_time_ms = 800;
			float next_bp_x, next_bp_y, next_bp_z;
			int next_b_x, next_b_y;
			theBall.reset_forecast();
			j=forcast_time_ms/time_diff;
			for(i=0; i<j; i++) {
				// geht das nur einmal oder geht das?
				theBall.forecast_position(next_bp_x, next_bp_y, next_bp_z, time_diff, gravitation);
				Coordinates.calcPicPosXY(next_b_x, next_b_y, next_bp_x, next_bp_y, next_bp_z);
				// blauer kreis
				circle( originalImage, Point(next_b_x, next_b_y), ballDetector.GetCoordinatesOfBall().r, Scalar(255-30*i,0,0), 1, LINE_AA);
				if(i == 0 && level5target == false) {
					if(reloading == false) {
						// Camera direkt auf Ball richten in nächster position
						panAxisAngle = panAxisAngle; //Coordinates.get_best_alpha(next_b_x, next_b_y);
						tiltAxisAngle = tiltAxisAngle; //Coordinates.get_best_beta(next_b_x, next_b_y)+TILT_CENTER;
						// cam ausrichten
						if(panAxisAngle > PAN_MAX)
						{
							panAxisAngle = PAN_MAX;
						}
						if(panAxisAngle < PAN_MIN)
						{
							panAxisAngle = PAN_MIN;
						}

						// Check movement range of tilt axis
						if(tiltAxisAngle > TILT_MAX)
						{
							tiltAxisAngle = TILT_MAX;
						}
						if(tiltAxisAngle < TILT_MIN)
						{
							tiltAxisAngle = TILT_MIN;
						}
						//panServo.setAngle(panAxisAngle);
						//tiltServo.setAngle(tiltAxisAngle);
						Coordinates.updateCamPos(panAxisAngle, tiltAxisAngle-TILT_CENTER);
					} else {
						cv::putText(originalImage,
					            "press space to enable shooting",
								cv::Point(offset, offset+line_dist*line_nr++), 				// Coordinates
					            cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
								text_size, 							// Scale. 2.0 = 2x bigger
					            cv::Scalar(0,255,0), 			// BGR colour
					            1, 								// Line Thickness
					            false);
					}
				}
			}
			if(level5target == false) {
				// geht durch Schranke ?
				if(next_bp_y < -0.2) {
					level5target = true;
					// Ball wird Linie überqueren, mache ab jetzt bilder
					// wird in 500 ms (forcast_time_ms) Schranke durchqueren ...
					imwrite("/home/pi/campics/serie_" + to_string(pic_series) + "/pic_100.jpg", originalImage);
					take_pictures = true;
					ticks_end_pictures = cpu_time + 3000000;

					// TREFFzeitpunktfestlegen
					level5targetTime = clock() + j*time_diff*1000;

					// zielen ...
					//panAxisAngle = Coordinates.get_best_alpha(next_b_x, next_b_y);
					//tiltAxisAngle = Coordinates.get_best_beta(next_b_x, next_b_y)+TILT_CENTER;
					// cam ausrichten
					if(panAxisAngle > PAN_MAX)
					{
						panAxisAngle = PAN_MAX;
					}
					if(panAxisAngle < PAN_MIN)
					{
						panAxisAngle = PAN_MIN;
					}

					// Check movement range of tilt axis
					if(tiltAxisAngle > TILT_MAX)
					{
						tiltAxisAngle = TILT_MAX;
					}
					if(tiltAxisAngle < TILT_MIN)
					{
						tiltAxisAngle = TILT_MIN;
					}
					//panServo.setAngle(panAxisAngle);
					//tiltServo.setAngle(tiltAxisAngle);
					Coordinates.updateCamPos(panAxisAngle, tiltAxisAngle-TILT_CENTER);
				}
			}

			// warten auf schuss?
			if(level5target && not reloading) {
				if(level5targetTime - clock() < time_diff*1000) {
					float shooting_delay = 0.01;
					float air_time = 0.05; // egentlich berechnenen
					float extra_time = (shooting_delay + air_time)*1000000;
					while(level5targetTime - clock() < extra_time) {
						// warte, aber nichts anderes mehr machen
					}
					// JETZT schuss
					shooting = true;
					ticks_end_shooting = cpu_time + 100000;
					GPIO23.activate();
					reloading = true;
					level5target = false;
				}
			}

		} else {
			// ball wieder verloren
			level5target = false;
			take_pictures = false;
		}
} // mode 5


		if(debug_coord) {
			// Hilfslinien einzeichnen
			// Horinzont in 1000 m Entfernung in orange
	/*		Coordinates.calcPicPosXY(help_line_y_x1, help_line_y_y1, 1000.0, 2000.0, 0.0);
			Coordinates.calcPicPosXY(help_line_y_x2, help_line_y_y2, 1000.0, -2000.0, 0.0);
			line(originalImage, Point(help_line_y_x1, help_line_y_y1), Point(help_line_y_x2, help_line_y_y2), Scalar(0, 140, 255));
	*/
			// Horizontal
			Coordinates.calcPicPosXY(help_line_y_x1, help_line_y_y1, help_line_distance, help_line_distance, 0.0);
			Coordinates.calcPicPosXY(help_line_y_x2, help_line_y_y2, help_line_distance, -help_line_distance, 0.0);
			line(originalImage, Point(help_line_y_x1, help_line_y_y1), Point(help_line_y_x2, help_line_y_y2), Scalar(255, 255, 255));
			// senkrechte zum horizont
			Coordinates.calcPicPosXY(help_line_y_x1, help_line_y_y1, 0.001, 0.0, 0.0);
			Coordinates.calcPicPosXY(help_line_y_x2, help_line_y_y2, help_line_distance, 0.0, 0.0);
			line(originalImage, Point(help_line_y_x1, help_line_y_y1), Point(help_line_y_x2, help_line_y_y2), Scalar(255, 255, 0));
			// links
			if(panAxisAngle > 13) { 	// wert 13 hängt vom öffnungswinkel der kamera ab
				Coordinates.calcPicPosXY(help_line_y_x1, help_line_y_y1, 0.001, help_line_distance, 0.0);
				Coordinates.calcPicPosXY(help_line_y_x2, help_line_y_y2, help_line_distance, help_line_distance, 0.0);
				line(originalImage, Point(help_line_y_x1, help_line_y_y1), Point(help_line_y_x2, help_line_y_y2), Scalar(255, 255, 255));
			}
			// rechts
			if(panAxisAngle < -13) {
				Coordinates.calcPicPosXY(help_line_y_x1, help_line_y_y1, 0.001, -help_line_distance, 0.0);
				Coordinates.calcPicPosXY(help_line_y_x2, help_line_y_y2, help_line_distance, -help_line_distance, 0.0);
				line(originalImage, Point(help_line_y_x1, help_line_y_y1), Point(help_line_y_x2, help_line_y_y2), Scalar(255, 255, 255));
			}
		}


		// Debuginfos in Bild schreiben
		if(debug_mode) {
			// Winkel
			cv::putText(originalImage,
					"fps: " + to_string(1000/time_diff) + "  /  Hilfslinien: " + to_string(help_line_distance) + " m  /  Modus: " + to_string(mode),
					cv::Point(offset, offset+line_dist*line_nr++), 				// Coordinates
					cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
					text_size, 							// Scale. 2.0 = 2x bigger
					cv::Scalar(255,255,255), 		// BGR colour
					1, 								// Line Thickness
					false);
			// Winkel
			cv::putText(originalImage,
					"pan: " + to_string(panAxisAngle) + "  /  tilt: " + to_string(tiltAxisAngle),
					cv::Point(offset, offset+line_dist*line_nr++), 				// Coordinates
					cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
					text_size, 							// Scale. 2.0 = 2x bigger
					cv::Scalar(255,255,255), 		// BGR colour
					1, 								// Line Thickness
					false);
			cv::putText(originalImage,
					"alpha: " + to_string(Coordinates.alpha) + "  /  beta: " + to_string(Coordinates.beta),
					cv::Point(offset, offset+line_dist*line_nr++), 				// Coordinates
					cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
					text_size, 							// Scale. 2.0 = 2x bigger
					cv::Scalar(255,255,255), 		// BGR colour
					1, 								// Line Thickness
					false);
			// berechnete camera position
			cv::putText(originalImage,
					"Kameraposition: " + to_string(Coordinates.camera_position_x) + " / " + to_string(Coordinates.camera_position_y) + " / " + to_string(Coordinates.camera_position_z),
					cv::Point(offset, offset+line_dist*line_nr++), 				// Coordinates
					cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
					text_size, 							// Scale. 2.0 = 2x bigger
					cv::Scalar(255,255,255), 		// BGR colour
					1, 								// Line Thickness
					false);
			cv::putText(originalImage,
					"Gunposition: " + to_string(Coordinates.gun_position_x) + " / " + to_string(Coordinates.gun_position_y) + " / " + to_string(Coordinates.gun_position_z),
					cv::Point(offset, offset+line_dist*line_nr++), 				// Coordinates
					cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
					text_size, 							// Scale. 2.0 = 2x bigger
					cv::Scalar(255,255,255), 		// BGR colour
					1, 								// Line Thickness
					false);

			if(theBall.activ) {
				// Distanz zum Ball
				cv::putText(originalImage,
						"Distanz zum Objekt: " + to_string(curr_distance),
						cv::Point(offset, offset+line_dist*line_nr++), 				// Coordinates
			            cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
						text_size, 							// Scale. 2.0 = 2x bigger
			            cv::Scalar(255,255,255), 		// BGR colour
			            1, 								// Line Thickness
			            false);
				// Ballkooridinaten
				cv::putText(originalImage,
		            	"Objektposition: " + to_string(curr_pos_x) + " / " + to_string(curr_pos_y) + " / " + to_string(curr_pos_z),
						cv::Point(offset, offset+line_dist*line_nr++), 				// Coordinates
			            cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
						text_size, 							// Scale. 2.0 = 2x bigger
			            cv::Scalar(255,255,255), 		// BGR colour
			            1, 								// Line Thickness
			            false);
			}
			// clock, als letzten Schritt Zeit aufs Bild schreiben
			cv::putText(originalImage,
					"clock_start: " + to_string(cpu_time) + "  /  clock_end: " + to_string(clock()),
					cv::Point(offset, 480-line_dist), 				// Coordinates
					cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
					text_size, 							// Scale. 2.0 = 2x bigger
					cv::Scalar(255,255,255), 		// BGR colour
					1, 								// Line Thickness
					false);
		}



		// Show image
		imshow("Processd image", originalImage);

		// Bilder speichern
		if(take_pictures) {
			imwrite("/home/pi/campics/serie_" + to_string(pic_series) + "/pic_" + to_string(pic_counter) + ".jpg", originalImage);
			pic_counter++;
		}

	} // endless loop

	// Shutdown
	std::cout<<"Closing the camera\n\r";
	cap.release();
	// GPIO23.~GPIO(); das verursacht den Fehler
	destroyAllWindows();

	return 0;
}





