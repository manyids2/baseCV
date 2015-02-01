#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

class selectBox 
{
  public:
  Point source, sink, now;
  bool complete = false;
  
  selectBox () { source.x = 0; source.y = 0; sink.x = 0;  sink.y = 0; now.x = 0; now.y = 0; }
  void set_source(int,int);
  void set_sink(int,int);
  void set_now(int,int);
};

void selectBox::set_source (int a, int b) {
  source.x = a;
  source.y = b;
}

void selectBox::set_sink (int a, int b) {
  sink.x = a;
  sink.y = b;
}

void selectBox::set_now (int a, int b) {
  now.x = a;
  now.y = b;
}

class valueBox
{
public:
  Point h = Point(0,255),s = Point(0,255),v = Point(0,255);
};

bool drawBox = false;
selectBox selB;
valueBox vals;

valueBox calc_vals(Mat hsv, selectBox s)
{
  if ((s.source.x!=0)&&(s.sink.x!=0)){
  Rect r(selB.source.x,selB.source.y,selB.sink.x,selB.sink.y);
  double minVal, maxVal;
  Mat channel[3];
  Mat selhsv = hsv(r);
  split(hsv, channel);
  minMaxLoc(channel[0], &minVal, &maxVal);
  printf ("%d,%d\n",minVal,maxVal) ;}
}

static void onMouse( int event, int x, int y, int, void* param)
{
    Mat* hsv = (Mat*) param; 
    if( event == EVENT_LBUTTONDOWN )
    {
    	printf ("Callback  : %d , %d\n", x, y);
	if (selB.complete){ selB.complete = false; selB.set_sink(x,y); }
	else { selB.complete = true; selB.set_source(x,y); selB.set_sink(0,0);}
	printf ("Selection  : %d , %d ; %d , %d\n", selB.source.x, selB.source.y, selB.sink.x, selB.sink.y);
    	Vec3b intensity = hsv->at<Vec3b>(y, x);
		uchar hue = intensity.val[0];
		uchar sat = intensity.val[1];
		uchar val = intensity.val[2];
		printf ("HSV Values: %d , %d , %d\n", hue, sat, val);
        return;
    }
    if(event == EVENT_MOUSEMOVE )
      {
	selB.set_now(x,y);
      }
}

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat frame, frz, old;
    int keyP;
    bool freeze = false;
    namedWindow("Video",1);
    cap >> frz;
    frz.copyTo(old);
    setMouseCallback( "Video", onMouse, &frz);
    for(;;)
    {
      if(!freeze)
	  { 
	    cap >> frame; // get a new frame from camera
	    cvtColor(frame, frame, COLOR_BGR2HSV);
	    frz = frame;
	    if(selB.complete)
	      {
		rectangle( frz, selB.source, selB.now, Scalar( 0, 0, 255 ), 2, 8 );
	      }
	    if(!selB.complete)
	      {
		rectangle( frz, selB.source, selB.sink, Scalar( 0, 0, 255 ), 2, 8 );
	      }
	  }
      else
	  {
	    old.copyTo(frz);
	    if(selB.complete)
	      {
		rectangle( frz, selB.source, selB.now, Scalar( 0, 0, 255 ), 2, 8 );
	      }
	    if(!selB.complete)
	      {
		rectangle( frz, selB.source, selB.sink, Scalar( 0, 0, 255 ), 2, 8 );
	      }
	  }
	imshow("Video", frz);
	keyP = waitKey(1);
        if(keyP == 27){break;}
	if((keyP == 1020)||(keyP == 102)){freeze = !freeze; frz.copyTo(old);}
	if(keyP == 120){ selB.complete = false; selB.set_source(0,0), selB.set_sink(0,0); }
	if((keyP == 97)&&(!selB.complete))
	  { 
	    vals = calc_vals(frz,selB);
	    printf ("Maxima : H : %d ; S : %d ; V : %d\n", vals.h.x, vals.s.x, vals.v.x); 
	    printf ("Minima : H : %d ; S : %d ; V : %d\n", vals.h.y, vals.s.y, vals.v.y); 
	  }
	
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
