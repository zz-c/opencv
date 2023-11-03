#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Windows.h>

int main(int argc, char* argv[]) {
    HWND hwndDesktop = GetDesktopWindow();
    HDC hdcScreen = GetDC(hwndDesktop);
    // 获取屏幕分辨率
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    // 创建设备兼容的位图DC
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
    // 将位图对象选入位图DC中
    SelectObject(hdcMem, hBitmap);
    // 将屏幕内容复制到位图DC中
    BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY);
    // 将位图转换为OpenCV图像
    cv::Mat targetImage8UC4(screenHeight, screenWidth, CV_8UC4);
    GetBitmapBits(hBitmap, screenHeight * screenWidth * 4, targetImage8UC4.data);
    // 释放资源
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(hwndDesktop, hdcScreen);
    // 显示结果图像
    //cv::imshow("Result", targetImage);

    cv::Mat targetImage;
    cv::cvtColor(targetImage8UC4, targetImage, cv::COLOR_BGRA2BGR);
    cv::Mat templateImage = cv::imread("G:/project/c/opencv/opencvtest/source/template.png", cv::IMREAD_COLOR);
    if (templateImage.empty()) {
        std::cerr << "无法读取图像文件" << std::endl;
        return -1;
    }
    //cv::imshow("targetImage", targetImage);

    // 创建结果图像
    cv::Mat resultImage;
    int resultCols = targetImage.cols - templateImage.cols + 1;
    int resultRows = targetImage.rows - templateImage.rows + 1;
    resultImage.create(resultRows, resultCols, CV_32FC1);
    // 进行模板匹配
    cv::matchTemplate(targetImage, templateImage, resultImage, cv::TM_CCOEFF_NORMED);
    // 定位最佳匹配位置
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(resultImage, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
    cv::rectangle(targetImage, maxLoc, cv::Point(maxLoc.x + templateImage.cols, maxLoc.y + templateImage.rows), cv::Scalar(0, 255, 0), 2);
    //cv::imshow("Result", targetImage);

    //键鼠操作
    SetCursorPos(maxLoc.x, maxLoc.y);
    mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);


    cv::waitKey(0);

    return 0;
}