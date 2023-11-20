#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <iostream>

using namespace cv;
using namespace cv::face;
using namespace std;

string haar_face_datapath = "E:/clib/opencv-4.8.0/etc/haarcascades/haarcascade_frontalface_alt_tree.xml";
int main(int argc, char** argv) {
	vector<Mat> images;
	vector<int> labels;
	for (int i = 1; i <= 2; i++) {
		for (int j = 1; j <= 2; j++) {
			String path = "F:/tmp/s" + std::to_string(i) + "/" + std::to_string(j) + ".jpg";
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
	//Ptr<BasicFaceRecognizer> model = createEigenFaceRecognizer();
	Ptr<BasicFaceRecognizer> model = EigenFaceRecognizer::create();
	model->train(images, labels);

	// recognition face
	int predictedLabel = model->predict(testSample);
	printf("actual label : %d, predict label :  %d\n", testLabel, predictedLabel);

	CascadeClassifier faceDetector;
	faceDetector.load(haar_face_datapath);

	VideoCapture capture(0);
	if (!capture.isOpened()) {
		printf("could not open camera...\n");
		return -1;
	}
	Mat frame;
	namedWindow("face-recognition", WINDOW_AUTOSIZE);
	vector<Rect> faces;
	Mat dst;
	while (capture.read(frame)) {
		flip(frame, frame, 1);
		faceDetector.detectMultiScale(frame, faces, 1.1, 1, 0, Size(80, 100), Size(380, 400));
		for (int i = 0; i < faces.size(); i++) {
			Mat roi = frame(faces[i]);
			cvtColor(roi, dst, COLOR_BGR2GRAY);
			resize(dst, testSample, testSample.size());
			int label = model->predict(testSample);
			rectangle(frame, faces[i], Scalar(255, 0, 0), 2, 8, 0);
			putText(frame, format("i'm %s", (label == 1 ? "zz" : "Unknow")), faces[i].tl(), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 255), 2, 8);
		}

		imshow("face-recognition", frame);
		char c = waitKey(10);
		if (c == 27) {
			break;
		}
	}

	waitKey(0);
	return 0;
}