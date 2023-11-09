#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	VideoCapture capture;
	capture.open("D:/vcprojects/images/mulballs.mp4");
	if (!capture.isOpened()) {
		printf("could not load video data...\n");
		return -1;
	}

	namedWindow("Multiple Objects Tracking", WINDOW_AUTOSIZE);
	MultiTracker trackers("KCF");
	vector<Rect2d> objects;

	Mat frame, gray;
	capture.read(frame);//¶ÁÈ¡µÚÒ»Ö¡ 
	selectROI("Multiple Objects Tracking", frame, objects);
	if (objects.size() < 1) {
		return -1;
	}

	trackers.add(frame, objects);
	while (capture.read(frame)) {
		trackers.update(frame);
		for (size_t t = 0; t < trackers.objects.size(); t++) {
			rectangle(frame, trackers.objects[t], Scalar(0, 0, 255), 2, 8, 0);
		}
		imshow("Multiple Objects Tracking", frame);
		char c = waitKey(5);
		if (c == 27) {
			break;
		}
	}

	capture.release();
	waitKey(0);
	return 0;
}