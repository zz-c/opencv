#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	Mat src = imread("E:/clib/data/test.jpg", IMREAD_GRAYSCALE);
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	imshow("input image", src);

	// kaze detection
	Ptr<AKAZE> detector = AKAZE::create();
	vector<KeyPoint> keypoints;
	double t1 = getTickCount();
	detector->detect(src, keypoints, Mat());
	double t2 = getTickCount();
	double tkaze = 1000 * (t2 - t1) / getTickFrequency();
	printf("KAZE Time consume(ms) : %f", tkaze);

	Mat keypointImg;
	drawKeypoints(src, keypoints, keypointImg, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	imshow("kaze key points", keypointImg);

	waitKey(0);
	return 0;
}