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