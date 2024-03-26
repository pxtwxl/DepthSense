#include "StereoVision.h"

Mat StereoVision::add_HSV_filter(Mat& frame, int camera) {

	GaussianBlur(frame, frame, { 5,5 }, 0);
	
	cvtColor(frame, frame, COLOR_BGR2HSV);


	Mat mask;

	vector<int> lowerlimitRedRight = { 60,110,50 };
	vector<int> upperlimitRedRight = { 255,255,255 };
	vector<int> lowerlimitRedLeft = { 140,110,50 };
	vector<int> upperlimitRedLeft = { 255,255,255 };

	if (camera == 1) {
		inRange(frame, lowerlimitRedRight, upperlimitRedRight, mask);
	}
	else {
		inRange(frame, lowerlimitRedLeft, upperlimitRedLeft, mask);
	}

	erode(mask, mask, (3, 3));
	dilate(mask, mask, (3, 3));

	return mask;
}

Point StereoVision::find_ball(Mat& frame, Mat& mask) {
	vector<vector<Point>> contours;

	findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	sort(contours.begin(), contours.end(), [](const vector<Point>& c1, const vector<Point>& c2) {
		return contourArea(c1, false) < contourArea(c2, false);
	});

	if (contours.size() > 0) {

		vector<Point> largestContour = contours[contours.size() - 1];
		Point2f center;
		float radius;
		minEnclosingCircle(largestContour, center, radius);
		Moments m = moments(largestContour);
		Point centerPoint(m.m10 / m.m00, m.m01 / m.m00);


		if (radius > 10) {
			circle(frame, center, int(radius), (0, 255, 255), 2);
			circle(frame, centerPoint,5, (0, 0, 255), -1);
		}

		return centerPoint;
	}

	return { 0,0 };
}

float StereoVision::find_depth(Point circleLeft, Point circleRight, Mat& leftFrame, Mat& rightFrame) {

	int focal_pixels = 0;

	if (rightFrame.cols == leftFrame.cols) {
		focal_pixels = (rightFrame.cols * 0.5) / tan(alpha * 0.5 * CV_PI / 180.0);
	}
	else {
		cout << "Left and Right Camera Frames do not have same pixel width" << endl;
	}

	int xleft = circleLeft.x;
	int xright = circleRight.x;

	int disparity = xleft - xright;

	float zDepth = (baseline * float(focal_pixels) / float(disparity));

	return abs(zDepth);

}