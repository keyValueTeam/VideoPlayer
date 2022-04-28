#include "Player.h"

Player::Player(QWidget* parent = nullptr)
{
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    output = new QAudioOutput(format);
    device = output->start();
	mPlaySpdVal = 40;
	pcm1 = getPCM("test.mp4");
	timer_play = new QTimer();
	timer_play->setTimerType(Qt::PreciseTimer);


	QObject::connect(timer_play, &QTimer::timeout, [&] {
		if (pcm1.size() > 0) {
			int readSize = output->periodSize();
			int chunks = output->bytesFree() / readSize;

			while (chunks) {
				QByteArray samples = pcm1.mid(0, readSize);
				int len = samples.size();
				pcm1.remove(0, len);

				if (len) device->write(samples);
				if (len != readSize) break;

				chunks--;
			}
		}
		});
	timer_play->start(100);
    resize(720, 652);
	labelVideo = new QLabel(this);
	labelVideo->setGeometry(100, 100, 640, 368);
	//labelVideo->resize(video->size());
    action_OpenLocalFiles = new QAction(this);
    action_OpenNetStream = new QAction(this);
    action_OpenCamera = new QAction(this);
    action_About = new QAction(this);
    action_GetSourceCode = new QAction(this);
    centralWidget = new QWidget(this);
    openGLLabel = new QLabel(centralWidget);
    openGLLabel->setGeometry(QRect(40, 30, 611, 351));
    openGLLabel->setStyleSheet(QString::fromUtf8(""));
    label_Playtime = new QLabel(centralWidget);
    label_Playtime->setGeometry(QRect(40, 530, 331, 16));
    label_Playtime->setStyleSheet(QString::fromUtf8("QLabel\n"
        "{\n"
        "	font-family: \"Microsoft YaHei\";\n"
        "    font-size: 18px;\n"
        "    color: white;\n"
        "	background-color: rgb(53,53,53);\n"
        "}"));

    sliderVolume = new QSlider(centralWidget);
    sliderVolume->setGeometry(QRect(540, 530, 81, 16));
    sliderVolume->setOrientation(Qt::Horizontal);
    btnPlayVideo = new QPushButton(centralWidget);
    btnPlayVideo->setGeometry(QRect(380, 520, 41, 31));
    btnCutImage = new QPushButton(centralWidget);
    btnCutImage->setGeometry(QRect(430, 520, 30, 30));
    btnSetVolume = new QPushButton(centralWidget);
    btnSetVolume->setGeometry(QRect(510, 520, 30, 30));
    btnFullScreen = new QPushButton(centralWidget);
    btnFullScreen->setGeometry(QRect(640, 520, 30, 30));
    this->setCentralWidget(centralWidget);
    menuBar = new QMenuBar(this);
    menuBar->setGeometry(QRect(0, 0, 720, 37));
    menu_File = new QMenu(menuBar);
    menushezhi = new QMenu(menuBar);
    menu_Help = new QMenu(menuBar);
    this->setMenuBar(menuBar);
    mainToolBar = new QToolBar(this);
    this->addToolBar(Qt::TopToolBarArea, mainToolBar);
    statusBar = new QStatusBar(this);
    this->setStatusBar(statusBar);

    menuBar->addAction(menu_File->menuAction());
    menuBar->addAction(menushezhi->menuAction());
    menuBar->addAction(menu_Help->menuAction());
    menu_File->addAction(action_OpenLocalFiles);
    menu_File->addAction(action_OpenNetStream);
    menu_File->addAction(action_OpenCamera);
    menu_Help->addAction(action_About);
    menu_Help->addAction(action_GetSourceCode);
	QObject::connect(btnPlayVideo, &QPushButton::clicked, this, &Player::playVideo);

}

void Player::playVideo(bool p = true)
{
	const char* videoPath = "test.mp4";
	unsigned char* buf;
	int isVideo = -1;
	int ret, gotPicture;
	unsigned int i, streamIndex = 0;
	const AVCodec* pCodec;
	AVPacket* pAVpkt;
	AVCodecContext* pAVctx;
	AVFrame* pAVframe, * pAVframeRGB;
	AVFormatContext* pFormatCtx;
	struct SwsContext* pSwsCtx;

	//����AVFormatContext
	pFormatCtx = avformat_alloc_context();

	//��ʼ��pFormatCtx
	if (avformat_open_input(&pFormatCtx, videoPath, NULL, NULL) != 0)
	{
		qDebug("avformat_open_input err.");
		//return -1;
	}

	//��ȡ����Ƶ��������Ϣ
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
	{
		avformat_close_input(&pFormatCtx);
		qDebug("avformat_find_stream_info err.");
		//return -2;
	}

	//�ҵ���Ƶ��������
	for (i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			streamIndex = i;
			isVideo = 0;
			break;
		}
	}

	//û����Ƶ�����˳�
	if (isVideo == -1)
	{
		avformat_close_input(&pFormatCtx);
		qDebug("nb_streams err.");
		//return -3;
	}

	//��ȡ��Ƶ������
	pAVctx = avcodec_alloc_context3(NULL);;

	//���ҽ�����
	avcodec_parameters_to_context(pAVctx, pFormatCtx->streams[streamIndex]->codecpar);
	pCodec = avcodec_find_decoder(pAVctx->codec_id);
	if (pCodec == NULL)
	{
		avcodec_close(pAVctx);
		avformat_close_input(&pFormatCtx);
		qDebug("avcodec_find_decoder err.");
		//return -4;
	}

	//��ʼ��pAVctx
	if (avcodec_open2(pAVctx, pCodec, NULL) < 0)
	{
		avcodec_close(pAVctx);
		avformat_close_input(&pFormatCtx);
		qDebug("avcodec_open2 err.");
		//return -5;
	}

	//��ʼ��pAVpkt
	pAVpkt = (AVPacket*)av_malloc(sizeof(AVPacket));

	//��ʼ������֡�ռ�
	pAVframe = av_frame_alloc();
	pAVframeRGB = av_frame_alloc();

	//����ͼ�����ݴ洢buf
	//av_image_get_buffer_sizeһ֡��С
	buf = (unsigned char*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32, pAVctx->width, pAVctx->height, 1));
	av_image_fill_arrays(pAVframeRGB->data, pAVframeRGB->linesize, buf, AV_PIX_FMT_RGB32, pAVctx->width, pAVctx->height, 1);

	//������Ƶ������µ������ڴ�С ��Ƶ��� pAVctx->width, pAVctx->height
	//resizeWindow(pAVctx->width, pAVctx->height);

	//��ʼ��pSwsCtx
	pSwsCtx = sws_getContext(pAVctx->width, pAVctx->height, pAVctx->pix_fmt, pAVctx->width, pAVctx->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

	//ѭ����ȡ��Ƶ����
	for (;;)
	{
		if (mVideoPlaySta == Video_Playing)//���ڲ���
		{
			if (av_read_frame(pFormatCtx, pAVpkt) >= 0)//��ȡһ֡δ���������
			{
				//�������Ƶ����
				if (pAVpkt->stream_index == (int)streamIndex)
				{
					//����һ֡��Ƶ����
					//ret = avcodec_decode_video2(pAVctx, pAVframe, &gotPicture, pAVpkt);
					avcodec_send_packet(pAVctx, pAVpkt);
					ret = avcodec_receive_frame(pAVctx, pAVframe);
					if (ret < 0)
					{
						qDebug("Decode Error.\n");
						continue;
					}
					sws_scale(pSwsCtx, (const unsigned char* const*)pAVframe->data, pAVframe->linesize, 0, pAVctx->height, pAVframeRGB->data, pAVframeRGB->linesize);
					QImage img((uchar*)pAVframeRGB->data[0], pAVctx->width, pAVctx->height, QImage::Format_RGB32);
					labelVideo->setPixmap(QPixmap::fromImage(img));
					delay(mPlaySpdVal);
				}
				av_packet_unref(pAVpkt);
			}
			else
			{
				break;
			}
		}
		else if (mVideoPlaySta == Video_PlayFinish)//���Ž���
		{
			break;
		}
	}

	//�ͷ���Դ
	sws_freeContext(pSwsCtx);
	av_frame_free(&pAVframeRGB);
	av_frame_free(&pAVframe);
	avcodec_close(pAVctx);
	avformat_close_input(&pFormatCtx);
	mVideoPlaySta = Video_PlayFinish;

}

Player::~Player() {
    delete output;
}

void delay(int ms)
{
    QTime stopTime;
    stopTime.start();
    //qDebug()<<"start:"<<QTime::currentTime().toString("HH:mm:ss.zzz");
    while (stopTime.elapsed() < ms)//stopTime.elapsed()���ش�start��ʼ�����ڵĺ�����
    {
        QCoreApplication::processEvents();
    }
    //qDebug()<<"stop :"<<QTime::currentTime().toString("HH:mm:ss.zzz");
}

//QString path = QFileDialog::getOpenFileName(this, "ѡ����Ƶ�ļ�", "D:\\Test\\Video\\", "WMV��Ƶ(*.wmv);;MP4��Ƶ(*.mp4);;AVI��Ƶ(*.avi)");
	//if (!path.isEmpty())
	//{
	//    //mVideoFile->setText(path);
	//    //on_videoPlayCtrl();
	//}


//û���ڲ�����Ƶʱֱ���˳�
	//if (mVideoPlaySta != Video_PlayFinish)
	//{
	//    if (QMessageBox::Yes == QMessageBox::information(this, "��ʾ", "ȷ�Ϲرգ�", QMessageBox::Yes, QMessageBox::No))
	//    {
	//        mVideoPlaySta = Video_PlayFinish;
	//        //event->accept();
	//    }
	//    else
	//    {
	//        event->ignore();//���ԣ����ر�
	//    }
	//}