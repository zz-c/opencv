#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Rect roi;
void processFrame(Mat &binary, Rect &rect);

int main(int argc, char* argv) {
	// load video
	VideoCapture capture;
	capture.open("G:/project/c/opencv/opencv06/source/video_006.mp4");
	if (!capture.isOpened()) {
		printf("could not find video file");
		return -1;
	}

	Mat frame, mask;
	Mat kernel1 = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	Mat kernel2 = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));

	namedWindow("input video", WINDOW_AUTOSIZE);
	namedWindow("track mask", WINDOW_AUTOSIZE);
	while (capture.read(frame)) {
		inRange(frame, Scalar(0, 127, 0), Scalar(120, 255, 120), mask); // 过滤绿色
		morphologyEx(mask, mask, MORPH_OPEN, kernel1, Point(-1, -1), 1); // 开操作
		dilate(mask, mask, kernel2, Point(-1, -1), 4);// 膨胀
		imshow("track mask", mask);

		processFrame(mask, roi); // 轮廓发现与位置标定
		rectangle(frame, roi, Scalar(0, 0, 255), 3, 8, 0);
		imshow("input video", frame);

		// trigger exit
		char c = waitKey(100);
		if (c == 27) {
			break;
		}
	}

	capture.release();
	waitKey(0);
	return 0;
}

void processFrame(Mat &binary, Rect &rect) {
	vector<vector<Point>> contours;
	vector<Vec4i> hireachy;
	findContours(binary, contours, hireachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	if (contours.size() > 0) {
		double maxArea = 0.0;
		for (size_t t = 0; t < contours.size(); t++) {
			double area = contourArea(contours[static_cast<int>(t)]);
			if (area > maxArea) {
				maxArea = area;
				rect = boundingRect(contours[static_cast<int>(t)]);
			}
		}
	}
	else {
		rect.x = rect.y = rect.width = rect.height = 0;
	}

}