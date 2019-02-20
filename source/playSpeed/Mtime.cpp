

#include "Mtime.h"

Timestamp::Timestamp()
{
	QueryPerformanceFrequency(&_frequency);
	QueryPerformanceCounter(&_startCount);
}
Timestamp::~Timestamp()
{}

void    Timestamp::update()
{
	QueryPerformanceCounter(&_startCount);
}
/**
*   获取当前秒
*/
double Timestamp::getElapsedSecond()
{
	return  getElapsedTimeInMicroSec() * 0.000001;
}
/**
*   获取毫秒
*/
double Timestamp::getElapsedTimeInMilliSec()
{
	return this->getElapsedTimeInMicroSec() * 0.001;
}
/**
*   获取微妙
*/
double Timestamp::getElapsedTimeInMicroSec()
{
	LARGE_INTEGER endCount;
	QueryPerformanceCounter(&endCount);

	double  startTimeInMicroSec = _startCount.QuadPart * (1000000.0 / _frequency.QuadPart);
	double  endTimeInMicroSec = endCount.QuadPart * (1000000.0 / _frequency.QuadPart);

	return  endTimeInMicroSec - startTimeInMicroSec;
}
