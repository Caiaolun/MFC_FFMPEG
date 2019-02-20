#pragma once

#include <windows.h>

class Timestamp
{
public:
	Timestamp();
	~Timestamp();

	void    update();
	/**
	*   ��ȡ��ǰ��
	*/
	double getElapsedSecond();
	/**
	*   ��ȡ����
	*/
	double getElapsedTimeInMilliSec();
	/**
	*   ��ȡ΢��
	*/
	double getElapsedTimeInMicroSec();
protected:
	LARGE_INTEGER   _frequency;
	LARGE_INTEGER   _startCount;
};