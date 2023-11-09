#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>

using namespace cv;
using namespace cv::dnn;
using namespace std;

String model_text_file = "E:/clib/opencv-dnn/deploy.prototxt";
String modelFile = "E:/clib/opencv-dnn/res10_300x300_ssd_iter_140000.caffemodel";

int main(int argc, char** argv) {

	namedWindow("input", WINDOW_AUTOSIZE);

	// set up net
	Net net = readNetFromCaffe(model_text_file, modelFile);
	net.setPreferableBackend(DNN_BACKEND_OPENCV);
	net.setPreferableTarget(DNN_TARGET_CPU);

	Mat frame = imread("E:/clib/data/student.jpg");
	imshow("input", frame);
	//Ô¤²â
	Mat inputblob = blobFromImage(frame, 1.0, Size(300, 300), Scalar(104, 177, 123), false);
	net.setInput(inputblob, "data");
	Mat detection = net.forward("detection_out");

	//¼ì²â
	Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
	float confidence_threshold = 0.13;
	for (int i = 0; i < detectionMat.rows; i++) {
		float confidence = detectionMat.at<float>(i, 2);
		if (confidence > confidence_threshold) {
			size_t objIndex = (size_t)(detectionMat.at<float>(i, 1));
			float tl_x = detectionMat.at<float>(i, 3) * frame.cols;
			float tl_y = detectionMat.at<float>(i, 4) * frame.rows;
			float br_x = detectionMat.at<float>(i, 5) * frame.cols;
			float br_y = detectionMat.at<float>(i, 6) * frame.rows;

			Rect object_box((int)tl_x, (int)tl_y, (int)(br_x - tl_x), (int)(br_y - tl_y));
			rectangle(frame, object_box, Scalar(0, 0, 255), 2, 8, 0);
			//putText(frame, format("%s %.2f", "face", confidence), Point(tl_x, tl_y), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 0, 0), 2);
		}
	}
	//vector < double>layerstimings;
	//double freq = getTickFrequency() / 1000;
	//double time = net.getPerfProfile(layerstimings) / freq;
	//ostringstream ss;
	//ss << "FPS" << 1000 / time << ";time:" << time << "ms";
	//putText(frame, ss.str(), Point(20, 20), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 2, 8);
	imshow("face-detection-demo", frame);

	waitKey(0);
	return 0;
}