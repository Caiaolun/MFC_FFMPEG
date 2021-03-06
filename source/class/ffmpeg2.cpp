#include "ffmpeg2.h"


FFVideoReader::FFVideoReader()
{
	_formatCtx = 0;
	_videoIndex = -1;
	_codecCtx = 0;
	_codec = 0;
	_frame = 0;
	_frameRGB = 0;
	_convertCtx = 0;
	_screenW = 0;
	_screenH = 0;

}

FFVideoReader::~FFVideoReader()
{
	sws_freeContext(_convertCtx);
	av_free(_frameRGB);
	av_free(_frame);
	avcodec_close(_codecCtx);
	avformat_close_input(&_formatCtx);
}

void    FFVideoReader::setup()
{
	av_register_all();
	_formatCtx = avformat_alloc_context();
}
int     FFVideoReader::load(const char* filepath)
{
	int     ret = 0;

	//! 打开文件
	if (avformat_open_input(&_formatCtx, filepath, NULL, NULL) != 0)
	{
		return -1;
	}
	//! 检测文件中是否存在数据流
	if (avformat_find_stream_info(_formatCtx, NULL) < 0)
	{
		return -1;
	}
	//! 获取视频流索引
	_videoIndex = -1;
	for (int i = 0; i < _formatCtx->nb_streams; i++)
	{
		if (_formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			_videoIndex = i;
			break;
		}
	}
	/**
	*   没有视频流，则返回
	*/
	if (_videoIndex == -1)
	{
		return -1;
	}
	_codecCtx = _formatCtx->streams[_videoIndex]->codec;
	_codec = avcodec_find_decoder(_codecCtx->codec_id);
	if (_codec == NULL)
	{
		return -1;
	}
	/**
	*   打开解码器
	*/
	if (avcodec_open2(_codecCtx, _codec, NULL) < 0)
	{
		return -1;
	}
	_frame = av_frame_alloc();
	_frameRGB = av_frame_alloc();

	_screenW = _codecCtx->width;
	_screenH = _codecCtx->height;

	_convertCtx = sws_getContext(
		_codecCtx->width
		, _codecCtx->height
		, _codecCtx->pix_fmt
		, _codecCtx->width
		, _codecCtx->height
		, AV_PIX_FMT_RGB24
		, SWS_BICUBIC
		, NULL
		, NULL
		, NULL
	);

	int     numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, _codecCtx->width, _codecCtx->height);
	uint8_t*buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
	avpicture_fill((AVPicture *)_frameRGB, buffer, AV_PIX_FMT_RGB24, _codecCtx->width, _codecCtx->height);
	_imageSize = numBytes;
	return  0;
}

bool    FFVideoReader::readFrame(FrameInfor& infor)
{
	AVPacket packet;
	av_init_packet(&packet);
	for (;;)
	{
		if (av_read_frame(_formatCtx, &packet))
		{
			av_free_packet(&packet);
			return false;
		}
		if (packet.stream_index != _videoIndex)
		{
			continue;
		}
		int frame_finished = 0;

		int res = avcodec_decode_video2(_codecCtx, _frame, &frame_finished, &packet);

		if (frame_finished)
		{
			AVStream*   streams = _formatCtx->streams[_videoIndex];
			double      tmbase = av_q2d(streams->time_base);
			int64_t     pts = _frame->pts;



			char        buf[128];
			sprintf(buf, "pts = %I64d     dts =  %I64d\n", packet.pts, packet.dts);
			int res = sws_scale(
				_convertCtx
				, (const uint8_t* const*)_frame->data
				, _frame->linesize
				, 0
				, _codecCtx->height
				, _frameRGB->data
				, _frameRGB->linesize
			);
			av_packet_unref(&packet);

			infor._data = _frameRGB->data[0];
			infor._dataSize = _imageSize;
			infor._width = _screenW;
			infor._height = _screenH;
			infor._pts = _frame->pts;
			infor._timeBase = av_q2d(streams->time_base);

			return  true;
		}
	}
	return  false;
}
void*   FFVideoReader::readFrame()
{
	AVPacket packet;
	av_init_packet(&packet);
	for (;;)
	{
		if (av_read_frame(_formatCtx, &packet))
		{
			av_free_packet(&packet);
			return 0;
		}
		if (packet.stream_index != _videoIndex)
		{
			continue;
		}
		int frame_finished = 0;

		int res = avcodec_decode_video2(_codecCtx, _frame, &frame_finished, &packet);

		if (frame_finished)
		{
			AVStream*   streams = _formatCtx->streams[_videoIndex];
			double      tmbase = av_q2d(streams->time_base);
			int64_t     pts = _frame->pts;

			char        buf[128];
			sprintf(buf, "pts = %I64d     dts =  %I64d\n", packet.pts, packet.dts);
			int res = sws_scale(
				_convertCtx
				, (const uint8_t* const*)_frame->data
				, _frame->linesize
				, 0
				, _codecCtx->height
				, _frameRGB->data
				, _frameRGB->linesize
			);
			av_packet_unref(&packet);

			return  _frameRGB->data[0];
		}
	}
	return  0;
}


