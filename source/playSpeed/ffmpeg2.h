#pragma once


extern "C"
{
#include <libavutil/imgutils.h>
#include <libavutil/parseutils.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libavutil/file.h>
}


struct  FrameInfor
{
	void*   _data;
	int     _dataSize;
	int     _width;
	int     _height;
	int64_t _pts;
	double  _timeBase;

};
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

	bool    readFrame(FrameInfor& infor);

	void*   readFrame();
};

