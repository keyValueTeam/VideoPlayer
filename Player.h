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
#include <QFileDialog>
#include <QDebug>
#include "PlayTask.h"
#include <QTime>
#include "sqltest.h"
#include "dialog.h"

class UI_Player
{
public:
    QWidget* centralWidget;
    QLabel* label_Playtime;
    QLabel* openGLLabel;
    QPushButton* btnPlay;
    QPushButton* btnOpenDir;
    QPushButton* btnSelect;
    //QSlider *sliderVolume;
    //QPushButton *btnFullScreen;//set size
    //QPushButton *btnCutImage;
};
namespace Ui {
    class Widget;
}

class Player:public QMainWindow
{
public:
    unsigned char* buf;
    int ret;
    unsigned int i, streamIndex = 0;
    AVFormatContext* pFormatCtx;
    UI_Player uis;
    QThread* pThAudio;
    DecodeThread* thDecode;
    RingBuffer<AudioFrame, 50>* audioBuf;
    int mVideoPlaySta;
    QSqlDatabase db;
    Player(QWidget* parent);
    ~Player();
    Ui::Widget* ui;
    QMediaPlayer* music = NULL;//音乐

    QMediaPlaylist* playlist = NULL;//播放列表1
    QMediaPlaylist* playlist_2 = NULL;//播放列表2
    QMediaPlaylist* playlist_3 = NULL;//播放列表3
    QMediaPlaylist* playlist_4 = NULL;//历史

    QPixmap pix;//图片
    QPalette pal;//调色板

    QAction* action1;//添加至本地音乐
    QAction* action2;//添加至我喜欢
    QAction* action3;//添加至播放队列
    QAction* add;//增加菜单
    QAction* location;//定位

    QAction* action4;//透明度设置
    QAction* action4_1;//透明度加
    QAction* action4_2;//透明度减
    QAction* action5;//皮肤设置
    QAction* action5_1;//默认皮肤1
    QAction* action5_2;//默认皮肤2
    QAction* action5_4;
    QAction* action5_3;//自定义皮肤
    QAction* action6;//关于
    QAction* action7;//使用介绍
    QAction* action8;//退出
    QAction* ClearALL;//清除菜单
    QAction* ClearALL_1;//清除本地音乐
    QAction* ClearALL_2;//清除我喜欢
    QAction* ClearALL_3;//清除播放队列
    QAction* ClearALL_4;//清除所有
    QAction* ClearHistory;
    //右键菜单
    QAction* action9;//下一首
    QAction* action10;//上一首
    QAction* action11;//播放
    QAction* action11_1;//暂停
    QAction* action12;//播放模式
    QAction* action12_1;//顺序播放
    QAction* action12_2;//随机播放
    QAction* action12_3;//单曲循环
    QAction* action13;//退出
    QAction* detila;//歌曲详情
    QMenu* menu;//右键菜单
    QPoint offset;//点
    double transparency;//透明度

   // QSqlTableModel* model;//播放列表1模型
   // QSqlTableModel* model_2;//播放列表2模型
   // QSqlTableModel* model_3;//播放列表3模型

    QList<int> load;//映射
    QStringList labelText1;
    QStringList labelText2;
    Dialog* Music = NULL;//副窗口

   
    QVector<QString> currentFileName;
    QVector<int> list1;
    QList<int> map;
    QVector<QString> FilePath;
    void InitElse();
    void InitIcon();
    inline QString MenuStyle();
    inline QString PlayStyle();
    inline QString PaseStyle();
    inline QString IlikeStyle();
    inline QString Ilike1Style();
    inline QString ShowListStyle();
    inline QString HideListStyle();
    inline QString RandomStyle();
    inline QString LoopStyle();
    inline QString LoopOneStyle();
    inline QString VoiceStyle();
    inline QString NoVoiceStyle();
    inline QString MusicListStyle();
public slots:
    void PlayAudio();
    void LoadAudios();
    void SelectAudios();
    //void RemoveAudios();
};

