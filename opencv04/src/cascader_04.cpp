#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

CascadeClassifier face_cascader;
String haarfile = "E:/clib/opencv-4.8.0/etc/haarcascades/haarcascade_frontalface_alt.xml";
String lbpfile = "E:/clib/opencv-4.8.0/etc/lbpcascades/lbpcascade_frontalface.xml";
int main(int argc, char** argv) {
	if (!face_cascader.load(haarfile)) {
		printf("could not load image...\n");
		return -1;
	}
	namedWindow("demo-win", WINDOW_AUTOSIZE);
	//VideoCapture capture;
	//capture.open("E:/clib/data/test-cg.mp4");
	VideoCapture capture(0);//…„œÒÕ∑
	Mat frame;
	Mat gray;
	vector<Rect> faces;
	
	while (capture.read(frame)) {
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		equalizeHist(gray, gray);
		face_cascader.detectMultiScale(gray, faces, 1.1, 3, 0, Size(30, 30));
		for (size_t t = 0; t < faces.size(); t++) {
			rectangle(frame, faces[t], Scalar(255, 0, 0), 2, 8, 0);
		}
		imshow("demo-win", frame);
		char c = waitKey(30);
		if (c == 27) { // ESC
			break;
		}
	}

	// release resource
	capture.release();
	waitKey(0);
	return 0;
}