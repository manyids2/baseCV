#include "opencv2/opencv.hpp"

using namespace cv;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat edges;
    namedWindow("Video",1);
    Mat frame;
    cap >> frame; // get a new frame from camera
    imshow("video", frame);
    waitKey(2000);
    
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}