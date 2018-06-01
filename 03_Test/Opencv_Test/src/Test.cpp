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

  Mat frame;
  printf("Start grabbing, press a key on Live window to terminate\n\r");
  while(1)
  {
    cap >> frame;
    if (frame.empty())
    {
    	printf("ERROR: Unable to grab from the camera\n\r");
        break;
    }
    imshow("Live",frame);
    int key = cv::waitKey(5);
    key = (key==255) ? -1 : key; //#Solve bug in 3.2.0
    if (key>=0)
      break;
  }

  printf("Closing the camera\n\r");
  cap.release();
  destroyAllWindows();

  waitKey(0);
  return 0;
}


