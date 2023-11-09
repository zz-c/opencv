#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace cv::dnn;
using namespace std;

//opencv-4.8.0\samples\data\dnn
//bvlc_googlenet.caffemodel模型的下载地址：http://dl.caffe.berkeleyvision.org/bvlc_googlenet.caffemodel
//bvlc_googlenet.prototxt文件下载地址：https://github.com/opencv/opencv_extra/blob/master/testdata/dnn/bvlc_googlenet.prototxt
String model_bin_file = "E:/clib/opencv-dnn/bvlc_googlenet.caffemodel";//G:/project/c/opencv/opencv07/source
String model_txt_file = "G:/project/c/opencv/opencv07/source/bvlc_googlenet.prototxt";
String labels_txt_file = "G:/project/c/opencv/opencv07/source/synset_words.txt";
vector<String> readLabels();
int main(int argc, char** argv) {
	Mat src = imread("G:/project/c/opencv/opencv07/source/cat.png");
	//Mat src = imread("E:/clib/data/student.jpg");
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	namedWindow("input image", WINDOW_AUTOSIZE);
	imshow("input image", src);
	vector<String> labels = readLabels();
	Net net = readNetFromCaffe(model_txt_file, model_bin_file);
	if (net.empty()) {
		printf("read caffe model data failure...\n");
		return -1;
	}
	Mat inputBlob = blobFromImage(src, 1.0, Size(224, 224), Scalar(104, 117, 123));
	Mat prob;
	for (int i = 0; i < 10; i++) {
		net.setInput(inputBlob, "data");//bvlc_googlenet.prototxt里
		prob = net.forward("prob");//bvlc_googlenet.prototxt里
	}
	Mat probMat = prob.reshape(1, 1);
	Point classNumber;
	double classProb;
	minMaxLoc(probMat, NULL, &classProb, NULL, &classNumber);
	int classidx = classNumber.x;
	printf("\n current image classification : %s, possible : %.2f", labels.at(classidx).c_str(), classProb);

	putText(src, labels.at(classidx), Point(20, 20), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 255), 2, 8);
	imshow("Image Classification", src);

	waitKey(0);
	return 0;
}
vector<String> readLabels() {
	vector<String> classNames;
	std::ifstream fp(labels_txt_file);
	if (!fp.is_open()) {
		printf("could not open the file");
		exit(-1);
	}
	string name;
	while (!fp.eof()) {
		getline(fp, name);
		if (name.length()) {
			classNames.push_back(name.substr(name.find(' ') + 1));
		}
	}
	fp.close();
	return classNames;
}
