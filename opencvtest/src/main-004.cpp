#include <windows.h>
#include <opencv2/opencv.hpp>
#include <stdio.h>

// 回调函数，枚举窗口时调用
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    char windowTitle[256];
    // 获取窗口标题
    if (GetWindowText(hwnd, windowTitle, sizeof(windowTitle))) {
        // 检查标题是否包含“记事本”
        if (strstr(windowTitle, "画图") != NULL) {
            // 找到目标窗口，保存句柄并停止枚举
            HWND* pHwnd = (HWND*)lParam;
            *pHwnd = hwnd;
            return FALSE; // 停止枚举
        }
    }
    return TRUE; // 继续枚举
}

// 获取记事本窗口句柄
HWND GetNotepadWindowHandle() {
    HWND notepadHwnd = NULL;
    EnumWindows(EnumWindowsProc, (LPARAM)&notepadHwnd);
    return notepadHwnd;
}

// 截图窗口
cv::Mat CaptureWindow(HWND hwnd) {

    // 获取窗口的矩形区域
    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);

    int width = windowRect.right - windowRect.left;
    int height = windowRect.bottom - windowRect.top;

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
    BitBlt(hdcMemDC, 0, 0, width, height, hdcScreen, windowRect.left, windowRect.top, SRCCOPY);
    // 创建一个OpenCV Mat对象
    cv::Mat screenshot(height, width, CV_8UC4);
    // 使用GetDIBits将位图数据复制到Mat
    GetDIBits(hdcScreen, hbmScreen, 0, height, screenshot.data, (BITMAPINFO*)&bmiHeader, DIB_RGB_COLORS);
    // 释放资源
    DeleteObject(hbmScreen);
    DeleteDC(hdcMemDC);
    ReleaseDC(NULL, hdcScreen);

    return screenshot;
}

int main() {
    HWND notepadHwnd = GetNotepadWindowHandle();
    if (notepadHwnd == NULL) {
        printf("无法找到记事本窗口。\n");
        return 1;
    }

    while (true) {
        int key = cv::waitKey(30);
        if (key == 'q' || key == 'Q') {
            break;
        }
        cv::Mat img = CaptureWindow(notepadHwnd);
        cv::imshow("Captured Image", img);
    }

    return 0;
}
