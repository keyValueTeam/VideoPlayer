#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QSpacerItem>
#include <QVideoWidget>
#include <QMouseEvent>
#include "MySlider.h"
#include<QContextMenuEvent>
#include"MylistWidget.h"
/* 媒体信息结构体 */
struct MediaObjectInfo {
    QString fileName; // 用于保存视频文件名
    QString filePath; // 用于保存视频文件路径
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    /* 属性 */
private:
    QMediaPlayer* videoPlayer; // 媒体播放器，用于播放视频
    QMediaPlaylist* mediaPlaylist; // 媒体列表
    QVideoWidget* videoWidget; // 视频显示窗口
    QWidget* imgWidget;   // 图片窗口
    MyListWidget* videoListWidget; // 视频列表
    MySlider* playProgressScrollBar; // 播放进度条
    QSlider* volumeScrollBar; // 音量条
    QPushButton* pushButton[10]; // 视频播放器按钮
    QHBoxLayout* hBoxLayout[3]; // 水平布局
    QWidget* hWidget[3]; // 水平容器
    QLabel* label[2]; // 标签文本
    QWidget* vWidget[2]; // 垂直容器
    QVBoxLayout* vBoxLayout[2]; // 垂直布局
    QPushButton* a;
    int rate = 1; //倍数
protected:
    virtual void keyPressEvent(QKeyEvent* event);
/* 函数 */
public:
    void initWidget(); // 初始化窗口
    void mediaPlayerInit(); // 媒体播放初始化
    void scanVideoFiles(); // 扫描本地视频文件

/* 槽函数 */
public slots:
    void SltBtnPlayClicked(); // 播放点击
    void SltBtnNextClicked(); // 下一首点击
    void SltBtnPreClicked();  // 上一首点击
    void SltBtnVolumeDownClicked(); // 声音加点击
    void SltBtnVolumeUpClicked(); // 声音减点击
    void SltBtnFullScreenClicked(); // 全屏点击
    void SltVideoListWidgetCliked(QListWidgetItem*); // 视频列表点击
    void SltMediaPlaylistCurrentIndexChanged(int); // 媒体当前项改变
    void SltMediaPlayerStateChanged(QMediaPlayer::State); // 媒体播放状态改变
    void SltMediaPlayerDurationChanged(qint64); // 媒体时长发生变化
    void SltMediaPlayerPositionChanged(qint64); // 媒体当前播放时间发生变化
    void SltPlayProgressScrollBarReleased(); // 播放进度条
    void SltPlayProgressScrollBarClicked();   //点击播放进度条
    void SltVolumeScrollBarReleased(); // 音量进度条
    void SltBtnAddFile();    //添加文件到播放器
    void SltBtnChangePlayMode();    //改变播放模式
    void SltBtnClearClicked();   //清空列表
    void resizeEvent(QResizeEvent* event);   
    void SltBtnRateClicked();   //改变速率

    void finfofun(void);      //获取文件信息
};
#endif // MAINWINDOW_H