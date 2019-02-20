#pragma once
#pragma once

#include <windows.h>

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