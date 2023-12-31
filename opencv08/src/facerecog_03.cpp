#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

double calcPCAOrientation(vector<Point> &pts, Mat &image);
//PCA（主成分分析）
int main(int argc, char** argv) {
	Mat src = imread("E:/clib/data/student.jpg");
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	namedWindow("input image", WINDOW_AUTOSIZE);
	imshow("input image", src);

	Mat gray, binary;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("binary image", binary);

	vector<Vec4i> hireachy;
	vector<vector<Point>> contours;
	findContours(binary, contours, hireachy, RETR_LIST, CHAIN_APPROX_NONE);
	Mat result = src.clone();
	for (int i = 0; i < contours.size(); i++) {
		double area = contourArea(contours[i]);
		if (area > 1e5 || area < 1e2) continue;//10的n次方
		drawContours(result, contours, i, Scalar(0, 0, 255), 2, 8);
		double theta = calcPCAOrientation(contours[i], result);
	}
	imshow("contours result", result);

	waitKey(0);
	return 0;
}
//计算方向
double calcPCAOrientation(vector<Point> &pts, Mat &image) {
	int size = static_cast<int>(pts.size());
	Mat data_pts = Mat(size, 2, CV_64FC1);
	for (int i = 0; i < size; i++) {
		data_pts.at<double>(i, 0) = pts[i].x;
		data_pts.at<double>(i, 1) = pts[i].y;
	}

	// perfrom PCA projection
	//PCA pca_analysis(data_pts, Mat(), CV_PCA_DATA_AS_ROW);
	PCA pca_analysis(data_pts, Mat(), PCA::DATA_AS_ROW);
	Point cnt = Point(static_cast<int>(pca_analysis.mean.at<double>(0, 0)),
					static_cast<int>(pca_analysis.mean.at<double>(0, 1)));
	circle(image, cnt, 2, Scalar(0, 255, 0), 2, 8, 0);//画中心点

	vector<Point2d> vecs(2);//特征矩阵
	vector<double> vals(2);//特征向量
	for (int i = 0; i < 2; i++) {
		vals[i] = pca_analysis.eigenvalues.at<double>(i, 0);
		printf("Th %d eigen value : %.2f\n", i, vals[i]);
		vecs[i] = Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
			pca_analysis.eigenvectors.at<double>(i, 1));
	}
	Point p1 = cnt + 0.02*Point(static_cast<int>(vecs[0].x*vals[0]), static_cast<int>(vecs[0].y*vals[0]));
	Point p2 = cnt - 0.05*Point(static_cast<int>(vecs[1].x*vals[1]), static_cast<int>(vecs[1].y*vals[1]));

	line(image, cnt, p1, Scalar(255, 0, 0), 2, 8, 0);
	line(image, cnt, p2, Scalar(255, 255, 0), 2, 8, 0);

	double angle = atan2(vecs[0].y, vecs[0].x);//三角函数求角度
	printf("angle : %.2f\n", 180 * (angle / CV_PI));
	return angle;
}