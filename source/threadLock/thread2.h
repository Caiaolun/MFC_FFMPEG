#pragma once



#include <windows.h>
#include "ffmpeg2.h"
class   Thread
{
public:
	DWORD               _threadId;
	HANDLE              _thread;

	BYTE*           _imageBuf;
	HDC             _hMem;
	HBITMAP	        _hBmp;
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
	bool            _exitFlag;
public:
	DecodeThread();

	void    exitThread();
	/**
	*   �����ļ�
	*/
	virtual void    load(const char* fileName);
	/**
	*   �߳�ִ�к���
	*/
	virtual bool    run();
};