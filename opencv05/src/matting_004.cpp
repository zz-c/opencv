#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace cv::ml;
using namespace std;

int main(int argc, char** argv) {
	Mat src = imread("G:/project/c/opencv/opencv05/source/toux.jpg");
	if (src.empty()) {
		printf("could not load iamge...\n");
		return -1;
	}
	char* inputWinTitle = "input image";
	namedWindow(inputWinTitle, WINDOW_AUTOSIZE);
	imshow(inputWinTitle, src);

	// 初始化
	int numCluster = 3;
	const Scalar colors[] = {
		Scalar(255, 0, 0),
		Scalar(0, 255, 0),
		Scalar(0, 0, 255),
		Scalar(255, 255, 0)
	};

	int width = src.cols;
	int height = src.rows;
	int dims = src.channels();
	int nsamples = width*height;
	Mat points(nsamples, dims, CV_64FC1);
	Mat labels;
	Mat result = Mat::zeros(src.size(), CV_8UC3);

	// 图像RGB像素数据转换为样本数据 
	int index = 0;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			index = row*width + col;
			Vec3b rgb = src.at<Vec3b>(row, col);
			points.at<double>(index, 0) = static_cast<int>(rgb[0]);
			points.at<double>(index, 1) = static_cast<int>(rgb[1]);
			points.at<double>(index, 2) = static_cast<int>(rgb[2]);
		}
	}

	// EM Cluster Train
	Ptr<EM> em_model = EM::create();
	em_model->setClustersNumber(numCluster);
	em_model->setCovarianceMatrixType(EM::COV_MAT_SPHERICAL);
	em_model->setTermCriteria(TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 100, 0.1));
	em_model->trainEM(points, noArray(), labels, noArray());

	// 对每个像素标记颜色与显示
	Mat sample(dims, 1, CV_64FC1);
	double time = getTickCount();
	int r = 0, g = 0, b = 0;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			index = row*width + col;
			int label = labels.at<int>(index, 0);
			Scalar c = colors[label];
			result.at<Vec3b>(row, col)[0] = c[0];
			result.at<Vec3b>(row, col)[1] = c[1];
			result.at<Vec3b>(row, col)[2] = c[2];

			//b = src.at<Vec3b>(row, col)[0];
			//g = src.at<Vec3b>(row, col)[1];
			//r = src.at<Vec3b>(row, col)[2];
			//sample.at<double>(0) = b;
			//sample.at<double>(1) = g;
			//sample.at<double>(2) = r;
			//int response = cvRound(em_model->predict2(sample, noArray())[1]);
			//Scalar c = colors[response];
			//result.at<Vec3b>(row, col)[0] = c[0];
			//result.at<Vec3b>(row, col)[1] = c[1];
			//result.at<Vec3b>(row, col)[2] = c[2];

		}
	}
	printf("execution time(ms) : %.2f\n", (getTickCount() - time)/getTickFrequency()*1000);
	imshow("EM-Segmentation", result);

	waitKey(0);
	return 0;
}