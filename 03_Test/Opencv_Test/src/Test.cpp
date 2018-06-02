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
int main( int argc, char** argv )
{

  Mat image;
  image = imread( argv[1], 1 );
  if( argc != 2 || !image.data )
    {
      printf( "No image data \n" );
      return -1;
    }
  namedWindow( "Display Image", WINDOW_AUTOSIZE );
  imshow( "Display Image", image );


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
    // Split in 3 different images
    /*split(hsvimage, channels);
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
    inRange(hsvimage, Scalar(20, 0,160), Scalar(165, 50,220), hsvimage);
    //inRange(hsvimage, Scalar(27, 4,160), Scalar(56, 50, 260), hsvimage);
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


