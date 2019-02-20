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
	_imageBuf = 0;
	_hMem = 0;
	_hBmp = 0;
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
*   �����̺߳���
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
*   �ȴ��˳�����
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
	_ffReader.setup();
	_hWnd = 0;
	_exitFlag = false;
}

void    DecodeThread::exitThread()
{
	_exitFlag = true;
	join();
}
/**
*   �����ļ�
*/
void    DecodeThread::load(const char* fileName)
{
	_ffReader.load(fileName);
}
/**
*   �߳�ִ�к���
*/
bool    DecodeThread::run()
{
	while (!_exitFlag)
	{
		FrameInfor  infor;
		if (!_ffReader.readFrame(infor))
		{
			break;
		}

		BYTE*   data = (BYTE*)infor._data;
		for (int i = 0; i < infor._dataSize; i += 3)
		{
			_imageBuf[i + 0] = data[i + 2];
			_imageBuf[i + 1] = data[i + 1];
			_imageBuf[i + 2] = data[i + 0];
		}
		InvalidateRect(_hWnd, 0, 0);
	}

	return  true;
}
