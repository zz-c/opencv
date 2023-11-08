#include "opencv2/opencv.hpp"
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

void drawOpticalFlowHF(const Mat &flowdata, Mat& image, int step);
int main(int argc, char** argv)
{
	VideoCapture capture;
	capture.open("G:/project/c/opencv/opencv06/source/bike.avi");
	if (!capture.isOpened()) {
		printf("could not load image...\n");
		return -1;
	}

	Mat frame, gray;
	Mat prev_frame, prev_gray;
	Mat flowResult, flowdata;
	capture.read(frame);
	cvtColor(frame, prev_gray, COLOR_BGR2GRAY);
	namedWindow("flow", WINDOW_AUTOSIZE);
	namedWindow("input", WINDOW_AUTOSIZE);

	// 从第二帧数据开始
	while(capture.read(frame))
	{
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		if (!prev_gray.empty()) {
			calcOpticalFlowFarneback(prev_gray, gray, flowdata, 0.5, 3, 15, 3, 5, 1.2, 0);
			cvtColor(prev_gray, flowResult, COLOR_GRAY2BGR);
			drawOpticalFlowHF(flowdata, flowResult, 10);
			imshow("flow", flowResult);
			imshow("input", frame);
		}
		char c = waitKey(1);
		if (c == 27) {
			break;
		}
	}
	return 0;
}

void drawOpticalFlowHF(const Mat &flowdata, Mat& image, int step) {
	for (int row = 0; row < image.rows; row++) {
		for (int col = 0; col < image.cols; col++) {
			const Point2f fxy = flowdata.at<Point2f>(row, col);
			if (fxy.x > 1 || fxy.y > 1) {
				line(image, Point(col, row), Point(cvRound(col + fxy.x), cvRound(row + fxy.y)), Scalar(0, 255, 0), 2, 8, 0);
				circle(image, Point(col, row), 2, Scalar(0, 0, 255), -1);
			}
		}
	}
}