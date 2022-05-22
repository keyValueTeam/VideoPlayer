#pragma once
#include <QThread>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample\swresample.h>
#include <libavdevice/avdevice.h>
#include <libavformat/version.h>
#include <libavutil/time.h>
#include <libavutil/mathematics.h>
#include <libavutil/imgutils.h>
}

#include "RingBuffer.h"

struct AudioFrame
{
	char* frameData;
	int frameSize;
};

class MediaLoader
{
public:
	static void load(const char* filename, int* para, AVFormatContext* fmtCtx)
	{
		if (avformat_open_input(&fmtCtx, filename, NULL, NULL) < 0) {
			para[0] = -1;
			return;
		}
		if (avformat_find_stream_info(fmtCtx, NULL) < 0) {
			para[0] = -2;
			return;
		}
		para[0] = 0;
		para[1] = -1;
		for (size_t i = 0; i < fmtCtx->nb_streams; i++) {
			if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
				para[1] = (int)i;
			}
		}
		return;
	}

private:
	MediaLoader();
};


static const int MAX_AUDIO_FRAME_SIZE = 192000;

class DecodeThread : public QThread
{
signals:
	void getPicture(char* data,int height,int width);

public:
	DecodeThread(AVFormatContext* formatCtx, int streamIndex, RingBuffer<AudioFrame, 50>* auBuf) {
		
		audioBuf = auBuf;
		pkt = av_packet_alloc();
		frame = av_frame_alloc();
		outSampleFmt = AV_SAMPLE_FMT_S16;
		audio_out_buffer = (uint8_t*)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
		fmtCtx = formatCtx;
		audioIndex = streamIndex;
		AVCodecParameters* aCodecPara = fmtCtx->streams[audioIndex]->codecpar;
		const AVCodec* aCodec = avcodec_find_decoder(aCodecPara->codec_id);
		if (!aCodec) {
			//return -1;
		}
		aCodecCtx = avcodec_alloc_context3(aCodec);
		if (avcodec_parameters_to_context(aCodecCtx, aCodecPara) < 0) {
			//return -2;
		}
		aCodecCtx->pkt_timebase = fmtCtx->streams[audioIndex]->time_base;
		if (avcodec_open2(aCodecCtx, aCodec, NULL) < 0) {
			//return -3;
		}
		InitTransCode();
		
	}
	void run() {
		
		int out_size;
		char* auData;
		while (av_read_frame(fmtCtx, pkt) >= 0) {
			if (pkt->stream_index == audioIndex) {
				if (avcodec_send_packet(aCodecCtx, pkt) >= 0) {
					while (avcodec_receive_frame(aCodecCtx, frame) >= 0) {
						if (av_sample_fmt_is_planar(aCodecCtx->sample_fmt)) {
							int len = swr_convert(swr_ctx,
								&audio_out_buffer,
								MAX_AUDIO_FRAME_SIZE * 2,
								(const uint8_t**)frame->data,
								frame->nb_samples);
							if (len <= 0) {
								continue;
							}

							out_size = av_samples_get_buffer_size(0,
								outChannels,
								len,
								outSampleFmt,
								1);
							auData = new char[MAX_AUDIO_FRAME_SIZE * 2];
							memcpy_s(auData, out_size, (char*)audio_out_buffer, out_size);
							AudioFrame aframe;
							aframe.frameData = auData;
							aframe.frameSize = out_size;
							//pkt->pts* av_q2d(aCodecCtx->time_base);
							//char buffer[50] = { 0 };
							//sprintf(buffer, "pkt->pts: %lld;%f\n", pkt->pts, av_q2d(aCodecCtx->time_base));
							//av_log(NULL, AV_LOG_INFO, buffer);
							while (true)
							{
								if (audioBuf->push(aframe))
								{
									break;
								}
							}
						}
					}
				}
			}
			av_packet_unref(pkt);
		}
		av_frame_free(&frame);
		av_log(NULL, AV_LOG_INFO, "push over");
		av_free(&audio_out_buffer);
		while (true)
		{
			AudioFrame aframe;
			aframe.frameData = nullptr;//EOQ
			aframe.frameSize = -1;
			if (audioBuf->push(aframe))
			{
				quit();
				break;
			}
		}
	}
private:
	RingBuffer<AudioFrame, 50>* audioBuf;
	AVPacket* pkt;
	AVFrame* frame;
	uint8_t* audio_out_buffer;
	SwrContext* swr_ctx = NULL;
	AVFormatContext* fmtCtx = NULL;
	int audioIndex = -1;
	int videoIndex = -1;
	enum AVSampleFormat outSampleFmt;
	int outChannels = 2;
	AVCodecContext* aCodecCtx = NULL;
	void InitTransCode() {
		uint64_t out_channel_layout = aCodecCtx->channel_layout;
		outSampleFmt = AV_SAMPLE_FMT_S16;
		int out_sample_rate = aCodecCtx->sample_rate;
		outChannels = av_get_channel_layout_nb_channels(out_channel_layout);


		swr_ctx = swr_alloc_set_opts(NULL,
			out_channel_layout,
			outSampleFmt,
			out_sample_rate,
			aCodecCtx->channel_layout,
			aCodecCtx->sample_fmt,
			aCodecCtx->sample_rate,
			0, NULL);
		swr_init(swr_ctx);
	}
};
