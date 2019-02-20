#pragma once

#include "thread2.h"


DWORD    CALLBACK Thread::threadEnter(void* pVoid)
{
	Thread* pThis = (Thread*)pVoid;
	if (pThis)
	{
		pThis->run();
	}
	return  0;
}

Thread::Thread()
{
	_thread = 0;
	_threadId = 0;
}
Thread::~Thread()
{
	join();
}

bool    Thread::run()
{
	return  true;
}
/**
*   启动线程函数
*/
bool    Thread::start()
{
	if (_thread != 0)
	{
		return  false;
	}
	else
	{
		//HIGH_PRIORITY_CLASS
		_thread = CreateThread(0, 0, &Thread::threadEnter, this, 0, &_threadId);
		return  true;
	}
}
/**
*   等待退出函数
*/
void    Thread::join()
{
	if (_thread)
	{
		WaitForSingleObject(_thread, 0xFFFFFFFF);
		CloseHandle(_thread);
		_thread = 0;
	}
}





DecodeThread::DecodeThread()
{
	_exitFlag = false;
	_hWnd = 0;
}

void    DecodeThread::setup(HWND hwnd, const char* fileName)
{
	_hWnd = hwnd;
	_ffReader.setup();
	_ffReader.load(fileName);

	HDC     hDC = GetDC(hwnd);
	_hMem = ::CreateCompatibleDC(hDC);


	BITMAPINFO	bmpInfor;
	bmpInfor.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfor.bmiHeader.biWidth = _ffReader._screenW;
	bmpInfor.bmiHeader.biHeight = -_ffReader._screenH;
	bmpInfor.bmiHeader.biPlanes = 1;
	bmpInfor.bmiHeader.biBitCount = 24;
	bmpInfor.bmiHeader.biCompression = BI_RGB;
	bmpInfor.bmiHeader.biSizeImage = 0;
	bmpInfor.bmiHeader.biXPelsPerMeter = 0;
	bmpInfor.bmiHeader.biYPelsPerMeter = 0;
	bmpInfor.bmiHeader.biClrUsed = 0;
	bmpInfor.bmiHeader.biClrImportant = 0;

	_hBmp = CreateDIBSection(hDC, &bmpInfor, DIB_RGB_COLORS, (void**)&_imageBuf, 0, 0);
	SelectObject(_hMem, _hBmp);
}
/**
*   加载文件
*/
void    DecodeThread::load(const char* fileName)
{
	_ffReader.load(fileName);
}

void    DecodeThread::join()
{
	_exitFlag = true;
	Thread::join();
}
/**
*   线程执行函数
*/
bool    DecodeThread::run()
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
		BYTE*   data = (BYTE*)infor._data;
		for (int i = 0; i < infor._dataSize; i += 3)
		{
			_imageBuf[i + 0] = data[i + 2];
			_imageBuf[i + 1] = data[i + 1];
			_imageBuf[i + 2] = data[i + 0];
		}
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
