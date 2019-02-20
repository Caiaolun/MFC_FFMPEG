

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
*   ��ȡ��ǰ��
*/
double Timestamp::getElapsedSecond()
{
	return  getElapsedTimeInMicroSec() * 0.000001;
}
/**
*   ��ȡ����
*/
double Timestamp::getElapsedTimeInMilliSec()
{
	return this->getElapsedTimeInMicroSec() * 0.001;
}
/**
*   ��ȡ΢��
*/
double Timestamp::getElapsedTimeInMicroSec()
{
	LARGE_INTEGER endCount;
	QueryPerformanceCounter(&endCount);

	double  startTimeInMicroSec = _startCount.QuadPart * (1000000.0 / _frequency.QuadPart);
	double  endTimeInMicroSec = endCount.QuadPart * (1000000.0 / _frequency.QuadPart);

	return  endTimeInMicroSec - startTimeInMicroSec;
}
