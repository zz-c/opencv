#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char* argv[]) {
    // 读取目标图像和模板图像
    cv::Mat targetImage = cv::imread("G:/project/c/opencv/opencvtest/source/target.png", cv::IMREAD_COLOR);
    cv::Mat templateImage = cv::imread("G:/project/c/opencv/opencvtest/source/template.png", cv::IMREAD_COLOR);

    if (targetImage.empty() || templateImage.empty()) {
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
    cv::imshow("resultImage", resultImage);
    // 定位最佳匹配位置
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(resultImage, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

    // 绘制矩形框显示匹配结果
    cv::rectangle(targetImage, maxLoc, cv::Point(maxLoc.x + templateImage.cols, maxLoc.y + templateImage.rows), cv::Scalar(0, 255, 0), 2);
    //cv::rectangle(targetImage, minLoc, cv::Point(minLoc.x + templateImage.cols, minLoc.y + templateImage.rows), cv::Scalar(255, 0, 0), 2);

    // 保存结果图像
    //cv::imwrite("result.jpg", targetImage);

    // 显示结果图像
    cv::imshow("Result", targetImage);
    cv::waitKey(0);

    return 0;
}