#pragma once
#include <Windows.h>

#include "thread.h"
#include "ffmpeg2.h"


#define WM_UPDATE_VIDEO WM_USER + 100



class   DecodeThread :public Thread
{
public:
	FFVideoReader   _ffReader;
	HWND            _hWnd;
	bool            _exitFlag;
public:
	DecodeThread();

	void    exitThread();
	/**
	*   加载文件
	*/
	virtual void    load(const char* fileName);
	/**
	*   线程执行函数
	*/
	virtual bool    run();
};