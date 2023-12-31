#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat frame, gray;
Mat prev_frame, prev_gray;
vector<Point2f> features; // shi-tomasi角点检测 - 特征数据

vector<Point2f> iniPoints; // 初始化特征数据
vector<Point2f> fpts[2]; // 保持当前帧和前一帧的特征点位置

vector<uchar> status; // 特征点跟踪成功标志位
vector<float> errors; // 跟踪时候区域误差和

void drawFeature(Mat &inFrame);
void detectFeatures(Mat &inFrame, Mat &ingray);
void klTrackFeature();
void drawTrackLines();
int main(int argc, char** argv) {
	//VideoCapture capture(0);
	VideoCapture capture;
	capture.open("G:/project/c/opencv/opencv06/source/bike.avi");
	if (!capture.isOpened()) {
		printf("could not load video file...\n");
		return -1;
	}

	namedWindow("camera input", WINDOW_AUTOSIZE);
	while (capture.read(frame)) {
		//flip(frame, frame, 1);
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		if (fpts[0].size() < 40) {
			detectFeatures(frame, gray);
			fpts[0].insert(fpts[0].end(), features.begin(), features.end());
			iniPoints.insert(iniPoints.end(), features.begin(), features.end());
		}
		else {
			printf("tttttttttttttttttttttttttttttttttttttttt...\n");
		}

		if (prev_gray.empty()) {
			gray.copyTo(prev_gray);
		}

		klTrackFeature();
		drawFeature(frame);

		// 更新前一帧数据
		gray.copyTo(prev_gray);
		frame.copyTo(prev_frame);
		imshow("camera input", frame);

		char c = waitKey(50);
		if (c == 27) {
			break;
		}
	}

	waitKey(0);
	return 0;
}

void detectFeatures(Mat &inFrame, Mat &ingray) {
	double maxCorners = 5000;
	double qualitylevel = 0.01;
	double minDistance = 10;
	double blockSize = 3;
	double k = 0.04;
	goodFeaturesToTrack(ingray, features, maxCorners, qualitylevel, minDistance, Mat(), blockSize, false, k);
	cout << "detect features : " << features.size() << endl;
}

void drawFeature(Mat &inFrame) {
	for (size_t t = 0; t < fpts[0].size(); t++) {
		circle(inFrame, fpts[0][t], 2, Scalar(0, 0, 255), 2, 8, 0);
	}
}

void klTrackFeature() {
	// KLT
	calcOpticalFlowPyrLK(prev_gray, gray, fpts[0], fpts[1], status, errors);
	int k = 0;
	// 特征点过滤
	for (int i = 0; i < fpts[1].size(); i++) {
		double dist = abs(fpts[0][i].x - fpts[1][i].x) + abs(fpts[0][i].y - fpts[1][i].y);
		if (dist > 2 && status[i]) {
			iniPoints[k] = iniPoints[i];
			fpts[1][k++] = fpts[1][i];
		}
	}
	// 保存特征点并绘制跟踪轨迹
	iniPoints.resize(k);
	fpts[1].resize(k);
	drawTrackLines();

	std::swap(fpts[1], fpts[0]);
}

void drawTrackLines() {
	for (size_t t=0; t<fpts[1].size(); t++) {
		line(frame, iniPoints[t], fpts[1][t], Scalar(0, 255, 0), 1, 8, 0);
		circle(frame, fpts[1][t], 2, Scalar(0, 0, 255), 2, 8, 0);
	}
}