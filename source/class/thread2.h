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
	*   �߳���ں���
	*/
	static  DWORD    CALLBACK threadEnter(void* pVoid);
public:
	Thread();

	virtual ~Thread();

	virtual bool    run();
	/**
	*   �����̺߳���
	*/
	virtual bool    start();
	/**
	*   �ȴ��˳�����
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
	*   �����ļ�
	*/
	virtual void    load(const char* fileName);

	virtual void    join();
	/**
	*   �߳�ִ�к���
	*/
	virtual bool    run();
};