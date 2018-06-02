/*
 * Test.cpp
 *
 *  Created on: 1 Jun 2018
 *      Author: pi
 */

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>


using namespace cv;

const int max_value_H = 360/2;
const int max_value = 255;
const String window_detection_name = "Object Detection";
int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;


static void on_low_H_thresh_trackbar(int, void *)
{
    low_H = min(high_H-1, low_H);
    setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
    high_H = max(high_H, low_H+1);
    setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
    low_S = min(high_S-1, low_S);
    setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
    high_S = max(high_S, low_S+1);
    setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
    low_V = min(high_V-1, low_V);
    setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
    high_V = max(high_V, low_V+1);
    setTrackbarPos("High V", window_detection_name, high_V);
}



int main( int argc, char** argv )
{
  // Raspberry Pi Camera Test
  VideoCapture cap(0);
  if (!cap.isOpened())
  {
	  printf("ERROR: Unable to open the camera\n\r");
  }

  Mat originalImage;
  Mat hsvimage;
  Mat channels[3];

  printf("Start grabbing, press a key on Live window to terminate\n\r");

  while(1)
  {
	// Get actual image
	cap >> originalImage;

    // Convert image from camera to HSV space
    cvtColor(originalImage, hsvimage, COLOR_BGR2HSV,0);
    //imshow("hsvimage",hsvimage);

    // Use the following lines to evaluate the thresholds:
    /*namedWindow(window_detection_name);

    // Trackbars to set thresholds for HSV values
    createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
    createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
    createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
    createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
    createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
    createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);

    inRange(hsvimage, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), hsvimage);

    imshow(window_detection_name, hsvimage);

    // Split in 3 different images
    split(hsvimage, channels);
    imshow("hsvimage1",channels[0]);
    imshow("hsvimage2",channels[1]);
    imshow("hsvimage3",channels[2]);

    inRange(channels[0], Scalar(5, 0, 150), Scalar(8, 5, 170),channels[0]);
    inRange(channels[1], Scalar(160, 0, 160), Scalar(190, 5, 180),channels[1]);
	inRange(channels[2], Scalar(150, 0, 130), Scalar(170, 5, 185),channels[2]);

    imshow("hsvimage1",channels[0]);
    imshow("hsvimage2",channels[1]);
    imshow("hsvimage3",channels[2]);
	*/

    // Apply thresholds
    inRange(hsvimage, Scalar(0, 0, 0), Scalar(139, 196, 243), hsvimage);
    imshow("hsvimage",hsvimage);

    // Smooth image
    GaussianBlur(hsvimage, hsvimage, Size(9,9), 0, 0);

    // Search for circles
    std::vector<Vec3f> circles;
    HoughCircles(hsvimage, circles, HOUGH_GRADIENT, 1, hsvimage.rows/8, 100, 30, 0, 0);

    // Highlight detected circles
    Mat cimg = originalImage;
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        circle( cimg, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, LINE_AA);
        circle( cimg, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, LINE_AA);
    }
    imshow("detected circles", cimg);

    // Check exit condition for endless loop
    if (cv::waitKey(5)>=0)
    {
     break;
    }
  }

  printf("Closing the camera\n\r");
  cap.release();
  destroyAllWindows();


  waitKey(0);
  return 0;
}


