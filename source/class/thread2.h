#pragma once

#include <windows.h>
#include "ffmpeg2.h"
class   Thread
{
public:
	DWORD               _threadId;
	HANDLE              _thread;
protected:

	/**
	*   线程入口函数
	*/
	static  DWORD    CALLBACK threadEnter(void* pVoid);
public:
	Thread();

	virtual ~Thread();

	virtual bool    run();
	/**
	*   启动线程函数
	*/
	virtual bool    start();
	/**
	*   等待退出函数
	*/
	virtual void    join();
};


class   DecodeThread :public Thread
{
public:
	FFVideoReader   _ffReader;
	HWND            _hWnd;
	BYTE*           _imageBuf;
	HDC             _hMem;
	HBITMAP	        _hBmp;
	bool            _exitFlag;
public:
	DecodeThread();

	virtual void    setup(HWND hwnd, const char* fileName = "12.mp4");

	/**
	*   加载文件
	*/
	virtual void    load(const char* fileName);

	virtual void    join();
	/**
	*   线程执行函数
	*/
	virtual bool    run();
};