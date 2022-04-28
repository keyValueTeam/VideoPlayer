#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "FFmpeg.h"
#include <QTime>

void delay(int);
const int Video_PlayFinish = 1;
const int Video_Playing = 0;

class Player:public QMainWindow
{
public:
    QAction *action_OpenLocalFiles;
    QAction *action_OpenNetStream;
    QAction *action_OpenCamera;
    QAction *action_About;
    QAction *action_GetSourceCode;
    QWidget *centralWidget;
    QLabel *label_Playtime;
    QLabel* openGLLabel;
    QSlider *sliderVolume;
    QPushButton *btnPlayVideo;
    QPushButton *btnCutImage;
    QPushButton *btnSetVolume;
    QPushButton *btnFullScreen;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menushezhi;
    QMenu *menu_Help;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QTimer* timer_play;
    QByteArray pcm1;
    QAudioOutput* output;
    QIODevice* device;
    QAudioFormat format;
    QLabel* labelVideo;
    int mVideoPlaySta;
    int mPlaySpdVal;
    Player(QWidget* parent);
    ~Player();
public slots:
    void playVideo(bool);
};

