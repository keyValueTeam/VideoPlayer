#include "mainwindow.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>
#include "jsonData.h"
#include "MySlider.h"
#include<QUrl>
#include "function.h"
#include <QAction>
#include <QMenu>
#include< QMessageBox>
#include"Musicinfo.h"


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    initWidget(); // 初始化窗口
    mediaPlayerInit(); // 媒体播放初始化
    scanVideoFiles(); // 扫描本地视频
    /* 按钮连接信号槽 */
    connect(pushButton[0], SIGNAL(clicked()), this, SLOT(SltBtnPlayClicked())); // 播放
    connect(pushButton[5], SIGNAL(clicked()), this, SLOT(SltBtnPreClicked())); //上一首
    connect(pushButton[1], SIGNAL(clicked()), this, SLOT(SltBtnNextClicked())); // 下一首
    connect(pushButton[2], SIGNAL(clicked()), this, SLOT(SltBtnVolumeDownClicked())); // 声音减
    connect(pushButton[3], SIGNAL(clicked()), this, SLOT(SltBtnVolumeUpClicked())); // 声音加
    connect(pushButton[4], SIGNAL(clicked()), this, SLOT(SltBtnFullScreenClicked())); // 全屏
    connect(pushButton[6], SIGNAL(clicked()), this, SLOT(SltBtnAddFile()));//添加文件
    connect(pushButton[7], SIGNAL(clicked()), this, SLOT(SltBtnRateClicked()));//倍速
    connect(pushButton[8], SIGNAL(clicked()), this, SLOT(SltBtnChangePlayMode()));   //播放模式
    connect(pushButton[9], SIGNAL(clicked()), this, SLOT(SltBtnClearClicked()));    //清空列表

    connect(videoListWidget->finfo, SIGNAL(triggered()), this, SLOT(finfofun()));
    //connect(videoListWidget->finfo, &QAction::triggered, this, finfofun())

    /* 列表连接信号槽 */
    connect(videoListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SltVideoListWidgetCliked(QListWidgetItem*)));

    /* 媒体连接信号槽 */
    connect(mediaPlaylist, SIGNAL(currentIndexChanged(int)), this, SLOT(SltMediaPlaylistCurrentIndexChanged(int)));
    connect(videoPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(SltMediaPlayerStateChanged(QMediaPlayer::State)));
    connect(videoPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(SltMediaPlayerDurationChanged(qint64)));
    connect(videoPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(SltMediaPlayerPositionChanged(qint64)));

    /* 滚动条连接  */
    connect(playProgressScrollBar, SIGNAL(sliderReleased()), this, SLOT(SltPlayProgressScrollBarReleased()));
    connect(playProgressScrollBar, SIGNAL(costomSliderClicked()), this, SLOT(SltPlayProgressScrollBarClicked()));
    connect(volumeScrollBar, SIGNAL(sliderReleased()), this, SLOT(SltVolumeScrollBarReleased()));
}

MainWindow::~MainWindow()
{
}
/**
 * @brief Widget::initWidget
 * 初始化窗口
 */
void MainWindow::initWidget()
{
    this->setGeometry(0, 0, 800, 480);
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::white); // 设置窗口文字为白色

    /* 构造对象 */
    for (int i = 0; i < 3; i++) {
        /* 水平容器 */
        hWidget[i] = new QWidget();
        hWidget[i]->setAutoFillBackground(true);
        /* 水平布局 */
        hBoxLayout[i] = new QHBoxLayout();
    }
    for (int i = 0; i < 2; i++) {
        /* 垂直容器 */
        vWidget[i] = new QWidget();
        vWidget[i]->setAutoFillBackground(true);
        /* 垂直布局 */
        vBoxLayout[i] = new QVBoxLayout();
    }
    /* 标签 */
    for (int i = 0; i < 2; i++) {
        label[i] = new QLabel();
    }
    /* 按键 */
    for (int i = 0; i < 10; i++) {
        pushButton[i] = new QPushButton();
        pushButton[i]->setMaximumSize(44, 44);
        pushButton[i]->setMinimumSize(44, 44);
    }
    /* 设置按钮的属性 */
    pushButton[0]->setCheckable(true); // 播放
    pushButton[4]->setCheckable(true); // 全屏

    /* 设置样式名  */
    vWidget[0]->setObjectName("vWidget0");
    vWidget[1]->setObjectName("vWidget1");
    hWidget[1]->setObjectName("hWidget1");
    hWidget[2]->setObjectName("hWidget2");
    pushButton[0]->setObjectName("btn_play");
    pushButton[1]->setObjectName("btn_next");
    pushButton[2]->setObjectName("btn_volumedown");
    pushButton[3]->setObjectName("btn_volumeup");
    pushButton[4]->setObjectName("btn_screen");
    pushButton[5]->setObjectName("btn_pre");
    pushButton[6]->setObjectName("btn_addFile");
    pushButton[7]->setObjectName("btn_accelerate");
    pushButton[8]->setObjectName("btn_playMode");
    pushButton[9]->setObjectName("btn_delete");

    //初始化倍速显示
    pushButton[7]->setText(QString::fromLocal8Bit("倍速×1"));
    QFont font;

    font.setPixelSize(18); // 字体大小
    label[0]->setFont(font);
    label[1]->setFont(font);
    pal.setColor(QPalette::WindowText, Qt::white);
    label[0]->setPalette(pal);
    label[1]->setPalette(pal);
    label[0]->setText("00:00"); // 播放时间显示
    label[1]->setText("/00:00");

    /* 播放进度条 */
    playProgressScrollBar = new MySlider(); 
    playProgressScrollBar->setOrientation(Qt::Horizontal);    // 水平方向
    playProgressScrollBar->setMaximumHeight(15);
    playProgressScrollBar->setObjectName("durationSlider");
    /* 声音滚动条 */
    volumeScrollBar = new QSlider(Qt::Horizontal);
    volumeScrollBar->setRange(0, 100);
    volumeScrollBar->setFixedWidth(50);
    volumeScrollBar->setMaximumWidth(80);
    volumeScrollBar->setObjectName("volumeSlider");
    volumeScrollBar->setValue(50); // 中间值
    /* 视频列表 */
    videoListWidget = new MyListWidget();
    videoListWidget->setObjectName("listWidget");
    videoListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏垂直滚动条
    videoListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏水平滚动条

    



    /* 视频窗口 */
    videoWidget = new QVideoWidget();
    videoWidget->setStyleSheet("border-image:none;" "background: transparent;""border:none");
    videoWidget->setMinimumSize(500, 480); // 视频窗口大小
    //videoWidget->setStyleSheet("border-image:url(./icons/music.png);");
    /* 图片窗口*/
    imgWidget = new QWidget();
    imgWidget->setStyleSheet("image-positon:center;" "image:url(./icons/music.png);");

    //禁用默认快捷键
    videoListWidget->setFocusPolicy(Qt::NoFocus);
    playProgressScrollBar->setFocusPolicy(Qt::NoFocus);
    volumeScrollBar->setFocusPolicy(Qt::NoFocus);
    for (int i = 0; i < 10; ++i)
    {
        pushButton[i]->setFocusPolicy(Qt::NoFocus);
    }


    /* 下面是界面布局 */
    /* H0 布局 */
    vWidget[0]->setMinimumSize(300, 480);
    vWidget[0]->setMaximumWidth(300);

    hBoxLayout[0]->addWidget(videoWidget); // 视频窗口 左边
    //videoWidget->hide();
    hBoxLayout[0]->addWidget(imgWidget);   //图片窗口
    imgWidget->hide();
    hBoxLayout[0]->addWidget(vWidget[0]); // 右边 垂直窗口

    hWidget[0]->setLayout(hBoxLayout[0]);
    hBoxLayout[0]->setContentsMargins(0, 0, 0, 0);

    setCentralWidget(hWidget[0]); // 只有Qmainwindow才能创建中心布局Qwidget不行

    /* V0 布局 */
    QSpacerItem* vSpacer0 = new QSpacerItem(0, 80, QSizePolicy::Minimum, QSizePolicy::Maximum);
    vBoxLayout[0]->addWidget(videoListWidget); // 添加视频列表
    vBoxLayout[0]->addSpacerItem(vSpacer0);
    vBoxLayout[0]->setContentsMargins(0, 0, 0, 0);
    vWidget[0]->setLayout(vBoxLayout[0]);

    /* V1 布局 */
    /* 底板部件布局 */
    hWidget[1]->setMaximumHeight(15);
    hWidget[2]->setMinimumHeight(55);
    vBoxLayout[1]->addWidget(hWidget[1]);
    vBoxLayout[1]->addWidget(hWidget[2]);
    vBoxLayout[1]->setAlignment(Qt::AlignCenter);

    vWidget[1]->setLayout(vBoxLayout[1]);
    vWidget[1]->setParent(this);
    vWidget[1]->setGeometry(0, this->height() - 80, this->width(), 80);
    vBoxLayout[1]->setContentsMargins(0, 0, 0, 0);
    /* 位于最上层 */
    vWidget[1]->raise();

    /* H1 布局 */
    hBoxLayout[1]->addWidget(playProgressScrollBar); // 播放进度条
    hBoxLayout[1]->setContentsMargins(0, 0, 0, 0);
    hWidget[1]->setLayout(hBoxLayout[1]);

    /* H2 布局 */
    QSpacerItem* hSpacer0 = new QSpacerItem(300, 80, QSizePolicy::Expanding, QSizePolicy::Maximum);

    hBoxLayout[2]->addSpacing(10);
    hBoxLayout[2]->addWidget(pushButton[8]);
    hBoxLayout[2]->addSpacing(10);
    hBoxLayout[2]->addWidget(pushButton[5]);
    hBoxLayout[2]->addSpacing(10);
    hBoxLayout[2]->addWidget(pushButton[0]);
    hBoxLayout[2]->addSpacing(10);
    hBoxLayout[2]->addWidget(pushButton[1]);
    hBoxLayout[2]->addSpacing(10);
    hBoxLayout[2]->addWidget(pushButton[2]);
    hBoxLayout[2]->addWidget(volumeScrollBar);
    hBoxLayout[2]->addWidget(pushButton[3]);
    hBoxLayout[2]->addWidget(label[0]);
    hBoxLayout[2]->addWidget(label[1]);
    hBoxLayout[2]->addSpacerItem(hSpacer0);
    hBoxLayout[2]->addWidget(pushButton[7]);
    hBoxLayout[2]->addSpacing(10);
    hBoxLayout[2]->addWidget(pushButton[6]);
    hBoxLayout[2]->addSpacing(10);
    hBoxLayout[2]->addWidget(pushButton[9]);
    hBoxLayout[2]->addSpacing(10);
    hBoxLayout[2]->addWidget(pushButton[4]);
    hBoxLayout[2]->addSpacing(20);
    hBoxLayout[2]->setContentsMargins(0, 0, 0, 0);
    hBoxLayout[2]->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    hWidget[2]->setLayout(hBoxLayout[2]);
}

/**
 * @brief MainWindow::mediaPlayerInit
 * 媒体初始化
 */
void MainWindow::mediaPlayerInit()
{
    videoPlayer = new QMediaPlayer(this);
    mediaPlaylist = new QMediaPlaylist(this); // 播放列表
    mediaPlaylist->clear();
    videoPlayer->setPlaylist(mediaPlaylist);
    videoPlayer->setVideoOutput(videoWidget); // 设置视频输出窗口
    mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop); // 设置播放模式，Loop 是列循环
    videoPlayer->setVolume(50); // 设置默认软件音量为 50%
}

/**
 * @brief MainWindow::scanVideoFiles
 * 扫描视频文件
 */
void MainWindow::scanVideoFiles()
{
    
    //    QDir dir(QCoreApplication::applicationDirPath()+ "/myVideo");
    //QDir dir(videoFilePath); // 视频文件路径
    //QDir dirbsolutePath(dir.absolutePath()); // 绝对路径
//    qDebug() << dirbsolutePath << endl;
    /*
    if (dirbsolutePath.exists()) { // 如果目录存在
        QStringList filter; // 定义过滤器
    */
        /* 包含所有 xx 后缀的文件 */
    /*    
        filter << "*.mp4" << "*.mkv" << "*.wmv" << "*.avi" << "*.mp3";
        // 获取该目录下的所有文件 //
        QFileInfoList files = dirbsolutePath.entryInfoList(filter, QDir::Files);
    */
    vector<string> filePaths = getAllPath();   //读取json所有文件路径
    vector<string> fileNames = getAllName();   //读取json所有文件名
    if (filePaths.size() != 0)
    {
        for (int i = 0; i < filePaths.size(); i++) {
            MediaObjectInfo info;
            cout << fileNames[i] << "   *  " << filePaths[i] << endl;
            /* 使用 utf-8 编码 */
            info.fileName = QString(QString::fromLocal8Bit(fileNames[i].c_str()));
            info.filePath = QString(QString::fromLocal8Bit(filePaths[i].c_str()));
            /* 媒体列表添加视频 */
            if (mediaPlaylist->addMedia(
                QUrl::fromLocalFile(info.filePath))) {
                /* 添加到容器数组里储存 */
                //mediaObjectInfo.append(info);
                /* 添加视频名字至列表 */
                videoListWidget->addItem(info.fileName);
            }
            else {
                qDebug() << mediaPlaylist->errorString().toUtf8().data() << endl;
                qDebug() << " Error number:" << mediaPlaylist->error() << endl;
            }
        }
    }
    else {
        qDebug() << "json数据内容为空 " << endl;
    }
}
/**
* @brief MainWindow::SltBtnDelClicked();   
* 清空列表
*/
void MainWindow::SltBtnClearClicked()
{
    mediaPlaylist->clear();
    videoListWidget->clear();
    jsonFileClear();
}
/**
 * @brief MainWindow::SltBtnPlayClicked
 * 播放点击
 */
void MainWindow::SltBtnPlayClicked()
{
    int state = videoPlayer->state(); // 获取播放的状态
    switch (state) {
    case QMediaPlayer::StoppedState:
        /* 媒体播放 */
        videoPlayer->play();
        break;
    case QMediaPlayer::PlayingState:
        /* 媒体暂停 */
        videoPlayer->pause();
        break;
    case QMediaPlayer::PausedState:
        videoPlayer->play();
        break;
    }
}

/**
*@brief MainWindow::SletBtnPreClicked();
* 上一首点击
*/
void MainWindow::SltBtnPreClicked()
{
    videoPlayer->stop();    //停止播放
    int count = mediaPlaylist->mediaCount();   //获取播放列表视频个数
    if (0 == count)
        return;
    mediaPlaylist->previous();
    videoPlayer->play();
    //videoWidget->hide();
    //backGround->show();
}
/**
 * @brief MainWindow::SltBtnNextClicked
 * 下一首点击
 */
void MainWindow::SltBtnNextClicked()
{
    videoPlayer->stop(); // 停止播放
    int count = mediaPlaylist->mediaCount(); // 获取播放列表的视频个数
    if (0 == count)
        return;
    /* 列表下一个 */
    mediaPlaylist->next();
    videoPlayer->play();
}

/**
 * @brief MainWindow::SltBtnVolumeDownClicked
 * 声音减点击
 */
void MainWindow::SltBtnVolumeDownClicked()
{
    /* 点击每次音量-5 */
    volumeScrollBar->setValue(volumeScrollBar->value() - 5);
    videoPlayer->setVolume(volumeScrollBar->value());
}
/**
 * @brief MainWindow::SltBtnVolumeUpClicked
 * 声音加点击
 */
void MainWindow::SltBtnVolumeUpClicked()
{
    /* 点击每次音量+5 */
    volumeScrollBar->setValue(volumeScrollBar->value() + 5);
    videoPlayer->setVolume(volumeScrollBar->value());
}

/**
 * @brief MainWindow::SltBtnFullScreenClicked
 * 全屏点击
 */
void MainWindow::SltBtnFullScreenClicked()
{
    /* 全屏/非全屏操作 */
    cout << "全屏" << endl;
    vWidget[0]->setVisible(!pushButton[4]->isChecked());
}

/**
 * @brief MainWindow::SltVideoListWidgetCliked
 * 视频列表点击
 * @param item
 */
void MainWindow::SltVideoListWidgetCliked(QListWidgetItem* item)
{
    videoPlayer->stop();
    mediaPlaylist->setCurrentIndex(videoListWidget->row(item)); // 选中点击的项
    //QListWidget str = videoListWidget -> currentItem();
    videoPlayer->play();
}

/**
 * @brief MainWindow::SltMediaPlaylistCurrentIndexChanged
 * 媒体当前项发生改变 选中播放列表
 * @param index
 */
void MainWindow::SltMediaPlaylistCurrentIndexChanged(int index)
{
    if (-1 == index)
        return;

    /* 设置列表正在播放的项 */
    videoListWidget->setCurrentRow(index);
    QMediaContent temp = mediaPlaylist->currentMedia();
  
    //mediaPlaylist->currentIndex();
    QString filePath = temp.canonicalUrl().toString();
    if (isMP3(filePath))
    {
        videoWidget->hide();
        imgWidget->show();
    }
    else
    {
        videoWidget->show();
        imgWidget->hide();
    }
}

/**
 * @brief MainWindow::SltMediaPlayerStateChanged
 * 媒体播放状态发生改变
 * @param state
 */
void MainWindow::SltMediaPlayerStateChanged(QMediaPlayer::State state)
{
    switch (state) {
    case QMediaPlayer::StoppedState:
        pushButton[0]->setChecked(false);
        break;
    case QMediaPlayer::PlayingState:
        pushButton[0]->setChecked(true);
        break;
    case QMediaPlayer::PausedState:
        pushButton[0]->setChecked(false);
        break;
    }
}
/**
 * @brief MainWindow::SltMediaPlayerDurationChanged
 * 媒体播放时长发生变化
 * @param duration
 */
void MainWindow::SltMediaPlayerDurationChanged(qint64 duration)
{
    playProgressScrollBar->setRange(0, duration / 1000); // 设置播放进度条的范围
    int second = duration / 1000;
    int minute = second / 60;
    second %= 60;

    QString mediaDuration;
    mediaDuration.clear();

    if (minute >= 10)
        mediaDuration = QString::number(minute, 10);
    else
        mediaDuration = "0" + QString::number(minute, 10);

    if (second >= 10)
        mediaDuration = mediaDuration + ":" + QString::number(second, 10);
    else
        mediaDuration = mediaDuration + ":0" + QString::number(second, 10);

    /* 显示媒体总长度时间 */
    label[1]->setText("/" + mediaDuration);
}
/**
 * @brief MainWindow::SltMediaPlayerPositionChanged
 * 媒体当前播放时间发生变化
 * @param position
 */
void MainWindow::SltMediaPlayerPositionChanged(qint64 position)
{
    if (!playProgressScrollBar->isSliderDown()) // 进度条没有按下
        playProgressScrollBar->setValue(position / 1000);

    int second = position / 1000;
    int minute = second / 60;
    second %= 60;

    QString mediaPosition;
    mediaPosition.clear();

    if (minute >= 10)
        mediaPosition = QString::number(minute, 10);
    else
        mediaPosition = "0" + QString::number(minute, 10);

    if (second >= 10)
        mediaPosition = mediaPosition + ":" + QString::number(second, 10);
    else
        mediaPosition = mediaPosition + ":0" + QString::number(second, 10);

    /* 显示现在播放的时间 */
    label[0]->setText(mediaPosition);
}
/**
 * @brief MainWindow::SltPlayProgressScrollBarReleased
 * 播放进度条释放
 */
void MainWindow::SltPlayProgressScrollBarReleased()
{
    /* 设置媒体播放的位置 */
    videoPlayer->setPosition(playProgressScrollBar->value() * 1000);
}

/**
 * @brief MainWindow::SltPlayProgressScrollBarClicked
 * 点击播放进度条
 */
void MainWindow::SltPlayProgressScrollBarClicked()
{
    /* 设置媒体播放的位置 */
    videoPlayer->setPosition(playProgressScrollBar->value() * 1000);
}
/**
 * @brief MainWindow::SltVolumeScrollBarReleased
 * 音量进度条释放
 */
void MainWindow::SltVolumeScrollBarReleased()
{
    /* 设置音量 */
    videoPlayer->setVolume(volumeScrollBar->value());
}
/**
* @brief MainWindow::SltBtnAddFile();
* 添加文件到播放器
*/
void MainWindow::SltBtnAddFile()
{
    QStringList filter; // 定义过滤器
        /* 包含所有 xx 后缀的文件 */
    filter << "*.mp4" << "*.mkv" << "*.wmv" << "*.avi" << "*.flv" << "*.mp3";
    QStringList filePaths = QFileDialog::getOpenFileNames(this, "选择多个文件","./", "*.mp4 *.mkv *.wmv *.avi *.flv *.mp3");

    for (int i = 0; i < filePaths.count(); ++i)
    {
        QFileInfo info = QFileInfo(filePaths.at(i));   //获取文件信息
        //qDebug() << info.fileName() << "  " << info.filePath() << endl;
        bool flag = addFile((const char*)info.fileName().toLocal8Bit(), (const char*)info.filePath().toLocal8Bit());
        if (flag == false) continue;
        MediaObjectInfo MediaInfo;   
        MediaInfo.fileName = QString::fromUtf8(info.fileName().toUtf8().data());
        MediaInfo.filePath = QString::fromUtf8(info.filePath().toUtf8().data());
        /*媒体列表添加视频*/
        if (mediaPlaylist->addMedia(QUrl::fromLocalFile(MediaInfo.filePath)))
        {
            //添加文件名字到列表
            videoListWidget->addItem(MediaInfo.fileName);
        }
        else
        {
            qDebug() << filePaths.at(i) << endl;
        }
    }
}
/**
* @brief MainWindow::SltBtnRateClicked();
* 改变播放速率
*/
void MainWindow::SltBtnRateClicked()
{
    if (rate == 1)
    {
        videoPlayer->pause();
        pushButton[7]->setText(QString::fromLocal8Bit("倍速×2"));
        videoPlayer->setPlaybackRate(2);
        rate *= 2;
        videoPlayer->play();
    }
    else if (rate == 2)
    {
        videoPlayer->pause();
        pushButton[7]->setText(QString::fromLocal8Bit("倍速×4"));
        videoPlayer->setPlaybackRate(4);
        rate *= 2;
        videoPlayer->play();
    }
    else if (rate == 4)
    {
        videoPlayer->pause();
        pushButton[7]->setText(QString::fromLocal8Bit("倍速×8"));
        videoPlayer->setPlaybackRate(8);
        rate = 8;
        videoPlayer->play();
    }
    else if (rate == 8)
    {
        videoPlayer->pause();
        pushButton[7]->setText(QString::fromLocal8Bit("倍速×1"));
        videoPlayer->setPlaybackRate(1);
        rate =1;
        videoPlayer->play();
    }
}
/**
 * @brief MainWindow::SltBtnChangePlayMode();
 * 改变播放模式
 * @param mode
 */
void MainWindow::SltBtnChangePlayMode()
{
    //随机播放
    if (mediaPlaylist->playbackMode() == QMediaPlaylist::Loop)   
    {
        mediaPlaylist->setPlaybackMode(QMediaPlaylist::Random);  
        pushButton[8]->setStyleSheet("QPushButton{border-image:url(./icons/btn_playRandom1.png);} QPushButton:hover{border-image:url(./icons/btn_playRandom2.png);}");
    }
    //循环播放
    else if (mediaPlaylist->playbackMode() == QMediaPlaylist::Random)
    {
        mediaPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        pushButton[8]->setStyleSheet("QPushButton{border-image:url(./icons/btn_playLoopCurrent1.png);} QPushButton:hover{border-image:url(./icons/btn_playLoopCurrent2.png);}");
    }
    //只播放一次
    else if (mediaPlaylist->playbackMode() == QMediaPlaylist::CurrentItemInLoop)
    {
        mediaPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        pushButton[8]->setStyleSheet("QPushButton{border-image:url(./icons/btn_playOnceCurrent1.png);} QPushButton:hover{border-image:url(./icons/btn_playOnceCurrent2.png);}");
    }
    //顺序循环播放
    else if (mediaPlaylist->playbackMode() == QMediaPlaylist::CurrentItemOnce)
    {
        mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
        pushButton[8]->setStyleSheet("QPushButton{border-image:url(./icons/btn_playLoopList1.png);} QPushButton:hover{border-image:url(./icons/btn_playLoopList2.png);}");
    }
    
}
/**
 * @brief MainWindow::keyPressEvent
 * 快捷键事件
 * @param event
 */
void MainWindow::keyPressEvent(QKeyEvent* event)
{
    //暂停
    if (event->key() == Qt::Key_Space)
    {
        SltBtnPlayClicked();
        cout << "按下空格键" << endl;
    }
    //全屏
    if (event->key() == Qt::Key_Escape)
    {
        if (vWidget[1]->isVisible())
        {
            vWidget[1]->hide();
        }
        else
        {
            vWidget[1]->show();
        }
        cout << "隐藏底部栏" << endl;
    }
    //ctrl左键——上一曲
    if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_Left)
    {
        SltBtnPreClicked();
        cout << "按下Ctrl+左键" << endl;
    }
    //ctrl右键——下一曲
    if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_Right)
    {
        SltBtnNextClicked();
        cout << "按下Ctrl+右键" << endl;
    }
    //音量增加
    if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_Up)
    {
        SltBtnVolumeUpClicked();
        cout << "按下Ctrl+上键" << endl;
    }
    //音量减少
    if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_Down)
    {
        SltBtnVolumeDownClicked();
        cout << "按下Ctrl+下键" << endl;
    } 
    //全屏
    if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_F)
    {
        pushButton[4]->click();   //设置全屏
        //SltBtnFullScreenClicked();
        cout << "按下CTRL+F" << endl;
    }
    //唤起资源弹窗
    if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_I)
    {
        SltBtnAddFile();
        cout << "按下CTRL+I" << endl;
    }
}
/**
 * @brief MainWindow::resizeEvent
 * 重绘窗口
 * @param event
 */
void MainWindow::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    vWidget[1]->setGeometry(0, this->height() - 80, this->width(), 80);
}


void MainWindow::finfofun(void)      //获取文件信息
{
    MP3Header* mp3header = new MP3Header();
    QString Murl = videoListWidget->musicItem->text();
    string musicPath = getPathByName(Murl.toStdString());

    MP3INFO mp3info = mp3header->GetAllInfo(QString::fromStdString(musicPath).toStdWString().c_str(), 10);
    //MP3INFO mp3info = mp3header->GetAllInfo(L"./myVideo/Alone-Alan Walker.mp3", 9);
    QMessageBox::about(this, u8"歌曲信息", QString(u8"歌曲名 : %1  \n"
        u8"作者 : %2 \n"
        u8"时长 : %3 \n"
        u8"文件大小 : %4\n").arg(mp3info.Name).arg(mp3info.Singer).arg("00:00").arg(mp3info.lenth));
}