#include "thread.h"

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