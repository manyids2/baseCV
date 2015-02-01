
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 50;
int max_thresh = 255;

/// Function header
void thresh_callback(int, void* );

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat edges;
    namedWindow("Video",1);
    for(;;)
    {
        Mat frame;
        Mat canny_output, thresh_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        blur( frame, frame, Size(3,3) );
        threshold(frame , thresh_output, thresh, max_thresh, THRESH_BINARY);
        //Canny( frame, canny_output, thresh, thresh*2, 3 );
        findContours( thresh_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
        /// Draw contours
        Scalar color = Scalar( 0, 0, 255 );
        Mat drawing = Mat::zeros( thresh_output.size(), CV_8UC3 );
        for( int i = 0; i< contours.size(); i++ )
            {
                drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
            }
        imshow("Video", drawing);
        if(waitKey(10) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}