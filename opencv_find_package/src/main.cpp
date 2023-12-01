#include<opencv2\opencv.hpp>
#include <Windows.h>

using namespace cv;
int main(int argc, char** argv)
{
    // 获取窗口的句柄（请确保游戏窗口处于活动状态）
    //HWND gameWindow = FindWindow(NULL, "日志分析.txt - 记事本");
    HWND gameWindow = FindWindow(NULL, "计算器");

    if (gameWindow == NULL) {
        std::cerr << "无法找到窗口" << std::endl;
        return -1;
    }

    // 获取窗口的矩形区域
    RECT gameRect;
    GetWindowRect(gameWindow, &gameRect);

    int width = gameRect.right - gameRect.left;
    int height = gameRect.bottom - gameRect.top;

    // 创建设备上下文和位图
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMemDC = CreateCompatibleDC(hdcScreen);
    HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);
    SelectObject(hdcMemDC, hbmScreen);

    // 定义和分配内存给BITMAPINFOHEADER
    BITMAPINFOHEADER bmiHeader;
    ZeroMemory(&bmiHeader, sizeof(BITMAPINFOHEADER));
    bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmiHeader.biWidth = width;
    bmiHeader.biHeight = -height;
    bmiHeader.biPlanes = 1;
    bmiHeader.biBitCount = 32;
    bmiHeader.biCompression = BI_RGB;
    bmiHeader.biSizeImage = 0;
    bmiHeader.biXPelsPerMeter = 0;
    bmiHeader.biYPelsPerMeter = 0;
    bmiHeader.biClrUsed = 0;
    bmiHeader.biClrImportant = 0;

    // 拷贝窗口的图像到内存上下文
    BitBlt(hdcMemDC, 0, 0, width, height, hdcScreen, gameRect.left, gameRect.top, SRCCOPY);

    // 创建一个OpenCV Mat对象
    cv::Mat screenshot(height, width, CV_8UC4);

    // 使用GetDIBits将位图数据复制到Mat
    GetDIBits(hdcScreen, hbmScreen, 0, height, screenshot.data, (BITMAPINFO*)&bmiHeader, DIB_RGB_COLORS);

    imshow("My Test2", screenshot);

    // 将图像保存为文件
    //cv::cvtColor(screenshot, screenshot, cv::COLOR_RGBA2BGR);
    //cv::imwrite("screenshot.png", screenshot);

    // 释放资源
    DeleteObject(hbmScreen);
    DeleteDC(hdcMemDC);
    ReleaseDC(NULL, hdcScreen);
	// 关闭
	waitKey(0);
	return 0;
}