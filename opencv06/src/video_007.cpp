#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char**) {
	VideoCapture capture;
	capture.open("D:/vcprojects/images/768x576.avi");
	if (!capture.isOpened()) {
		printf("could not load video data...\n");
		return -1;
	}

	namedWindow("input video", CV_WINDOW_AUTOSIZE);
	namedWindow("motion objects", CV_WINDOW_AUTOSIZE);

	// ³õÊ¼BSÄ£ÐÍ
	Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2();
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));

	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	int count = 0;

	Mat frame, gray, mogMask;
	while (capture.read(frame)) {
		imshow("input video", frame);
		pMOG2->apply(frame, mogMask);
		threshold(mogMask, mogMask, 100, 255, THRESH_BINARY);
		morphologyEx(mogMask, mogMask, MORPH_OPEN, kernel, Point(-1, -1));

		findContours(mogMask, contours, hireachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		count = 0;
		char numText[8];
		for (size_t t = 0; t < contours.size(); t++) {
			double area = contourArea(contours[t]);
			if (area < 1000) continue;
			Rect selection = boundingRect(contours[t]);
			if (selection.width < 30 || selection.height < 30) continue;
			count++;
			rectangle(frame, selection, Scalar(0, 0, 255), 2, 8);

			sprintf(numText, "%d", count);
			putText(frame, numText, Point(selection.x, selection.y), CV_FONT_NORMAL, FONT_HERSHEY_PLAIN, Scalar(255, 0, 0), 1, 8);
		}
		
		imshow("motion objects", frame);
		char c = waitKey(50);
		if (c == 27) {// ESC 
			break;
		}
	}

	capture.release();
	waitKey(0);
	return 0;
}