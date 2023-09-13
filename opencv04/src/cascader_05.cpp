#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

String catfile = "E:/clib/opencv-4.8.0/etc/haarcascades/haarcascade_frontalcatface_extended.xml";
CascadeClassifier catface_detector;
int main(int argc, char** argv) {
	if (!catface_detector.load(catfile)) {
		printf("could not load image...\n");
		return -1;
	}
	Mat src = imread("D:/vcprojects/images/twocat.png");
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	imshow("input image", src);

	Mat gray;
	vector<Rect> cats;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	equalizeHist(gray, gray);

	catface_detector.detectMultiScale(gray, cats, 1.1, 3, 0, Size(70, 70));
	for (size_t t = 0; t < cats.size(); t++) {
		rectangle(src, cats[t], Scalar(0, 0, 255), 2, 8, 0);
	}

	// show
	namedWindow("Cat Face Detector Demo", CV_WINDOW_AUTOSIZE);
	imshow("Cat Face Detector Demo", src);

	waitKey(0);
	return 0;
}