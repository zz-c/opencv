#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace cv::face;
using namespace std;
/**
EigenFaceRecognizer:
原理： 使用主成分分析（PCA）降维的方法。它基于主成分分析，通过找到训练集中人脸图像的主要特征来进行人脸识别。
优点： 对于较大的人脸数据库，具有较好的识别效果。
缺点： 对于光照、表情等变化较为敏感。
FisherFaceRecognizer:
原理： 使用线性判别分析（LDA）进行降维。与EigenFaceRecognizer不同，FisherFaceRecognizer寻找的是最大化类间距离和最小化类内距离的特征向量。
优点： 在处理较小的人脸数据库时，相对于EigenFaceRecognizer更有优势，且对于光照、表情等变化具有一定的稳健性。
缺点： 对于较大的人脸数据库，可能会过拟合。
**/
int main(int argc, char** argv) {
	vector<Mat> images;
	vector<int> labels;
	for (int i = 1; i <= 40; i++) {
		for (int j = 1; j <= 10; j++) {
			String path = "E:/clib/data//orl_faces/s" + std::to_string(i) + "/" + std::to_string(j) + ".pgm";
			printf("path : %s,%d\n", path.c_str(), i);
			images.push_back(imread(path, 0));
			labels.push_back(i);
		}
	}

	if (images.size() < 1 || labels.size() < 1) {
		printf("invalid image path...\n");
		return -1;
	}

	int height = images[0].rows;
	int width = images[0].cols;
	printf("height : %d, width : %d\n", height, width);

	Mat testSample = images[images.size() - 1];
	int testLabel = labels[labels.size() - 1];
	images.pop_back();
	labels.pop_back();

	// train it
	//Ptr<BasicFaceRecognizer> model = EigenFaceRecognizer::create();
	Ptr<BasicFaceRecognizer> model = FisherFaceRecognizer::create();
	model->train(images, labels);

	// recognition face
	int predictedLabel = model->predict(testSample);
	printf("actual label : %d, predict label :  %d\n", testLabel, predictedLabel);

	Mat eigenvalues = model->getEigenValues();
	Mat W = model->getEigenVectors();
	Mat mean = model->getMean();
	Mat meanFace = mean.reshape(1, height);
	Mat dst;
	if (meanFace.channels() == 1) {
		normalize(meanFace, dst, 0, 255, NORM_MINMAX, CV_8UC1);
	}
	else if (meanFace.channels() == 3) {
		normalize(meanFace, dst, 0, 255, NORM_MINMAX, CV_8UC3);
	}
	imshow("Mean Face", dst);

	// show eigen faces
	for (int i = 0; i < min(16, W.cols); i++) {
		Mat ev = W.col(i).clone();
		Mat grayscale;
		Mat eigenFace = ev.reshape(1, height);
		if (eigenFace.channels() == 1) {
			normalize(eigenFace, grayscale, 0, 255, NORM_MINMAX, CV_8UC1);
		}
		else if (eigenFace.channels() == 3) {
			normalize(eigenFace, grayscale, 0, 255, NORM_MINMAX, CV_8UC3);
		}
		Mat colorface;
		applyColorMap(grayscale, colorface, COLORMAP_BONE);
		char* winTitle = new char[128];
		sprintf(winTitle, "eigenface_%d", i);
		imshow(winTitle, colorface);
	}

	// 重建人脸
	for (int num = 0; num < min(W.cols, 16); num++) {
		Mat evs = W.col(num);
		Mat projection = LDA::subspaceProject(evs, mean, images[0].reshape(1, 1));
		Mat reconstruction = LDA::subspaceReconstruct(evs, mean, projection);

		Mat result = reconstruction.reshape(1, height);
		if (result.channels() == 1) {
			normalize(result, reconstruction, 0, 255, NORM_MINMAX, CV_8UC1);
		}
		else if (result.channels() == 3) {
			normalize(result, reconstruction, 0, 255, NORM_MINMAX, CV_8UC3);
		}
		char* winTitle = new char[128];
		sprintf(winTitle, "recon_face_%d", num);
		imshow(winTitle, reconstruction);
	}

	waitKey(0);
	return 0;
}