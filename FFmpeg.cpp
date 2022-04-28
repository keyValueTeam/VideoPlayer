#include "FFmpeg.h"
static const int MAX_AUDIO_FRAME_SIZE = 192000;
static const int SDL_AUDIO_BUFFER_SIZE = 1024;

FFmpegDecoder::FFmpegDecoder()
{
	ConvertConfig(NULL);
}

void FFmpegDecoder::Load(QString url, MyMediaInfo* info)
{
	if (avformat_open_input(&info->fmtCtx, url.toLocal8Bit().data(), NULL, NULL) < 0) {
		qDebug("Cannot open input file.");
		exit(-1);
	}
	if (avformat_find_stream_info(info->fmtCtx, NULL) < 0) {
		qDebug("Cannot find any stream in file.");
		exit(-1);
	}
	//av_dump_format(fmtCtx, 0, url.toLocal8Bit().data(), 0);
	for (size_t i = 0; i < info->fmtCtx->nb_streams; i++) {
		if (info->fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			info->audioStream = (int)i;
		}
		else if (info->fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
			info->videoStream = int(i);
		}
	}
}

void FFmpegDecoder::ConvertConfig(TransCodePara* para)
{
	uint64_t outChannelLayout = codecCtx->channel_layout;
	enum AVSampleFormat outSampleFmt = AV_SAMPLE_FMT_S16;
	int outSampleRate = codecCtx->sample_rate;
	int out_channels = av_get_channel_layout_nb_channels(outChannelLayout);
	//设置转码参数
	SwrContext* swr_ctx = swr_alloc_set_opts(NULL,
		outChannelLayout,
		outSampleFmt,
		outSampleRate,
		codecCtx->channel_layout,
		codecCtx->sample_fmt,
		codecCtx->sample_rate,
		0, NULL);
	swr_init(swr_ctx);//初始化转码
}
	
QByteArray FFmpegDecoder::MediaToPcm(MyMediaInfo* info)
{
	
	AVCodecParameters* audioCodecPara = info->fmtCtx->streams[info->audioStream]->codecpar;
	const AVCodec* audioCodec = avcodec_find_decoder(audioCodecPara->codec_id);
	if (!audioCodec) {
		qDebug("Cannot find any codec for audio.");
		exit(1315);
	}
	info->codecCtx = avcodec_alloc_context3(audioCodec);
	if (avcodec_parameters_to_context(info->codecCtx, audioCodecPara) < 0) {
		qDebug("Cannot alloc codec context.");
		exit(-1);
	}

	info->codecCtx->pkt_timebase = info->fmtCtx->streams[info->audioStream]->time_base;
	if (avcodec_open2(info->codecCtx, audioCodec, NULL) < 0) {
		qDebug("Cannot open audio codec.");
		exit(-1);
	}
	QByteArray pcmBytes;
	QDataStream pcmStream(&pcmBytes, QIODevice::WriteOnly);
	pcmStream.setByteOrder(QDataStream::LittleEndian);
	AVPacket* packet;
	AVFrame* frame;

	uint8_t* audio_out_buffer = (uint8_t*)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
	//double sleep_time = 0;

	while (av_read_frame(info->fmtCtx, packet) >= 0) {
		if (packet->stream_index == info->audioStream) {
			if (avcodec_send_packet(info->codecCtx, packet) >= 0) {
				while (avcodec_receive_frame(info->codecCtx, frame) >= 0) {
					if (av_sample_fmt_is_planar(info->codecCtx->sample_fmt)) {
						int len = swr_convert(swrCtx,
							&audio_out_buffer,
							MAX_AUDIO_FRAME_SIZE * 2,
							(const uint8_t**)frame->data,
							frame->nb_samples);
						if (len <= 0) {
							continue;
						}

						int out_size = av_samples_get_buffer_size(0,
							outChannels,
							len,
							outSampleFmt,
							1);
						//qDebug("buffer size is: %d.",dst_bufsize);

						/*if (audioOutput->bytesFree() < out_size) {
							QTest::qSleep(sleep_time);//等待？
							streamOut->write((char*)audio_out_buffer, out_size);
						}*/
						pcmStream.writeRawData((char*)audio_out_buffer, out_size);
						//将数据写入PCM文件
						//fwrite(audio_out_buffer,1,dst_bufsize,file);
					}
				}
			}
		}
		av_packet_unref(packet);
		av_frame_unref(frame);
	}
}

QByteArray getPCM(QString _url)
{
	QByteArray pcmBytes;
	QDataStream outStream(&pcmBytes, QIODevice::WriteOnly);
	outStream.setByteOrder(QDataStream::LittleEndian);
	AVFormatContext* fmtCtx = avformat_alloc_context();
	AVCodecContext* codecCtx = NULL;
	AVPacket* pkt = av_packet_alloc();
	AVFrame* frame = av_frame_alloc();
	if (avformat_open_input(&fmtCtx, _url.toLocal8Bit().data(), NULL, NULL) < 0) {
		qDebug("Cannot open input file.");
		exit(-1);
	}
	if (avformat_find_stream_info(fmtCtx, NULL) < 0) {
		qDebug("Cannot find any stream in file.");
		exit(-1);
	}

	av_dump_format(fmtCtx, 0, _url.toLocal8Bit().data(), 0);
	int StreamIndex = -1;

	for (size_t i = 0; i < fmtCtx->nb_streams; i++) {
		if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
			StreamIndex = (int)i;
			break;
		}
	}
	if (StreamIndex == -1) {
		qDebug("Cannot find audio stream.");
		exit(1314);
	}

	AVCodecParameters* aCodecPara = fmtCtx->streams[StreamIndex]->codecpar;
	const AVCodec* codec = avcodec_find_decoder(aCodecPara->codec_id);
	if (!codec) {
		qDebug("Cannot find any codec for audio.");
		exit(1315);
	}
	codecCtx = avcodec_alloc_context3(codec);
	if (avcodec_parameters_to_context(codecCtx, aCodecPara) < 0) {
		qDebug("Cannot alloc codec context.");
		exit(-1);
	}
	codecCtx->pkt_timebase = fmtCtx->streams[StreamIndex]->time_base;

	if (avcodec_open2(codecCtx, codec, NULL) < 0) {
		qDebug("Cannot open audio codec.");
		exit(-1);
	}

	//设置转码参数
	uint64_t out_channel_layout = codecCtx->channel_layout;
	enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
	int out_sample_rate = codecCtx->sample_rate;
	int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
	//printf("out rate : %d , out_channel is: %d\n",out_sample_rate,out_channels);

	uint8_t* audio_out_buffer = (uint8_t*)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);

	SwrContext* swr_ctx = swr_alloc_set_opts(NULL,
		out_channel_layout,
		out_sample_fmt,
		out_sample_rate,
		codecCtx->channel_layout,
		codecCtx->sample_fmt,
		codecCtx->sample_rate,
		0, NULL);
	swr_init(swr_ctx);

	double sleep_time = 0;

	while (av_read_frame(fmtCtx, pkt) >= 0) {
		if (pkt->stream_index == StreamIndex) {
			if (avcodec_send_packet(codecCtx, pkt) >= 0) {
				while (avcodec_receive_frame(codecCtx, frame) >= 0) {
					if (av_sample_fmt_is_planar(codecCtx->sample_fmt)) {
						int len = swr_convert(swr_ctx,
							&audio_out_buffer,
							MAX_AUDIO_FRAME_SIZE * 2,
							(const uint8_t**)frame->data,
							frame->nb_samples);
						if (len <= 0) {
							continue;
						}
						//qDebug("convert length is: %d.\n",len);

						int out_size = av_samples_get_buffer_size(0,
							out_channels,
							len,
							out_sample_fmt,
							1);
						//qDebug("buffer size is: %d.",dst_bufsize);

						/*if (audioOutput->bytesFree() < out_size) {
							QTest::qSleep(sleep_time);//等待？
							streamOut->write((char*)audio_out_buffer, out_size);
						}*/
						outStream.writeRawData((char*)audio_out_buffer, out_size);
						//将数据写入PCM文件
						//fwrite(audio_out_buffer,1,dst_bufsize,file);
					}
				}
			}
		}
		av_packet_unref(pkt);
	}
	return pcmBytes;
}
