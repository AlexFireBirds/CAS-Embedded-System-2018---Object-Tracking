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
	unsigned int x;
	unsigned int y;
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

private:
    unsigned int numberOfDetectedBalls;
    bool isDetectionRunning;
    BallCoordinates coordinatesOfBall;
};


#endif /* SRC_BALLDETECTION_HPP_ */
