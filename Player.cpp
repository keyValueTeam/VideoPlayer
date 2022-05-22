#include "Player.h"
#include <QDebug>
#include "ui_widget.h"
#include <QScrollBar>


Player::Player(QWidget* parent = nullptr) :ui(new Ui::Widget)
{
	ui->setupUi(this);
	ui->lineEdit->setPlaceholderText(QString::fromLocal8Bit("输入歌手或歌曲"));
	pFormatCtx = avformat_alloc_context();
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("my1.db");
	if (!db.open())
	{
		return;
	}
	InitElse();
	InitIcon();
	/*resize(1280, 840);

	uis.btnPlay = new QPushButton(this);
	uis.btnPlay->setGeometry(QRect(180, 720, 140, 30));

	uis.btnOpenDir = new QPushButton(this);
	uis.btnOpenDir->setGeometry(QRect(400,720,140,30));

	uis.btnSelect = new QPushButton(this);
	uis.btnSelect->setGeometry(QRect(600,720,140,30));
	QObject::connect(uis.btnPlay, &QPushButton::clicked, this, &Player::PlayAudio);
	QObject::connect(uis.btnOpenDir, &QPushButton::clicked, this, &Player::LoadAudios);
	QObject::connect(uis.btnSelect, &QPushButton::clicked, this, &Player::SelectAudios);*/
}

Player::~Player() {

}

void Player::InitElse()
{
	ui->listWidget->verticalScrollBar()->setCursor(Qt::PointingHandCursor);
	ui->listWidget_2->verticalScrollBar()->setCursor(Qt::PointingHandCursor);
	ui->listWidget_3->verticalScrollBar()->setCursor(Qt::PointingHandCursor);
	ui->listWidget_4->verticalScrollBar()->setCursor(Qt::PointingHandCursor);
	QString listWidgetStyle = "QScrollBar:vertical"
		"{"
		"width:8px;"
		"background-color:transparent;"
		"margin:0px,0px,0px,0px;"
		" padding-top:12px;   /*上预留位置*/"
		" padding-bottom:12px;    /*下预留位置*/"
		"}"


		"  QScrollBar::handle:vertical"
		" {"
		"width:8px;"
		" background-color:rgba(255,255,255,0.2);"
		" border-radius:4px;"
		" min-height:20px;"
		" }"


		"    QScrollBar::handle:vertical:hover"
		"   {"
		"      width:9px;"
		" background-color:rgba(255,255,255,0.5);"
		"   border-radius:4px;"
		"     min-height:20;"
		"  }"

		"  QScrollBar::add-line:vertical"
		"  {"
		" height:12px;"
		"     width:10px;"
		"        border-image:url(:/selectfile/scroll/3.png);"
		" subcontrol-position:bottom;"
		"    }"

		"    QScrollBar::sub-line:vertical"
		"    {"
		"        height:12px;"
		"width:10px;"
		"   border-image:url(:/selectfile/scroll/1.png);"
		"subcontrol-position:top;"
		"   }"
		"    QScrollBar::add-line:vertical:hover"
		"   {"
		" height:12px;"
		"width:10px;"
		" border-image:url(:/selectfile/scroll/4.png);"
		"subcontrol-position:bottom;"
		" }"


		" QScrollBar::sub-line:vertical:hover"
		" {"
		"     height:12px;"
		"  width:10px;"
		"  border-image:url(:/selectfile/scroll/2.png);"
		" subcontrol-position:top;"
		" }"

		" QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
		" {"
		" background-color:transparent;"
		"   border-radius:4px;"
		"}";
	ui->listWidget_2->verticalScrollBar()->setStyleSheet(listWidgetStyle);
	ui->listWidget_3->verticalScrollBar()->setStyleSheet(listWidgetStyle);
	ui->listWidget->verticalScrollBar()->setStyleSheet(listWidgetStyle);
	ui->listWidget_4->verticalScrollBar()->setStyleSheet(listWidgetStyle);
	ui->listWidget_5->verticalScrollBar()->setStyleSheet(listWidgetStyle);
	Music = new Dialog();//小播放器
	Music->setWindowFlags(Qt::FramelessWindowHint);//设置窗体类型
	QString FileName1 = ":/image/image/background/bg8.jpg";
	transparency = 1;

	ui->frame->setWindowOpacity(transparency);//设置透明度
	ui->frame->setStyleSheet(QString("QFrame#frame{border-radius:5px;border-image: url(%1);}").arg(FileName1));
	music = new QMediaPlayer(this);

	playlist = new QMediaPlaylist(this);//本地音乐播放列表
	playlist->setPlaybackMode(QMediaPlaylist::Loop);//随机播放
	playlist_2 = new QMediaPlaylist(this);//我喜欢播放列表
	playlist_2->setPlaybackMode(QMediaPlaylist::Loop);//随机播放
	playlist_3 = new QMediaPlaylist(this);//我的收藏播放列表
	playlist_3->setPlaybackMode(QMediaPlaylist::Loop);//随机播放
	playlist_4 = new QMediaPlaylist(this);
	playlist_4->setPlaybackMode(QMediaPlaylist::Loop);

	ui->verticalSlider->hide();
	ui->verticalSlider->setValue(100);


	ui->horizontalSlider->setValue(0);
	ui->stackedWidget->setCurrentIndex(0);
	ui->pushButton_10->setStyleSheet("QPushButton"
		"{"
		"background-image: url(:/image/image/image-hover/music-hover.png);"
		"background-repeat:no-repeat;"
		"background-position:center center;"
		"border:none;"

		"}"

		"QPushButton:hover{"

		"background-image: url(:/image/image/image-hover/music-hover.png);"
		"background-repeat:no-repeat;"
		"background-position:center center;"


		"}"
		"QPushButton:pressed{"
		"background-image: url(:/image/image/image/music.png);"
		"background-repeat:no-repeat;"
		"background-position:center center;"

		"}"

	);

}

//query.exec("create table LocalMusic(id(14) int primary key,duration(10) int,name varchar,artist varchar,album varchar,path varchar)");
void Player::LoadAudios()
{
	QString dirName = QFileDialog::getExistingDirectory();
	QDir dir(dirName);

	QStringList filter;
	filter.append("*.mp3");
	//other audio file suffix
	QSqlQuery query(db);
	QList<QFileInfo> fileInfo(dir.entryInfoList(filter));
	int duration = 0;
	int para[2] = { 0,-1 };
	for (int i = 0; i < fileInfo.count(); i++)
	{
		QStringList a;
		AVFormatContext* fmtCtx = avformat_alloc_context();
		MediaLoader::load(fileInfo.at(i).filePath().toStdString().c_str(), para, fmtCtx);
		AVDictionaryEntry* tag = nullptr;
		duration = fmtCtx->duration;
		if (para[0] != 0 || para[1] < 0)
		{
			continue;
		}
		if (tag = av_dict_get(fmtCtx->metadata, "artist", NULL, 0))
		{
			a.append(QString(tag->value));
		}
		else
		{
			a.append(QString("unknown artist"));
		}
		if (tag = av_dict_get(fmtCtx->metadata, "title", NULL, 0))
		{
			a.append(QString(tag->value));
		}
		else
		{
			a.append(fileInfo.at(i).filePath());
		}
		if (tag = av_dict_get(fmtCtx->metadata, "album", NULL, 0))
		{
			a.append(QString(tag->value));
		}
		else
		{
			a.append(QString("unknown album"));
		}
		av_free(fmtCtx);
		QString insert = "INSERT INTO LocalMusic (id,duration,name,artist,album,path)VALUES(%1,%2,'%3','%4','%5','%6');";
		query.exec(insert.arg(QDateTime::currentMSecsSinceEpoch()).arg(duration).arg(a[1]).arg(a[0]).arg(a[2]).arg(fileInfo.at(i).filePath()));
	}

}

void Player::PlayAudio()
{
	int* para = new int[2];
	MediaLoader::load("Candy.mp3", para, pFormatCtx);
	if (para[0] == -1)
	{
		qDebug() << "Cannot open input file";
	}
	else if (para[0] == -2)
	{
		qDebug() << "Cannot find any stream in file";
	}
	else
	{
		audioBuf = new RingBuffer<AudioFrame, 50>();
		thDecode = new DecodeThread(pFormatCtx, *(para + 1), audioBuf);
		pThAudio = new QThread();
		PlayTask* pt = new PlayTask(this, audioBuf);
		pt->moveToThread(pThAudio);
		pThAudio->start();
		//QObject::connect(pt, &PlayTask::finish,pt,&PlayTask::deleteLater);
		//QObject::connect(pt, &PlayTask::destroyed, pThAudio, &QThread::quit);
		//QObject::connect(pThAudio, &QThread::finished, pThAudio, &QThread::deleteLater);
		thDecode->start();
		delete[] para;
	}
}

void Player::SelectAudios()
{
	QSqlQuery query(db);
	QStringList filter;
	filter.append("*.mp3");
	QString select = "SELECT* FROM LocalMusic";
	query.exec(select);
	while (query.next())
	{
		qDebug() << query.value(0) << query.value(1) << query.value(2) << query.value(3) << query.value(4);
	}
	//query.exec(select.arg(QDateTime::currentMSecsSinceEpoch()).arg(duration).arg(a[1]).arg(a[0]).arg(a[2]).arg(fileInfo.at(i).filePath()));
}

void Player::InitIcon()
{
	ui->pushButton_2->setIconSize(QSize(48, 48));//暂停/播放
	ui->pushButton_2->setIcon(QIcon(":/image/image/image/pase.png"));
	ui->pushButton_2->setStyleSheet(PaseStyle());
	ui->pushButton_15->setIconSize(QSize(32, 32));//隐藏/显示播放列表
	ui->pushButton_15->setIcon(QIcon(":/image/image/image/left.png"));
	ui->pushButton_15->setStyleSheet(HideListStyle());
	ui->pushButton_9->setIconSize(QSize(32, 32));//播放模式
	ui->pushButton_9->setIcon(QIcon(":/image/image/image/loop.png"));
	ui->pushButton_9->setStyleSheet(LoopStyle());
	ui->pushButton_14->setIconSize(QSize(32, 32));//我喜欢
	ui->pushButton_14->setIcon(QIcon(":/image/image/image/Ilike.png"));
	ui->pushButton_14->setStyleSheet(IlikeStyle());
	ui->pushButton_8->setIconSize(QSize(32, 32));//音量
	ui->pushButton_8->setStyleSheet(VoiceStyle());
	ui->pushButton_8->setIcon(QIcon(":/image/image/image/music-voice.png"));


}

inline QString Player::MusicListStyle() {

	return  "QPushButton"
		" {"
		"background-image:url(:/image/image/image/music.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/image/image/image/music.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/image/image/image/music.png);"

		"}";

}

inline QString Player::MenuStyle()
{
	return     " QMenu {"
		" background: white; "
		"border-radius:2px;"
		"border:1px solid rgb(200,200,200);"

		"  }"
		"  QMenu::item {"

		" background-color: transparent;"
		" padding:5px 30px;"
		"       margin:0px 0px;"
		" border-bottom:0px solid #DBDBDB;"
		"}"
		" QMenu::item:selected { "
		"background-color: aqua;"
		"}";

}

inline QString Player::PlayStyle()
{

	return "QPushButton"
		" {"
		"background-image:url(:/image/image/image/play.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/image/image/image-hover/play-hover.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/image/image/image/play.png);"

		"}";

}
inline QString Player::PaseStyle()
{


	return "QPushButton"
		" {"
		"background-image:url(:/image/image/image/pase.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/image/image/image-hover/pase-hover.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/image/image/image/pase.png);"

		"}";

}
inline QString Player::IlikeStyle()
{


	return "QPushButton"
		" {"
		"background-image:url(:/image/image/image/Ilike.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/image/image/image-hover/Ilike-hover.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/image/image/image/Ilike.png);"

		"}";

}
inline QString Player::Ilike1Style()
{


	return "QPushButton"
		" {"
		"background-image:url(:/image/image/image/Ilike1.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/image/image/image-hover/Ilike1-hover.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/image/image/image/Ilike1.png);"

		"}";

}
inline QString Player::ShowListStyle()
{
	return "QPushButton"
		" {"
		"background-image:url(:/image/image/image/right.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/image/image/image-hover/right-hover.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/image/image/image/right.png);"

		"}";

}
inline QString Player::HideListStyle()
{
	return "QPushButton"
		" {"
		"background-image:url(:/image/image/image/left.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/image/image/image-hover/left-light.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/image/image/image/left.png);"

		"}";
}
inline QString Player::RandomStyle()
{

	return "QPushButton"
		" {"
		"background-image:url(:/image/image/image/play-random.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/image/image/image-hover/play-random-hover.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/image/image/image/play-random.png);"

		"}";

}
inline QString Player::LoopStyle()
{


	return "QPushButton"
		" {"
		"background-image:url(:/image/image/image/loop.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/image/image/image-hover/loop-hover.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/image/image/image/loop.png);"

		"}";

}
inline QString Player::LoopOneStyle()
{


	return "QPushButton"
		" {"
		"background-image:url(:/image/image/image/loop-one.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/image/image/image-hover/loop-one-hover.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/image/image/image/loop-one.png);"

		"}";

}
inline QString Player::VoiceStyle()
{

	return "QPushButton"
		" {"
		"background-image:url(:/image/image/image/music-voice.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/image/image/image-hover/music-voice-hover.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/image/image/image/music-voice.png);"

		"}";
}
inline QString Player::NoVoiceStyle()
{

	return "QPushButton"
		" {"
		"background-image:url(:/image/image/image/none-music.png);"
		"   background-repeat:no-repeat;"
		"  background-position:center center;"
		"   border:none;"

		"    }"

		"QPushButton:hover{"
		"  background-repeat:no-repeat;"
		"   background-position:center center;"
		"background-image:url(:/image/image/image-hover/none-music-hover.png);"

		" }"
		" QPushButton:pressed{"
		"background-repeat:no-repeat;"
		" background-position:center center;"
		" background-image:url(:/image/image/image/none-music.png);"

		"}";
}