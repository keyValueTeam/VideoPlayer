#pragma once
#include <QAudioOutput>
#include <QAudioFormat>
#include <QTimer>
#include "DecodeThread.h"

const int Video_Playing = 1;
const int Video_PlayFinish = 0;
const int Video_PlaySuspend = 2;


//在PlayTask中决定播放视频帧，然后打印到屏幕上（paintEvent?)

class PlayTask : public QObject
{
	Q_OBJECT
public:
	explicit PlayTask(QObject* parent, RingBuffer<AudioFrame, 50>* buf)
	{
		audioBuf = buf;
		QAudioFormat format;
		mVideoPlaySta = Video_Playing;
		format.setSampleRate(44100);
		format.setChannelCount(2);
		format.setSampleSize(16);
		format.setCodec("audio/pcm");
		format.setByteOrder(QAudioFormat::LittleEndian);
		format.setSampleType(QAudioFormat::UnSignedInt);

		output = new QAudioOutput(format);
		device = output->start();
		QObject::connect(&audioTimer, SIGNAL(timeout()), this, SLOT(PlayPeriod()));

		audioTimer.start(5);
	}

	~PlayTask() {
		delete output;
		
	}
signals:
	void finish();
public slots:
	void PlayPeriod()
	{
		if (mVideoPlaySta == Video_Playing) {
			AudioFrame aframe;
			while (true)
			{
				if (audioBuf->pop(aframe))
				{
					break;
				}
			}
			while (output->bytesFree() < aframe.frameSize)
			{

			}
			if (aframe.frameSize > 0)
			{
				QByteArray samples = QByteArray(aframe.frameData, aframe.frameSize);
				device->write(samples);
				delete[] aframe.frameData;
			}
			else
			{
				mVideoPlaySta = Video_PlayFinish;
				av_log(NULL, AV_LOG_INFO, "RECEIVE EOQ PACKET");
				emit finish();
			}
		}
		else if(mVideoPlaySta == Video_PlaySuspend)
		{
			//
		}
		else if (mVideoPlaySta == Video_PlayFinish)
		{
			//
		}
	}
	
private:
	QTimer audioTimer;
	RingBuffer<AudioFrame, 50>* audioBuf;
	int mVideoPlaySta;
	QAudioOutput* output;
	QIODevice* device;
};