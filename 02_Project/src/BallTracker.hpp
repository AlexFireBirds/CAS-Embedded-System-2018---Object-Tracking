/*
 * BallTracker.hpp
 *
 *  Created on: 7 Jun 2018
 *      Author: pi
 */

#ifndef SRC_BALLTRACKER_HPP_
#define SRC_BALLTRACKER_HPP_

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
} Coordinate;


class BallTracker
{
public:
	BallTracker(cv::Mat image);  		// Constructor
    ~BallTracker(); 			// Destructor

    void DrawTargetWindow(cv::Mat image);
    void SetTargetWindowSize(unsigned int width, unsigned height);
    cv::Point GetUpperLeftCornerOfTargetWindow(void);
    cv::Point GetLowerRightCornerOfTargetWindow(void);
    Coordinate GetCenterOfImage(void);


private:
    int imageWidth;
    int imageHeight;

    unsigned int targetWindowWidth;
    unsigned int targetWindowHeight;
    cv::Point upperLeftCornerOfTargetWindow;
    cv::Point lowerRightCornerOfTargetWindow;
    Coordinate centerOfImage;
};

#endif /* SRC_BALLTRACKER_HPP_ */
