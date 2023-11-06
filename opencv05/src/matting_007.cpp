#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

int numRun = 0;
Rect rect;
bool init = false;
Mat src, image;
Mat mask, bgModel, fgModel;
const char* winTitle = "input image";

void onMouse(int event, int x, int y, int flags, void* param);
void setROIMask();
void showImage();
void runGrabCut();
int main(int argc, char** argv) {
	src = imread("G:/project/c/opencv/opencv05/source/flower.png", 1);
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	mask.create(src.size(), CV_8UC1);
	mask.setTo(Scalar::all(GC_BGD));//Ä¬ÈÏÈ«ÊÇ±³¾°

	namedWindow(winTitle, WINDOW_AUTOSIZE);
	setMouseCallback(winTitle, onMouse, 0);
	imshow(winTitle, src);

	while (true) {
		printf("waitKey...\n");
		char c = (char)waitKey(0);
		printf("key : %c\n", c);
		if (c == 'n') {
			runGrabCut();
			numRun++;
			showImage();
			printf("current iteative times : %d\n", numRun);
		}
		if ((int)c == 27) {
			break;
		}
	}

	waitKey(0);
	return 0;
}

void showImage() {
	Mat result, binMask;
	binMask.create(mask.size(), CV_8UC1);
	binMask = mask & 1;
	if (init) {
		src.copyTo(result, binMask);
	} else {
		src.copyTo(result);
	}
	rectangle(result, rect, Scalar(0, 0, 255), 2, 8);
	imshow(winTitle, result);
}

void setROIMask() {
	// GC_FGD = 1
	// GC_BGD =0;
	// GC_PR_FGD = 3
	// GC_PR_BGD = 2
	mask.setTo(GC_BGD);
	rect.x = max(0, rect.x);
	rect.y = max(0, rect.y);
	rect.width = min(rect.width, src.cols - rect.x);
	rect.height = min(rect.height, src.rows - rect.y);
	mask(rect).setTo(Scalar(GC_PR_FGD));
}

void onMouse(int event, int x, int y, int flags, void* param) {
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		rect.x = x;
		rect.y = y;
		rect.width = 1;
		rect.height = 1;
		init = false;
		numRun = 0;
		break;
	case EVENT_MOUSEMOVE:
		if (flags & EVENT_FLAG_LBUTTON) {
			rect = Rect(Point(rect.x, rect.y), Point(x, y));
			showImage();
		}
		break;
	case EVENT_LBUTTONUP:
		if (rect.width > 1 && rect.height > 1) {
			setROIMask();
			showImage();
		}
		break;
	default:
		break;
	}
}

void runGrabCut() {
	if (rect.width < 2 || rect.height < 2) {
		return;
	}
	
	if (init) {
		grabCut(src, mask, rect, bgModel, fgModel, 1);
	} {
		grabCut(src, mask, rect, bgModel, fgModel, 1, GC_INIT_WITH_RECT);
		init = true;
	}
}