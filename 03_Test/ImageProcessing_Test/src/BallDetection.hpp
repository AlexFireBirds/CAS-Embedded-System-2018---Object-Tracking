/*
 * BallDetection.hpp
 *
 *  Created on: 2 Jun 2018
 *      Author: pi
 */

#ifndef SRC_BALLDETECTION_HPP_
#define SRC_BALLDETECTION_HPP_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

typedef struct
{
	int x;
	int y;
} BallCoordinates;


class BallDetection
{
public:
	BallDetection(void);  		// Constructor
    ~BallDetection(); 			// Destructor

    unsigned int NumberOfDetectedBalls(void);
    bool IsDetectionRunning(void);
    void ExecuteDetecionCycle(cv::Mat image);
    BallCoordinates GetCoordinatesOfBall(void);

    void SetLow_H(int value);
    void SetLow_S(int value);
    void SetLow_V(int value);
    void SetHigh_H(int value);
    void SetHigh_S(int value);
    void SetHigh_V(int value);

private:
    unsigned int numberOfDetectedBalls;
    bool isDetectionRunning;
    BallCoordinates coordinatesOfBall;
    int low_H;
    int low_S;
    int low_V;
    int high_H;
    int high_S;
    int high_V;
};


#endif /* SRC_BALLDETECTION_HPP_ */
