#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int x1, y1, x2, y2;

static void onMouse( int event, int x, int y, int, void* param)
{
	Mat* hsv = (Mat*) param;
    if( event == EVENT_LBUTTONDOWN )
    {
    	printf ("Callback  : %d , %d\n", x, y);
    	Vec3b intensity = hsv->at<Vec3b>(y, x);
		uchar hue = intensity.val[0];
		uchar sat = intensity.val[1];
		uchar val = intensity.val[2];
		printf ("HSV Values: %d , %d , %d\n", hue, sat, val);
        return;
    }
}

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat frame;
    namedWindow("Video",1);
    setMouseCallback( "Video", onMouse, &frame);
    for(;;)
    {
        cap >> frame; // get a new frame from camera
        cvtColor(frame, frame, COLOR_BGR2HSV);
        imshow("Video", frame);
        if(waitKey(1) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}