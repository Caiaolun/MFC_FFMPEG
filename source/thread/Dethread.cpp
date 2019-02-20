

#include "Dethread.h"


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
*   加载文件
*/
void    DecodeThread::load(const char* fileName)
{
	_ffReader.load(fileName);
}
/**
*   线程执行函数
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
		//! 这里需要通知窗口进行重绘制更新，显示更新数据
		SendMessage(_hWnd, WM_UPDATE_VIDEO, (WPARAM)&infor, 0);
	}

	return  true;
}