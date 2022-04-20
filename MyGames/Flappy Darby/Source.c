#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

LPRECT rctScr;
HDC mainDC;

void Redraw()
{
    HDC memDC = CreateCompatibleDC(mainDC);
    HBITMAP memBM = CreateCompatibleBitmap(mainDC, rctScr->right - rctScr->left, rctScr->bottom - rctScr->top);

    SelectObject(memDC, memBM);
    SelectObject(memDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(memDC, RGB(255, 255, 255));
    SelectObject(memDC, GetStockObject(DC_PEN));
    SetDCPenColor(memDC, RGB(255, 255, 255));
    Rectangle(memDC, 0, 0, 1920, 1080);

    // Paint here

    BitBlt(mainDC, 0, 0, rctScr->right - rctScr->left, rctScr->bottom - rctScr->top, memDC, 0, 0, SRCCOPY);
    DeleteDC(memDC);
    DeleteObject(memBM);
}

LRESULT WINAPI wndProcessor(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    if (Msg == WM_DESTROY)
        PostQuitMessage(0);
    else if (Msg == WM_KEYDOWN)
    {

    }
    else if (Msg == WM_CHAR)
    {

    }
    else if (Msg == WM_SIZE)
    {

    }
    else if (Msg == WM_MOVE)
    {

    }
    else if (Msg == WM_LBUTTONDOWN)
    {

    }
    else if (Msg == WM_MOUSEMOVE)
    {

    }
    else if (Msg == WM_COMMAND)
    {

    }
    else return DefWindowProc(hWnd, Msg, wParam, lParam);
}

WNDCLASS WINAPI ClassRegister(LPCWSTR str, WNDPROC f)
{
    WNDCLASS wclass;
    memset(&wclass, 0, sizeof(WNDCLASS));
    wclass.hbrBackground =
        CreateSolidBrush(RGB(255, 255, 255));
    wclass.hIcon = LoadIcon(NULL, IDI_QUESTION);
    wclass.style = (CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW);
    wclass.hCursor = LoadCursor(NULL, IDC_HAND);
    wclass.lpfnWndProc = f;
    wclass.lpszClassName = str;
    RegisterClass(&wclass);

    return wclass;
}

//int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hpi, LPSTR cmdline, int ss)
int main()
{
    LPCWSTR myWindowClass = L"myClass";
    WNDCLASS myClass = ClassRegister(myWindowClass, wndProcessor);

    rctScr = malloc(sizeof(*rctScr));
    GetClientRect(GetDesktopWindow(), rctScr);

    HANDLE mainWindow = CreateWindow(
        myWindowClass, L"Default Window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        (rctScr->right / 2) - 300, (rctScr->bottom / 4 - 200), 600, 800,
        NULL, NULL, NULL, NULL);

    mainDC = GetDC(mainWindow);

    MSG msg;
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    free(rctScr);
}