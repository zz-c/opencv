#include <windows.h>
#include <stdio.h>
#include <string.h>

// 回调函数，枚举窗口时调用
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    char windowTitle[256];
    // 获取窗口标题
    if (GetWindowText(hwnd, windowTitle, sizeof(windowTitle))) {
        // 检查标题是否包含“记事本”
        if (strstr(windowTitle, "记事本") != NULL) {
            printf("句柄: %p, 标题: %s\n", hwnd, windowTitle);
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

// 发送一个字符到窗口
// 发送一个字符到窗口
void SendCharacterToWindow(HWND hwnd, char ch) {
    // 设置目标窗口为前台窗口
    SetForegroundWindow(hwnd);
    // 准备输入结构
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wScan = 0;
    input.ki.time = 0;
    input.ki.dwExtraInfo = 0;
    // 设置按下的键
    input.ki.wVk = VkKeyScan(ch); // 使用VkKeyScan转换字符到虚拟键码
    input.ki.dwFlags = 0; // 按下键
    SendInput(1, &input, sizeof(INPUT));
    // 设置释放的键
    input.ki.dwFlags = KEYEVENTF_KEYUP; // 释放键
    SendInput(1, &input, sizeof(INPUT));
}


// 发送一个字符到窗口
// 设置窗口文本
void SetWindowTextToNotepad(HWND hwnd, const char* text) {
    SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)text);
}
int main() {
    HWND notepadHwnd = GetNotepadWindowHandle();
    if (notepadHwnd == NULL) {
        printf("无法找到记事本窗口。\n");
        return 1;
    }

    // 要输入的字符串
    const char* text = "Hello, Notepad!";
    size_t length = strlen(text);

    // 每秒输入一个字符
    for (size_t i = 0; i < length; ++i) {
        SendCharacterToWindow(notepadHwnd, text[i]);
        Sleep(1000); // 每秒发送一个字符
    }

    printf("输入完成。\n");
    return 0;
}
