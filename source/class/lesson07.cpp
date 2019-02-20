#include <windows.h>
#include <tchar.h>

//#include "FFVideoReader.hpp"
//#include "Thread.hpp"
#include "ffmpeg2.h"
#include "thread2.h"

#define WM_UPDATE_VIDEO WM_USER + 100


void  getResourcePath(HINSTANCE hInstance, char pPath[1024])
{
	char    szPathName[1024];
	char    szDriver[64];
	char    szPath[1024];
	GetModuleFileNameA(hInstance, szPathName, sizeof(szPathName));
	_splitpath(szPathName, szDriver, szPath, 0, 0);
	sprintf(pPath, "%s%s", szDriver, szPath);
}


DecodeThread    g_decode;

LRESULT CALLBACK    windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_UPDATE_VIDEO:
	{
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC         hdc;
		hdc = BeginPaint(hWnd, &ps);
		if (g_decode._hMem)
		{
			BITMAPINFO  bmpInfor;
			GetObject(g_decode._hBmp, sizeof(bmpInfor), &bmpInfor);
			BitBlt(hdc, 0, 0, bmpInfor.bmiHeader.biWidth, bmpInfor.bmiHeader.biHeight, g_decode._hMem, 0, 0, SRCCOPY);
		}

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_SIZE:
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		g_decode.join();
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return  DefWindowProc(hWnd, msg, wParam, lParam);
}

int     WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//  1   注册窗口类
	::WNDCLASSEXA winClass;
	winClass.lpszClassName = "FFVideoPlayer";
	winClass.cbSize = sizeof(::WNDCLASSEX);
	winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	winClass.lpfnWndProc = windowProc;
	winClass.hInstance = hInstance;
	winClass.hIcon = 0;
	winClass.hIconSm = 0;
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)(BLACK_BRUSH);
	winClass.lpszMenuName = NULL;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	RegisterClassExA(&winClass);

	//  2 创建窗口
	HWND    hWnd = CreateWindowExA(
		NULL,
		"FFVideoPlayer",
		"FFVideoPlayer",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		0,
		0,
		480,
		320,
		0,
		0,
		hInstance,
		0
	);

	UpdateWindow(hWnd);
	ShowWindow(hWnd, SW_SHOW);

	RECT    rt = { 0 };
	GetClientRect(hWnd, &rt);

	char    szPath[1024];
	char    szPathName[1024];

	getResourcePath(hInstance, szPath);

	sprintf(szPathName, "%sdata/12.mp4", szPath);

	g_decode.setup(hWnd, szPathName);
	g_decode.start();

	MSG     msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return  0;
}