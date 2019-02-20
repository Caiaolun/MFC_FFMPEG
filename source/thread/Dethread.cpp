

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
		//! ������Ҫ֪ͨ���ڽ����ػ��Ƹ��£���ʾ��������
		SendMessage(_hWnd, WM_UPDATE_VIDEO, (WPARAM)&infor, 0);
	}

	return  true;
}