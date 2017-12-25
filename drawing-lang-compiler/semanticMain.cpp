#include <cstdio>
#include"semantic.hpp"

#define MAX_CHARS 200

HDC hDC;                                                //窗口句柄，全局变量
char SrcFilePath[MAX_CHARS];                            //用于存放源程序文件路径
static char Name[] = "Compiler";                          //窗口名

static bool PrepareWindow(HINSTANCE, HINSTANCE, int); //----------初始化窗口函数声明

static bool CheckSrcFile(LPSTR); //----------检查源程序文件是否合法函数声明

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //----------窗口消息处理函数声明

//----------Window程序主函数
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

//保存原文件路径
    strcpy(SrcFilePath, "test.txt");

//初始化窗口
    if (!PrepareWindow(hInstance, hPrevInstance, nCmdShow)) {
        MessageBox(nullptr, "Window Initialize failed !", "ERROR !", MB_OK);
        return 1;
    }

//检查要分析的源程序文件
    if (!CheckSrcFile(SrcFilePath))
        return 1;

    Parser(SrcFilePath);

//进入window消息循环
    MSG Msg;
    while (GetMessage(&Msg, nullptr, 0, 0)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}

//----------初始化窗口函数实现
bool PrepareWindow(HINSTANCE hInst, HINSTANCE hPrevInstance, int nCmdShow) {
    HWND hWnd;
    WNDCLASS W;
    memset(&W, 0, sizeof(WNDCLASS));
    W.style = CS_HREDRAW | CS_VREDRAW;
    W.lpfnWndProc = WndProc;
    W.hInstance = hInst;
    W.hCursor = LoadCursor(nullptr, IDC_ARROW);
    W.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    W.lpszClassName = Name;
    RegisterClass(&W);

    hWnd = CreateWindow(Name, Name, WS_OVERLAPPEDWINDOW, 10, 10, 740, 490, nullptr, nullptr, hInst,
                        nullptr);
    if (hWnd == nullptr)
        return false;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    SetCursor(LoadCursor(hInst, IDC_ARROW));

    hDC = GetDC(hWnd);
    return true;
}

//----------检查源程序文件是否合法函数实现
bool CheckSrcFile(LPSTR lpszCmdParam) {
    FILE *file = nullptr;

    if (strlen(lpszCmdParam) == 0) {
        MessageBox(nullptr, "Source File Not Specified ", "ERROR", MB_OK);
        return false;
    }
    if ((file = fopen(lpszCmdParam, "r")) == nullptr) {
        MessageBox(nullptr, "Open Source File Error !", "ERROR", MB_OK);
        MessageBox(nullptr, lpszCmdParam, "Filename", MB_OK);
        return false;
    } else
        fclose(file);
    return true;
}

//----------窗口处理函数实现
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch (Message) {
        case WM_DESTROY :
            ReleaseDC(hWnd, hDC);
            PostQuitMessage(0);
            return 0;
//break;
        case WM_PAINT:
            PAINTSTRUCT pt;
            BeginPaint(hWnd, &pt);
            Parser(SrcFilePath);
            EndPaint(hWnd, &pt);
        default :
            return DefWindowProc(hWnd, Message, wParam, lParam);
    }
}

