#include <opencv2/opencv.hpp>

char* face_cascade_name = "E:\\clib\\opencv-3.4.10-src\\opencv-3.4.10\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
char* eyes_cascade_name = "E:\\clib\\opencv-3.4.10-src\\opencv-3.4.10\\data\\haarcascades\\haarcascade_eye.xml";


void faceRecongize(cv::CascadeClassifier faceCascade, cv::CascadeClassifier eyesCascade, cv::Mat& frame);

int main(){
	int duplWidth = 1920;
	int duplHeight = 1080;
	long mBufferSize = duplWidth * duplHeight * 4;
	uint8_t* mBuffer = new uint8_t[mBufferSize];
	for (int i = 0; i < mBufferSize;i++) {
		mBuffer[i] = i%255;
	}

	std::string fname = "opencv.jpg";

	cv::Mat bgra_image(duplHeight, duplWidth, CV_8UC4, mBuffer);
	cv::imwrite(fname, bgra_image);//LNK2019 无法解析的外部符号 d是debug版本，如果是release版本，就不需要加d
	//窗口进行展示
	//imshow("face", bgra_image);

	cv::CascadeClassifier faceCascade;
	cv::CascadeClassifier eyesCascade;
	// 加载脸部分类器文件
	if (!faceCascade.load(face_cascade_name)) {
		std::cout << "load face_cascade_name failed. " << std::endl;
		return -1;
	}
	// 加载眼睛部分分类器文件
	if (!eyesCascade.load(eyes_cascade_name)) {
		std::cout << "load eyes_cascade_name failed. " << std::endl;
		return -1;
	}

	cv::VideoCapture* videoCap = new cv::VideoCapture;
	// 打开摄像机
	videoCap->open(0);
	if (!videoCap->isOpened()) {
		videoCap->release();
		std::cout << "open camera failed" << std::endl;
		return -1;
	}
	std::cout << "open camera success" << std::endl;
	while (1) {
		cv::Mat frame;
		//读取视频帧
		videoCap->read(frame);

		if (frame.empty()) {
			videoCap->release();
			return -1;
		}

		//进行人脸识别
		faceRecongize(faceCascade, eyesCascade, frame);

		//窗口进行展示
		imshow("face", frame);

		//等待回车键按下退出程序
		if (cv::waitKey(30) == 13) {
			cv::destroyAllWindows();
			return 0;
		}
	}


	system("pause");
	return 0;
}

void faceRecongize(cv::CascadeClassifier faceCascade, cv::CascadeClassifier eyesCascade, cv::Mat& frame) {
	std::vector<cv::Rect> faces;

	// 检测人脸
	faceCascade.detectMultiScale(frame, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
	for (int i = 0; i < faces.size(); i++) {

		// 用椭圆画出人脸部分
		cv::Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, cv::Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);

		cv::Mat faceROI = frame(faces[i]);
		std::vector<cv::Rect> eyes;

		// 检测眼睛
		eyesCascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
		for (int j = 0; j < eyes.size(); j++)
		{
			// 用圆画出眼睛部分
			cv::Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
			int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
			circle(frame, eye_center, radius, cv::Scalar(255, 0, 0), 4, 8, 0);
		}

		cv::Mat mouthROI = frame(faces[i]);
		std::vector<cv::Rect> mouth;



		// 检测到两个眼睛 可认为检测到有效人脸
		if (eyes.size() >= 2) {

			// 人脸上方区域写字进行标识
			cv::Point centerText(faces[i].x + faces[i].width / 2 - 40, faces[i].y - 20);
			cv::putText(frame, "face", centerText, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
		}

	}
}
