#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	Mat img(500, 500, CV_8UC3);
	RNG rng(12345);

	Scalar colorTab[] = {
		Scalar(0, 0, 255),
		Scalar(0, 255, 0),
		Scalar(255, 0, 0),
		Scalar(0, 255, 255),
		Scalar(255, 0, 255)
	};

	int numCluster = rng.uniform(2, 5);
	printf("number of clusters : %d\n", numCluster);

	int sampleCount = rng.uniform(5, 1000);
	Mat points(sampleCount, 1, CV_32FC2);
	Mat labels;
	Mat centers;

	// 生成随机数
	for (int k = 0; k < numCluster; k++) {
		Point center;
		center.x = rng.uniform(0, img.cols);
		center.y = rng.uniform(0, img.rows);
		Mat pointChunk = points.rowRange(k*sampleCount / numCluster, 
			k == numCluster - 1 ? sampleCount : (k + 1)*sampleCount / numCluster);

		rng.fill(pointChunk, RNG::NORMAL, Scalar(center.x, center.y), Scalar(img.cols*0.05, img.rows*0.05));
	}
	randShuffle(points, 1, &rng);

	// 使用KMeans
	kmeans(points, numCluster, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 0.1), 3, KMEANS_PP_CENTERS, centers);

	// 用不同颜色显示分类
	img = Scalar::all(255);
	for (int i = 0; i < sampleCount; i++) {
		int index = labels.at<int>(i);
		Point p = points.at<Point2f>(i);
		circle(img, p, 2, colorTab[index], -1, 8);
	}

	// 每个聚类的中心来绘制圆
	for (int i = 0; i < centers.rows;  i++) {
		int x = centers.at<float>(i, 0);
		int y = centers.at<float>(i, 1);
		printf("c.x= %d, c.y=%d", x, y);
		circle(img, Point(x, y), 40, colorTab[i], 1, LINE_AA);
	}

	imshow("KMeans-Data-Demo", img);
	waitKey(0);
	return 0;
}