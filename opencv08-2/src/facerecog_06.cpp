#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <iostream>

using namespace cv;
using namespace cv::face;
using namespace std;

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
	//Ptr<LBPHFaceRecognizer> model = createLBPHFaceRecognizer();
	Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
	model->train(images, labels);

	// recognition face
	int predictedLabel = model->predict(testSample);
	printf("actual label : %d, predict label :  %d\n", testLabel, predictedLabel);

	// print parameters
	int radius = model->getRadius();
	int neibs = model->getNeighbors();
	int grad_x = model->getGridX();
	int grad_y = model->getGridY();
	double t = model->getThreshold();

	printf("radius : %d\n", radius);
	printf("neibs : %d\n", neibs);
	printf("grad_x : %d\n", grad_x);
	printf("grad_y : %d\n", grad_y);
	printf("threshold : %.2f\n", t);

	waitKey(0);
	return 0;
}