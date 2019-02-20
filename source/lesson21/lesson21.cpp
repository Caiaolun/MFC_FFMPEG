#include <windows.h>
#include <tchar.h>

#include "FFVideoReader.hpp"
#include "Thread.hpp"
#include "Timestamp.hpp"
#include "GLContext.h"

void  getResourcePath(HINSTANCE hInstance, char pPath[1024])
{
	char    szPathName[1024];
	char    szDriver[64];
	char    szPath[1024];
	GetModuleFileNameA(hInstance, szPathName, sizeof(szPathName));
	_splitpath(szPathName, szDriver, szPath, 0, 0);
	sprintf(pPath, "%s%s", szDriver, szPath);
}

class   DecodeThread :public Thread
{
public:
	FFVideoReader   _ffReader;
	HWND            _hWnd;

	bool            _exitFlag;
	Timestamp       _timestamp;
	GLContext       _glContext;
public:
	DecodeThread()
	{
		_exitFlag = false;
		_hWnd = 0;
	}

	virtual void    setup(HWND hwnd, const char* fileName = "11.flv")
	{
		_hWnd = hwnd;
		_ffReader.setup();
		_ffReader.load(fileName);
		_glContext.setup(hwnd, GetDC(hwnd));
	}
	/**
	*   加载文件
	*/
	virtual void    load(const char* fileName)
	{
		_ffReader.load(fileName);
	}
	virtual void    shutdown()
	{
		_exitFlag = true;
		Thread::join();
		_glContext.shutdown();
	}
	/**
	*   线程执行函数
	*/
	virtual bool    run()
	{
		_timestamp.update();
		while (!_exitFlag)
		{
			FrameInfor  infor;
			if (!_ffReader.readFrame(infor))
			{
				break;
			}
			double      tims = infor._pts * infor._timeBase * 1000;
			//! 这里需要通知窗口进行重绘制更新，显示更新数据
			InvalidateRect(_hWnd, 0, 0);

			double      elsped = _timestamp.getElapsedTimeInMilliSec();
			double      sleeps = (tims - elsped);
			if (sleeps > 1)
			{
				Sleep((DWORD)sleeps);
			}
		}

		return  true;
	}

	void    render()
	{
		struct  Vertex
		{
			float   x, y;
			float   u, v;
		};

		RECT    rt;
		GetClientRect(_hWnd, &rt);
		int     w = rt.right - rt.left;
		int     h = rt.bottom - rt.top;
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1, 0, 0, 1);

		glViewport(0, 0, w, h);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity(); 
		glOrtho(0, w, h, 0, -100, 100);

		Vertex  vertexs[] =
		{
			{ 0,  0,  0,  1 },
			{ 0,  h,  0,  0 },
			{ w,  0,  1,  1 },

			{ 0,  h,  0,  0 },
			{ w,  0,  1,  1 },
			{ w,  h,  1,  0 },
		};
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &vertexs[0].x);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertexs[0].u);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		_glContext.swapBuffer();
	}
};

DecodeThread    g_decode;


LRESULT CALLBACK    windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		g_decode.render();
	}
	break;
	case WM_SIZE:
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		g_decode.shutdown();
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


	char    szPath[1024];
	char    szPathName[1024];

	getResourcePath(hInstance, szPath);

	sprintf(szPathName, "%sdata/11.flv", szPath);

	g_decode.setup(hWnd, szPathName);
	g_decode.start();

	MSG     msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	g_decode.shutdown();
	return  0;
}