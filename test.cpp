#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <cv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Point top_lf;
Point top_rt;
Point bottom_lf;
Point bottom_rt;

Point2f src_frame[4];
Point2f dst_frame[4];

void perspective(Mat src,int a){
	Mat dst = Mat(src.size(),src.type());
	Point top_lf = Point(300,370);
	Point top_rt = Point(400,370);
	Point bottom_lf = Point(280,470);
	Point bottom_rt = Point(430,470);

	float w = top_rt.x - top_lf.x;
	float h = bottom_rt.y - top_rt.y;

	src_frame[0] = top_lf;
	src_frame[1] = bottom_lf;
	src_frame[2] = bottom_rt;
	src_frame[3] = top_rt;

	dst_frame[0] = Point2f(300,370);
	dst_frame[1] = Point2f(300,470);
	dst_frame[2] = Point2f(400,470);
	dst_frame[3] = Point2f(400,370);

	Mat warping = getPerspectiveTransform(src_frame,dst_frame);
	warpPerspective(src,dst,warping,dst.size(),CV_INTER_LINEAR);
	
} 


int main(int argc, char**argv)
{
   VideoCapture cap("/home/cseecar/catkin_ws/video/forward_center.mp4");

  //VideoCapture cap("compete.mp4");


  //if(!cap.isOpened()){
    //std::cout<<"no camera!"<< std::endl;
  //  return -1;
  //}

 // int key, fr_no=0;
  //bool capture = true;
  //for(;;){
        //  cap >> frame;
	  //if (frame.empty())
	//	  break;

	//if ((key = waitKey(30)) == 27) {
		//	break;
	//}

	//fr_no++;
      	Mat frame;
		
		cap >> frame;
		int fpsNum = cap.get(CV_CAP_PROP_FPS);
		int fps = 1800.0/ (fpsNum);

		while(1){
			cap >> frame;
			if (frame.empty())
			  break;
		Mat drive;		
		drive = frame.clone();
		Rect rect(220,330,250,140);
		Mat show;
		Mat show1 = frame(rect);

		cvtColor(show1,show,COLOR_BGR2GRAY);
		GaussianBlur(show,show,Size(5,5),5,5);
		//adaptiveThreshold(show,show,255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV,15,6);		
		Canny(show, show, 60, 200);
		imshow("blur-canny",show);
		//adaptiveThreshold(show,show,255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV,15,10);
		//imshow("threshold",show);
		vector<Vec4i> lines;

		HoughLinesP(show, lines, 1, CV_PI/180, 30, 20, 30);
		for(size_t i = 0; i < lines.size(); i++)
		{ 
			Vec4i l = lines[i];
			
			Point p1 = Point(l[0],l[1]);
			Point p2 = Point(l[2],l[3]);			

			double slope = atan2(p1.y - p2.y, p1.x - p2.x)*180/ CV_PI;
			line(show1,Point(l[0],l[1]),Point(l[2],l[3]),Scalar(255,0,0),2);
			cout<< "slope degree:  " << slope <<endl;
		} 
		
		//for(int j = 0; j <lines.size(); j++){

		//Vec4i hello =lines[j];
		//double slope = (atan2(hello[1]-hello[3]), hello[0]-hello[2])*180/ CV_PI;
		//cout<< "slope degree" << slope <<endl;

		//}

		imshow("test",show1);
		waitKey(fps);
  }
	
  //return 0;
}


