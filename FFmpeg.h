#pragma once
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

#include <QAudioOutput>
#include <QTimer>
#include <QDataStream>
#include <qmath.h>
#include <QDebug>
#include <QMessageBox>


struct MyMediaInfo {
	int audioStream;
	int videoStream;
	AVFormatContext* fmtCtx;
	AVCodecContext* codecCtx;
};


struct TransCodePara
{
	uint64_t outChannelLayout;//same as ...
	enum AVSampleFormat outSampleFmt;
	int outSampleRate;//
	int out_channels;
};

class FFmpegDecoder {
public:
	void Load(QString url, MyMediaInfo*);
	void ConvertConfig(TransCodePara*);
	QByteArray MediaToPcm(MyMediaInfo*);
private:
	FFmpegDecoder();
	SwrContext* swrCtx;
	//转码参数
	uint64_t outChannelLayout;//same as ...
	enum AVSampleFormat outSampleFmt;
	int outSampleRate;
	int outChannels;
	//源文件参数
	int audioStream;
	int videoStream;
	AVFormatContext* fmtCtx;
	AVCodecContext* codecCtx;
};

QByteArray getPCM(QString);