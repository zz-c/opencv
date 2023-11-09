#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//未完成
int main(int argc, char** argv) {
	//VideoCapture capture;
	//capture.open("G:/project/c/opencv/opencv06/source/video_006.mp4");
	VideoCapture capture(0);
	if (!capture.isOpened()) {
		printf("could not load video data...\n");
		return -1;
	}

	Mat frame;
	namedWindow("tracker demo", WINDOW_AUTOSIZE);
	cv::Ptr<cv::TrackerKCF> tracker = cv::TrackerKCF::create();

	int readCount = 0;
	while(readCount<50){
		capture.read(frame);
		readCount++;
		char c = waitKey(20);
		if (c == 27) {
			break;
		}
	}
	

	cv::Rect2d  roi = selectROI("tracker demo", frame);
	printf("roi x=%d,y=%d\n", roi.x, roi.y);
	//cv::Rect2d roi(100, 100, 50, 50);
	//vector<Rect2d> roi;
	//selectROI("Multiple Objects Tracking", frame, roi);
	if (roi.width == 0 || roi.height == 0) {
		return -1;
	}
	tracker->init(frame, roi);
	while (capture.read(frame)) {
		if (tracker->update(frame, cv::Rect(roi))) {//cv::Rect(roi) 无法用 "cv::Rect2d" 类型的值初始化 "cv::Rect &" 类型的引用(非常量限定)	
			cv::rectangle(frame, roi, cv::Scalar(0, 255, 0), 2);
		}
		imshow("tracker demo", frame);

		char c = waitKey(20);
		if (c == 27) {
			break;
		}
	}

	capture.release();
	waitKey(0);
	return 0;
}