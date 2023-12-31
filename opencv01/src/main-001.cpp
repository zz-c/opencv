#include<opencv2\opencv.hpp>

using namespace cv;
int main(int argc, char** argv)
{
	// read image
	Mat image = imread("test.jpg");
	if (image.empty()) {
		printf("could not load image...\n");
		return -1;
	}

	// 对图像进行所有像素用 （255- 像素值）
	Mat invertImage;
	image.copyTo(invertImage);

	// 获取图像宽、高
	int channels = image.channels();
	int rows = image.rows;
	int cols = image.cols * channels;
	if (image.isContinuous()) {
		cols *= rows;         
		rows = 1;
	}

	// 每个像素点的每个通道255取反
	uchar* p1;
	uchar* p2;
	for (int row = 0; row < rows; row++) {
		p1 = image.ptr<uchar>(row);// 获取像素指针
		p2 = invertImage.ptr<uchar>(row);
		for (int col = 0; col < cols; col++) {
			*p2 = 255 - *p1; // 取反
			p2++;
			p1++;
		}
	}

	// create windows
	namedWindow("My Test", WINDOW_AUTOSIZE);
	namedWindow("My Invert Image", WINDOW_AUTOSIZE);

	// display image
	imshow("My Test", image);
	imshow("My Invert Image", invertImage);

	// 关闭
	waitKey(0);
	destroyWindow("My Test");
	destroyWindow("My Invert Image");
	return 0;
}