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
    initWidget(); // ��ʼ������
    mediaPlayerInit(); // ý�岥�ų�ʼ��
    scanVideoFiles(); // ɨ�豾����Ƶ
    /* ��ť�����źŲ� */
    connect(pushButton[0], SIGNAL(clicked()), this, SLOT(SltBtnPlayClicked())); // ����
    connect(pushButton[5], SIGNAL(clicked()), this, SLOT(SltBtnPreClicked())); //��һ��
    connect(pushButton[1], SIGNAL(clicked()), this, SLOT(SltBtnNextClicked())); // ��һ��
    connect(pushButton[2], SIGNAL(clicked()), this, SLOT(SltBtnVolumeDownClicked())); // ������
    connect(pushButton[3], SIGNAL(clicked()), this, SLOT(SltBtnVolumeUpClicked())); // ������
    connect(pushButton[4], SIGNAL(clicked()), this, SLOT(SltBtnFullScreenClicked())); // ȫ��
    connect(pushButton[6], SIGNAL(clicked()), this, SLOT(SltBtnAddFile()));//����ļ�
    connect(pushButton[7], SIGNAL(clicked()), this, SLOT(SltBtnRateClicked()));//����
    connect(pushButton[8], SIGNAL(clicked()), this, SLOT(SltBtnChangePlayMode()));   //����ģʽ
    connect(pushButton[9], SIGNAL(clicked()), this, SLOT(SltBtnClearClicked()));    //����б�

    connect(videoListWidget->finfo, SIGNAL(triggered()), this, SLOT(finfofun()));
    //connect(videoListWidget->finfo, &QAction::triggered, this, finfofun())

    /* �б������źŲ� */
    connect(videoListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SltVideoListWidgetCliked(QListWidgetItem*)));

    /* ý�������źŲ� */
    connect(mediaPlaylist, SIGNAL(currentIndexChanged(int)), this, SLOT(SltMediaPlaylistCurrentIndexChanged(int)));
    connect(videoPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(SltMediaPlayerStateChanged(QMediaPlayer::State)));
    connect(videoPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(SltMediaPlayerDurationChanged(qint64)));
    connect(videoPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(SltMediaPlayerPositionChanged(qint64)));

    /* ����������  */
    connect(playProgressScrollBar, SIGNAL(sliderReleased()), this, SLOT(SltPlayProgressScrollBarReleased()));
    connect(playProgressScrollBar, SIGNAL(costomSliderClicked()), this, SLOT(SltPlayProgressScrollBarClicked()));
    connect(volumeScrollBar, SIGNAL(sliderReleased()), this, SLOT(SltVolumeScrollBarReleased()));
}

MainWindow::~MainWindow()
{
}
/**
 * @brief Widget::initWidget
 * ��ʼ������
 */
void MainWindow::initWidget()
{
    this->setGeometry(0, 0, 800, 480);
    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::white); // ���ô�������Ϊ��ɫ

    /* ������� */
    for (int i = 0; i < 3; i++) {
        /* ˮƽ���� */
        hWidget[i] = new QWidget();
        hWidget[i]->setAutoFillBackground(true);
        /* ˮƽ���� */
        hBoxLayout[i] = new QHBoxLayout();
    }
    for (int i = 0; i < 2; i++) {
        /* ��ֱ���� */
        vWidget[i] = new QWidget();
        vWidget[i]->setAutoFillBackground(true);
        /* ��ֱ���� */
        vBoxLayout[i] = new QVBoxLayout();
    }
    /* ��ǩ */
    for (int i = 0; i < 2; i++) {
        label[i] = new QLabel();
    }
    /* ���� */
    for (int i = 0; i < 10; i++) {
        pushButton[i] = new QPushButton();
        pushButton[i]->setMaximumSize(44, 44);
        pushButton[i]->setMinimumSize(44, 44);
    }
    /* ���ð�ť������ */
    pushButton[0]->setCheckable(true); // ����
    pushButton[4]->setCheckable(true); // ȫ��

    /* ������ʽ��  */
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

    //��ʼ��������ʾ
    pushButton[7]->setText(QString::fromLocal8Bit("���١�1"));
    QFont font;

    font.setPixelSize(18); // �����С
    label[0]->setFont(font);
    label[1]->setFont(font);
    pal.setColor(QPalette::WindowText, Qt::white);
    label[0]->setPalette(pal);
    label[1]->setPalette(pal);
    label[0]->setText("00:00"); // ����ʱ����ʾ
    label[1]->setText("/00:00");

    /* ���Ž����� */
    playProgressScrollBar = new MySlider(); 
    playProgressScrollBar->setOrientation(Qt::Horizontal);    // ˮƽ����
    playProgressScrollBar->setMaximumHeight(15);
    playProgressScrollBar->setObjectName("durationSlider");
    /* ���������� */
    volumeScrollBar = new QSlider(Qt::Horizontal);
    volumeScrollBar->setRange(0, 100);
    volumeScrollBar->setFixedWidth(50);
    volumeScrollBar->setMaximumWidth(80);
    volumeScrollBar->setObjectName("volumeSlider");
    volumeScrollBar->setValue(50); // �м�ֵ
    /* ��Ƶ�б� */
    videoListWidget = new MyListWidget();
    videoListWidget->setObjectName("listWidget");
    videoListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // ���ش�ֱ������
    videoListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // ����ˮƽ������

    



    /* ��Ƶ���� */
    videoWidget = new QVideoWidget();
    videoWidget->setStyleSheet("border-image:none;" "background: transparent;""border:none");
    videoWidget->setMinimumSize(500, 480); // ��Ƶ���ڴ�С
    //videoWidget->setStyleSheet("border-image:url(./icons/music.png);");
    /* ͼƬ����*/
    imgWidget = new QWidget();
    imgWidget->setStyleSheet("image-positon:center;" "image:url(./icons/music.png);");

    //����Ĭ�Ͽ�ݼ�
    videoListWidget->setFocusPolicy(Qt::NoFocus);
    playProgressScrollBar->setFocusPolicy(Qt::NoFocus);
    volumeScrollBar->setFocusPolicy(Qt::NoFocus);
    for (int i = 0; i < 10; ++i)
    {
        pushButton[i]->setFocusPolicy(Qt::NoFocus);
    }


    /* �����ǽ��沼�� */
    /* H0 ���� */
    vWidget[0]->setMinimumSize(300, 480);
    vWidget[0]->setMaximumWidth(300);

    hBoxLayout[0]->addWidget(videoWidget); // ��Ƶ���� ���
    //videoWidget->hide();
    hBoxLayout[0]->addWidget(imgWidget);   //ͼƬ����
    imgWidget->hide();
    hBoxLayout[0]->addWidget(vWidget[0]); // �ұ� ��ֱ����

    hWidget[0]->setLayout(hBoxLayout[0]);
    hBoxLayout[0]->setContentsMargins(0, 0, 0, 0);

    setCentralWidget(hWidget[0]); // ֻ��Qmainwindow���ܴ������Ĳ���Qwidget����

    /* V0 ���� */
    QSpacerItem* vSpacer0 = new QSpacerItem(0, 80, QSizePolicy::Minimum, QSizePolicy::Maximum);
    vBoxLayout[0]->addWidget(videoListWidget); // �����Ƶ�б�
    vBoxLayout[0]->addSpacerItem(vSpacer0);
    vBoxLayout[0]->setContentsMargins(0, 0, 0, 0);
    vWidget[0]->setLayout(vBoxLayout[0]);

    /* V1 ���� */
    /* �װ岿������ */
    hWidget[1]->setMaximumHeight(15);
    hWidget[2]->setMinimumHeight(55);
    vBoxLayout[1]->addWidget(hWidget[1]);
    vBoxLayout[1]->addWidget(hWidget[2]);
    vBoxLayout[1]->setAlignment(Qt::AlignCenter);

    vWidget[1]->setLayout(vBoxLayout[1]);
    vWidget[1]->setParent(this);
    vWidget[1]->setGeometry(0, this->height() - 80, this->width(), 80);
    vBoxLayout[1]->setContentsMargins(0, 0, 0, 0);
    /* λ�����ϲ� */
    vWidget[1]->raise();

    /* H1 ���� */
    hBoxLayout[1]->addWidget(playProgressScrollBar); // ���Ž�����
    hBoxLayout[1]->setContentsMargins(0, 0, 0, 0);
    hWidget[1]->setLayout(hBoxLayout[1]);

    /* H2 ���� */
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
 * ý���ʼ��
 */
void MainWindow::mediaPlayerInit()
{
    videoPlayer = new QMediaPlayer(this);
    mediaPlaylist = new QMediaPlaylist(this); // �����б�
    mediaPlaylist->clear();
    videoPlayer->setPlaylist(mediaPlaylist);
    videoPlayer->setVideoOutput(videoWidget); // ������Ƶ�������
    mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop); // ���ò���ģʽ��Loop ����ѭ��
    videoPlayer->setVolume(50); // ����Ĭ���������Ϊ 50%
}

/**
 * @brief MainWindow::scanVideoFiles
 * ɨ����Ƶ�ļ�
 */
void MainWindow::scanVideoFiles()
{
    
    //    QDir dir(QCoreApplication::applicationDirPath()+ "/myVideo");
    //QDir dir(videoFilePath); // ��Ƶ�ļ�·��
    //QDir dirbsolutePath(dir.absolutePath()); // ����·��
//    qDebug() << dirbsolutePath << endl;
    /*
    if (dirbsolutePath.exists()) { // ���Ŀ¼����
        QStringList filter; // ���������
    */
        /* �������� xx ��׺���ļ� */
    /*    
        filter << "*.mp4" << "*.mkv" << "*.wmv" << "*.avi" << "*.mp3";
        // ��ȡ��Ŀ¼�µ������ļ� //
        QFileInfoList files = dirbsolutePath.entryInfoList(filter, QDir::Files);
    */
    vector<string> filePaths = getAllPath();   //��ȡjson�����ļ�·��
    vector<string> fileNames = getAllName();   //��ȡjson�����ļ���
    if (filePaths.size() != 0)
    {
        for (int i = 0; i < filePaths.size(); i++) {
            MediaObjectInfo info;
            cout << fileNames[i] << "   *  " << filePaths[i] << endl;
            /* ʹ�� utf-8 ���� */
            info.fileName = QString(QString::fromLocal8Bit(fileNames[i].c_str()));
            info.filePath = QString(QString::fromLocal8Bit(filePaths[i].c_str()));
            /* ý���б������Ƶ */
            if (mediaPlaylist->addMedia(
                QUrl::fromLocalFile(info.filePath))) {
                /* ��ӵ����������ﴢ�� */
                //mediaObjectInfo.append(info);
                /* �����Ƶ�������б� */
                videoListWidget->addItem(info.fileName);
            }
            else {
                qDebug() << mediaPlaylist->errorString().toUtf8().data() << endl;
                qDebug() << " Error number:" << mediaPlaylist->error() << endl;
            }
        }
    }
    else {
        qDebug() << "json��������Ϊ�� " << endl;
    }
}
/**
* @brief MainWindow::SltBtnDelClicked();   
* ����б�
*/
void MainWindow::SltBtnClearClicked()
{
    mediaPlaylist->clear();
    videoListWidget->clear();
    jsonFileClear();
}
/**
 * @brief MainWindow::SltBtnPlayClicked
 * ���ŵ��
 */
void MainWindow::SltBtnPlayClicked()
{
    int state = videoPlayer->state(); // ��ȡ���ŵ�״̬
    switch (state) {
    case QMediaPlayer::StoppedState:
        /* ý�岥�� */
        videoPlayer->play();
        break;
    case QMediaPlayer::PlayingState:
        /* ý����ͣ */
        videoPlayer->pause();
        break;
    case QMediaPlayer::PausedState:
        videoPlayer->play();
        break;
    }
}

/**
*@brief MainWindow::SletBtnPreClicked();
* ��һ�׵��
*/
void MainWindow::SltBtnPreClicked()
{
    videoPlayer->stop();    //ֹͣ����
    int count = mediaPlaylist->mediaCount();   //��ȡ�����б���Ƶ����
    if (0 == count)
        return;
    mediaPlaylist->previous();
    videoPlayer->play();
    //videoWidget->hide();
    //backGround->show();
}
/**
 * @brief MainWindow::SltBtnNextClicked
 * ��һ�׵��
 */
void MainWindow::SltBtnNextClicked()
{
    videoPlayer->stop(); // ֹͣ����
    int count = mediaPlaylist->mediaCount(); // ��ȡ�����б����Ƶ����
    if (0 == count)
        return;
    /* �б���һ�� */
    mediaPlaylist->next();
    videoPlayer->play();
}

/**
 * @brief MainWindow::SltBtnVolumeDownClicked
 * ���������
 */
void MainWindow::SltBtnVolumeDownClicked()
{
    /* ���ÿ������-5 */
    volumeScrollBar->setValue(volumeScrollBar->value() - 5);
    videoPlayer->setVolume(volumeScrollBar->value());
}
/**
 * @brief MainWindow::SltBtnVolumeUpClicked
 * �����ӵ��
 */
void MainWindow::SltBtnVolumeUpClicked()
{
    /* ���ÿ������+5 */
    volumeScrollBar->setValue(volumeScrollBar->value() + 5);
    videoPlayer->setVolume(volumeScrollBar->value());
}

/**
 * @brief MainWindow::SltBtnFullScreenClicked
 * ȫ�����
 */
void MainWindow::SltBtnFullScreenClicked()
{
    /* ȫ��/��ȫ������ */
    cout << "ȫ��" << endl;
    vWidget[0]->setVisible(!pushButton[4]->isChecked());
}

/**
 * @brief MainWindow::SltVideoListWidgetCliked
 * ��Ƶ�б���
 * @param item
 */
void MainWindow::SltVideoListWidgetCliked(QListWidgetItem* item)
{
    videoPlayer->stop();
    mediaPlaylist->setCurrentIndex(videoListWidget->row(item)); // ѡ�е������
    //QListWidget str = videoListWidget -> currentItem();
    videoPlayer->play();
}

/**
 * @brief MainWindow::SltMediaPlaylistCurrentIndexChanged
 * ý�嵱ǰ����ı� ѡ�в����б�
 * @param index
 */
void MainWindow::SltMediaPlaylistCurrentIndexChanged(int index)
{
    if (-1 == index)
        return;

    /* �����б����ڲ��ŵ��� */
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
 * ý�岥��״̬�����ı�
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
 * ý�岥��ʱ�������仯
 * @param duration
 */
void MainWindow::SltMediaPlayerDurationChanged(qint64 duration)
{
    playProgressScrollBar->setRange(0, duration / 1000); // ���ò��Ž������ķ�Χ
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

    /* ��ʾý���ܳ���ʱ�� */
    label[1]->setText("/" + mediaDuration);
}
/**
 * @brief MainWindow::SltMediaPlayerPositionChanged
 * ý�嵱ǰ����ʱ�䷢���仯
 * @param position
 */
void MainWindow::SltMediaPlayerPositionChanged(qint64 position)
{
    if (!playProgressScrollBar->isSliderDown()) // ������û�а���
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

    /* ��ʾ���ڲ��ŵ�ʱ�� */
    label[0]->setText(mediaPosition);
}
/**
 * @brief MainWindow::SltPlayProgressScrollBarReleased
 * ���Ž������ͷ�
 */
void MainWindow::SltPlayProgressScrollBarReleased()
{
    /* ����ý�岥�ŵ�λ�� */
    videoPlayer->setPosition(playProgressScrollBar->value() * 1000);
}

/**
 * @brief MainWindow::SltPlayProgressScrollBarClicked
 * ������Ž�����
 */
void MainWindow::SltPlayProgressScrollBarClicked()
{
    /* ����ý�岥�ŵ�λ�� */
    videoPlayer->setPosition(playProgressScrollBar->value() * 1000);
}
/**
 * @brief MainWindow::SltVolumeScrollBarReleased
 * �����������ͷ�
 */
void MainWindow::SltVolumeScrollBarReleased()
{
    /* �������� */
    videoPlayer->setVolume(volumeScrollBar->value());
}
/**
* @brief MainWindow::SltBtnAddFile();
* ����ļ���������
*/
void MainWindow::SltBtnAddFile()
{
    QStringList filter; // ���������
        /* �������� xx ��׺���ļ� */
    filter << "*.mp4" << "*.mkv" << "*.wmv" << "*.avi" << "*.flv" << "*.mp3";
    QStringList filePaths = QFileDialog::getOpenFileNames(this, "ѡ�����ļ�","./", "*.mp4 *.mkv *.wmv *.avi *.flv *.mp3");

    for (int i = 0; i < filePaths.count(); ++i)
    {
        QFileInfo info = QFileInfo(filePaths.at(i));   //��ȡ�ļ���Ϣ
        //qDebug() << info.fileName() << "  " << info.filePath() << endl;
        bool flag = addFile((const char*)info.fileName().toLocal8Bit(), (const char*)info.filePath().toLocal8Bit());
        if (flag == false) continue;
        MediaObjectInfo MediaInfo;   
        MediaInfo.fileName = QString::fromUtf8(info.fileName().toUtf8().data());
        MediaInfo.filePath = QString::fromUtf8(info.filePath().toUtf8().data());
        /*ý���б������Ƶ*/
        if (mediaPlaylist->addMedia(QUrl::fromLocalFile(MediaInfo.filePath)))
        {
            //����ļ����ֵ��б�
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
* �ı䲥������
*/
void MainWindow::SltBtnRateClicked()
{
    if (rate == 1)
    {
        videoPlayer->pause();
        pushButton[7]->setText(QString::fromLocal8Bit("���١�2"));
        videoPlayer->setPlaybackRate(2);
        rate *= 2;
        videoPlayer->play();
    }
    else if (rate == 2)
    {
        videoPlayer->pause();
        pushButton[7]->setText(QString::fromLocal8Bit("���١�4"));
        videoPlayer->setPlaybackRate(4);
        rate *= 2;
        videoPlayer->play();
    }
    else if (rate == 4)
    {
        videoPlayer->pause();
        pushButton[7]->setText(QString::fromLocal8Bit("���١�8"));
        videoPlayer->setPlaybackRate(8);
        rate = 8;
        videoPlayer->play();
    }
    else if (rate == 8)
    {
        videoPlayer->pause();
        pushButton[7]->setText(QString::fromLocal8Bit("���١�1"));
        videoPlayer->setPlaybackRate(1);
        rate =1;
        videoPlayer->play();
    }
}
/**
 * @brief MainWindow::SltBtnChangePlayMode();
 * �ı䲥��ģʽ
 * @param mode
 */
void MainWindow::SltBtnChangePlayMode()
{
    //�������
    if (mediaPlaylist->playbackMode() == QMediaPlaylist::Loop)   
    {
        mediaPlaylist->setPlaybackMode(QMediaPlaylist::Random);  
        pushButton[8]->setStyleSheet("QPushButton{border-image:url(./icons/btn_playRandom1.png);} QPushButton:hover{border-image:url(./icons/btn_playRandom2.png);}");
    }
    //ѭ������
    else if (mediaPlaylist->playbackMode() == QMediaPlaylist::Random)
    {
        mediaPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        pushButton[8]->setStyleSheet("QPushButton{border-image:url(./icons/btn_playLoopCurrent1.png);} QPushButton:hover{border-image:url(./icons/btn_playLoopCurrent2.png);}");
    }
    //ֻ����һ��
    else if (mediaPlaylist->playbackMode() == QMediaPlaylist::CurrentItemInLoop)
    {
        mediaPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        pushButton[8]->setStyleSheet("QPushButton{border-image:url(./icons/btn_playOnceCurrent1.png);} QPushButton:hover{border-image:url(./icons/btn_playOnceCurrent2.png);}");
    }
    //˳��ѭ������
    else if (mediaPlaylist->playbackMode() == QMediaPlaylist::CurrentItemOnce)
    {
        mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
        pushButton[8]->setStyleSheet("QPushButton{border-image:url(./icons/btn_playLoopList1.png);} QPushButton:hover{border-image:url(./icons/btn_playLoopList2.png);}");
    }
    
}
/**
 * @brief MainWindow::keyPressEvent
 * ��ݼ��¼�
 * @param event
 */
void MainWindow::keyPressEvent(QKeyEvent* event)
{
    //��ͣ
    if (event->key() == Qt::Key_Space)
    {
        SltBtnPlayClicked();
        cout << "���¿ո��" << endl;
    }
    //ȫ��
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
        cout << "���صײ���" << endl;
    }
    //ctrl���������һ��
    if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_Left)
    {
        SltBtnPreClicked();
        cout << "����Ctrl+���" << endl;
    }
    //ctrl�Ҽ�������һ��
    if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_Right)
    {
        SltBtnNextClicked();
        cout << "����Ctrl+�Ҽ�" << endl;
    }
    //��������
    if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_Up)
    {
        SltBtnVolumeUpClicked();
        cout << "����Ctrl+�ϼ�" << endl;
    }
    //��������
    if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_Down)
    {
        SltBtnVolumeDownClicked();
        cout << "����Ctrl+�¼�" << endl;
    } 
    //ȫ��
    if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_F)
    {
        pushButton[4]->click();   //����ȫ��
        //SltBtnFullScreenClicked();
        cout << "����CTRL+F" << endl;
    }
    //������Դ����
    if (event->modifiers() == Qt::CTRL && event->key() == Qt::Key_I)
    {
        SltBtnAddFile();
        cout << "����CTRL+I" << endl;
    }
}
/**
 * @brief MainWindow::resizeEvent
 * �ػ洰��
 * @param event
 */
void MainWindow::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    vWidget[1]->setGeometry(0, this->height() - 80, this->width(), 80);
}


void MainWindow::finfofun(void)      //��ȡ�ļ���Ϣ
{
    MP3Header* mp3header = new MP3Header();
    QString Murl = videoListWidget->musicItem->text();
    string musicPath = getPathByName(Murl.toStdString());

    MP3INFO mp3info = mp3header->GetAllInfo(QString::fromStdString(musicPath).toStdWString().c_str(), 10);
    //MP3INFO mp3info = mp3header->GetAllInfo(L"./myVideo/Alone-Alan Walker.mp3", 9);
    QMessageBox::about(this, u8"������Ϣ", QString(u8"������ : %1  \n"
        u8"���� : %2 \n"
        u8"ʱ�� : %3 \n"
        u8"�ļ���С : %4\n").arg(mp3info.Name).arg(mp3info.Singer).arg("00:00").arg(mp3info.lenth));
}