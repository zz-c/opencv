#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	Mat img1 = imread("E:/clib/data/test2.jpg", IMREAD_GRAYSCALE);
	Mat img2 = imread("E:/clib/data/test.jpg", IMREAD_GRAYSCALE);
	if (img1.empty() || img2.empty()) {
		printf("could not load images...\n");
		return -1;
	}
	imshow("box image", img1);
	imshow("scene image", img2);

	// extract BRISK features
	Ptr<Feature2D> detector = BRISK::create();
	vector<KeyPoint> keypoints_obj;
	vector<KeyPoint> keypoints_scene;
	Mat descriptor_obj, descriptor_scene;
	detector->detectAndCompute(img1, Mat(), keypoints_obj, descriptor_obj);
	detector->detectAndCompute(img2, Mat(), keypoints_scene, descriptor_scene);

	// matching
	BFMatcher matcher(NORM_L2);
	vector<DMatch> matches;
	matcher.match(descriptor_obj, descriptor_scene, matches);

	// draw matches
	Mat matchesImg;
	drawMatches(img1, keypoints_obj, img2, keypoints_scene, matches, matchesImg);
	imshow("BRISK MATCH RESULT", matchesImg);

	//  draw key points
	// Mat resultImg;
	// drawKeypoints(src, keypoints, resultImg, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	// imshow("Brisk Key Points", resultImg);

	waitKey(0);
	return 0;
}