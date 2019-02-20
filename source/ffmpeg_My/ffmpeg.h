#pragma once

extern "C"
{
#include <libavutil\imgutils.h>
#include <libavutil\parseutils.h>
#include <libswscale\swscale.h>
#include <libavcodec\avcodec.h>
#include <libavformat\avio.h>
#include <libavformat\avformat.h>
#include <libavutil\file.h>
}


class   FFVideoReader
{
public:
	AVFormatContext*_formatCtx;
	int             _videoIndex;
	AVCodecContext* _codecCtx;
	AVCodec*        _codec;
	AVFrame*        _frame;
	AVFrame*        _frameRGB;
	SwsContext*     _convertCtx;
public:
	int             _screenW;
	int             _screenH;

	int             _imageSize;
public:
	FFVideoReader();

	~FFVideoReader();

	void    setup();
	int     load(const char* filepath = "11.flv");
	void*   readFrame();
	
};