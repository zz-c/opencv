#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace cv::ml;
using namespace std;

int main(int argc, char** argv) {
	Mat img = Mat::zeros(500, 500, CV_8UC3);
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
	Mat points(sampleCount, 2, CV_32FC1);
	Mat labels;

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
	Ptr<EM> em_model = EM::create();
	em_model->setClustersNumber(numCluster);
	em_model->setCovarianceMatrixType(EM::COV_MAT_SPHERICAL);
	em_model->setTermCriteria(TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 100, 0.1));
	em_model->trainEM(points, noArray(), labels, noArray());

	// classify every image pixels
	Mat sample(1, 2, CV_32FC1);
	for (int row = 0; row < img.rows; row++) {
		for (int col = 0; col < img.cols; col++) {
			sample.at<float>(0) = (float)col;
			sample.at<float>(1) = (float)row;
			int response = cvRound(em_model->predict2(sample, noArray())[1]);
			Scalar c = colorTab[response];
			circle(img, Point(col, row), 1, c*0.75, -1);
		}
	}

	// draw the clusters
	for (int i = 0; i < sampleCount; i++) {
		Point p(cvRound(points.at<float>(i, 0)), points.at<float>(i, 1));
		circle(img, p, 1, colorTab[labels.at<int>(i)], -1);
	}

	imshow("GMM-EM Demo", img);

	waitKey(0);
	return 0;
}