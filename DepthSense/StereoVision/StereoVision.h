#pragma once

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/calib3d.hpp>

#include<iostream>
#include<string>

using namespace cv;
using namespace std;

class StereoVision {
public:

	StereoVision(float baseline,float alpha,float focalLength)
	: baseline(baseline) , alpha(alpha) , focalLength(focalLength){}

//	void undistortFrame(Mat& frame);

	Mat add_HSV_filter(Mat& frame, int camera);

	Point find_ball(Mat& frame, Mat& mask);

	float find_depth(Point circleLeft, Point circleRight, Mat& leftFrame, Mat& rightFrame);




private:

	float baseline = 0;
	float alpha = 0;
	float focalLength = 0;
};
