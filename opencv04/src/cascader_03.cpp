#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

CascadeClassifier face_cascader;
String haarfile = "E:/clib/opencv-4.8.0/etc/haarcascades/haarcascade_frontalface_alt.xml";
String lbpfile = "E:/clib/opencv-4.8.0/etc/lbpcascades/lbpcascade_frontalface.xml";
int main(int argc, char** argv) {
	if (!face_cascader.load(lbpfile)) {//haarfile lbpfile
		printf("could not load image...\n");
		return -1;
	}
	namedWindow("demo-win", WINDOW_AUTOSIZE);
	Mat src = imread("E:/clib/data/test.jpg");
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	imshow("input image", src);

	Mat gray;
	vector<Rect> faces;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	equalizeHist(gray, gray);
	int st = getTickCount();
	face_cascader.detectMultiScale(gray, faces, 1.1, 3, 0, Size(30, 30));
	int et = (getTickCount() - st);
	printf("time consume : %d", et);

	for (size_t t = 0; t < faces.size(); t++) {
		rectangle(src, faces[t], Scalar(255, 0, 0), 2, 8, 0);
	}
	imshow("demo-win", src);


	waitKey(0);
	return 0;
}