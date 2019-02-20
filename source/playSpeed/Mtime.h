#pragma once

#include <windows.h>

class Timestamp
{
public:
	Timestamp();
	~Timestamp();

	void    update();
	/**
	*   获取当前秒
	*/
	double getElapsedSecond();
	/**
	*   获取毫秒
	*/
	double getElapsedTimeInMilliSec();
	/**
	*   获取微妙
	*/
	double getElapsedTimeInMicroSec();
protected:
	LARGE_INTEGER   _frequency;
	LARGE_INTEGER   _startCount;
};