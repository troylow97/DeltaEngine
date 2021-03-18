#pragma once

#include "DEpch.h"

//extern "C"
//{
//#include <libavformat/avformat.h>
//#include <libavcodec/avcodec.h>
//#include <libavfilter/avfilter.h>
//#include <libavdevice/avdevice.h>
//#include <libswresample/swresample.h>
//#include <libswscale/swscale.h>
//#include <libavutil/avutil.h>
//}


namespace DeltaEngine
{
  class VideoClip
  {
  //  struct AppData
		//{
		//	AVFormatContext* fmt_ctx;
		//	int stream_idx;
		//	AVStream* video_stream;
		//	AVCodecContext* codec_ctx;
		//	AVCodec* decoder;
		//	AVPacket* packet;
		//	AVFrame* av_frame;
		//	AVFrame* gl_frame;
		//	SwsContext* conv_ctx;

		//	// initialize the app data structure
		//	void Init();
		//	// clean up the app data structure
		//	void Clear();
		//	// read a video frame
		//	bool ReadFrame();
		//};
		//AppData data{};
		unsigned m_RendererID;
	public:

    static void Init();
    VideoClip(std::string name);
    void GetFrame();
  };
}
