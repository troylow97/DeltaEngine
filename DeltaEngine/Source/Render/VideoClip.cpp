#include "VideoClip.h"
#include "ErrorCheck.h"

namespace DeltaEngine
{
  void VideoClip::Init()
  {
		// initialize libav
		//av_register_all();
		//avformat_network_init();
  }

	VideoClip::VideoClip(std::string name)
	{
		//// initialize custom data structure
		//AppData data;
		//data.Init();

		//// open video
		//if (avformat_open_input(&data.fmt_ctx, name.c_str(), NULL, NULL) < 0)
		//{
		//	std::cout << "failed to open input" << std::endl;
		//	data.Clear();
		//	return;
		//}

		//// find stream info
		//if (avformat_find_stream_info(data.fmt_ctx, NULL) < 0)
		//{
		//	std::cout << "failed to get stream info" << std::endl;
		//	data.Clear();
		//	return;
		//}

		//// dump debug info
		//av_dump_format(data.fmt_ctx, 0, name.c_str(), 0);

		//// find the video stream
		//for (unsigned i = 0; i < data.fmt_ctx->nb_streams; ++i)
		//{
		//	if (data.fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		//	{
		//		data.stream_idx = i;
		//		break;
		//	}
		//}

		//if (data.stream_idx == -1)
		//{
		//	std::cout << "failed to find video stream" << std::endl;
		//	data.Clear();
		//	return;
		//}

		//data.video_stream = data.fmt_ctx->streams[data.stream_idx];
		//data.codec_ctx = data.video_stream->codec;

		//// find the decoder
		//data.decoder = avcodec_find_decoder(data.codec_ctx->codec_id);
		//if (data.decoder == NULL)
		//{
		//	std::cout << "failed to find decoder" << std::endl;
		//	data.Clear();
		//	return;
		//}

		//// open the decoder
		//if (avcodec_open2(data.codec_ctx, data.decoder, NULL) < 0)
		//{
		//	std::cout << "failed to open codec" << std::endl;
		//	data.Clear();
		//	return;
		//}

		//// allocate the video frames
		//data.av_frame = av_frame_alloc();
		//data.gl_frame = av_frame_alloc();
		//int size = avpicture_get_size(AV_PIX_FMT_RGB24, data.codec_ctx->width,
		//	data.codec_ctx->height);
		//uint8_t* internal_buffer = (uint8_t*)av_malloc(size * sizeof(uint8_t));
		//avpicture_fill((AVPicture*)data.gl_frame, internal_buffer, AV_PIX_FMT_RGB24,
		//	data.codec_ctx->width, data.codec_ctx->height);
		//data.packet = (AVPacket*)av_malloc(sizeof(AVPacket));

		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
		GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		//GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		//	data.codec_ctx->width, data.codec_ctx->height,
		//	0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void VideoClip::GetFrame()
	{
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}
}
