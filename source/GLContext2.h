#pragma once

#include <gl/gl.h>

class   GLContext
{
protected:
	int         _format;
	//! 窗口句柄
	HWND        _hWnd;
	//! 绘制设备上下文
	HDC         _hDC;
	//! OpenGL上下文
	HGLRC       _hRC;
public:
	GLContext();

	~GLContext();
	/**
	*   初始化GL
	*/
	bool    setup(HWND hWnd, HDC hDC);

	/**
	*   销毁EGL
	*/
	void    shutdown();

	/**
	*   交换缓冲区
	*/
	void    swapBuffer();
};
