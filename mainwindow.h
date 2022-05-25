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
#include "MySlider.h"

/* ý����Ϣ�ṹ�� */
struct MediaObjectInfo {
    QString fileName; // ���ڱ�����Ƶ�ļ���
    QString filePath; // ���ڱ�����Ƶ�ļ�·��
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    /* ���� */
private:
    QMediaPlayer* videoPlayer; // ý�岥���������ڲ�����Ƶ
    QMediaPlaylist* mediaPlaylist; // ý���б�
    QVideoWidget* videoWidget; // ��Ƶ��ʾ����
    QLabel* backGround;    // ͼƬ����
    QListWidget* videoListWidget; // ��Ƶ�б�
    MySlider* playProgressScrollBar; // ���Ž�����
    QSlider* volumeScrollBar; // ������
    QPushButton* pushButton[8]; // ��Ƶ��������ť
    QHBoxLayout* hBoxLayout[3]; // ˮƽ����
    QWidget* hWidget[3]; // ˮƽ����
    QLabel* label[2]; // ��ǩ�ı�
    QWidget* vWidget[2]; // ��ֱ����
    QVBoxLayout* vBoxLayout[2]; // ��ֱ����
    int rate = 1; //����
/* ���� */
public:
    void initWidget(); // ��ʼ������
    void mediaPlayerInit(); // ý�岥�ų�ʼ��
    void scanVideoFiles(); // ɨ�豾����Ƶ�ļ�

/* �ۺ��� */
public slots:
    void SltBtnPlayClicked(); // ���ŵ��
    void SltBtnNextClicked(); // ��һ�׵��
    void SltBtnPreClicked();  // ��һ�׵��
    void SltBtnVolumeDownClicked(); // �����ӵ��
    void SltBtnVolumeUpClicked(); // ���������
    void SltBtnFullScreenClicked(); // ȫ�����
    void SltVideoListWidgetCliked(QListWidgetItem*); // ��Ƶ�б���
    void SltMediaPlaylistCurrentIndexChanged(int); // ý�嵱ǰ��ı�
    void SltMediaPlayerStateChanged(QMediaPlayer::State); // ý�岥��״̬�ı�
    void SltMediaPlayerDurationChanged(qint64); // ý��ʱ�������仯
    void SltMediaPlayerPositionChanged(qint64); // ý�嵱ǰ����ʱ�䷢���仯
    void SltPlayProgressScrollBarReleased(); // ���Ž�����
    void SltPlayProgressScrollBarClicked();   //������Ž�����
    void SltVolumeScrollBarReleased(); // ����������
    void SltBtnAddFile();    //����ļ���������
    void resizeEvent(QResizeEvent* event);
    void SltBtnRateClicked();
};
#endif // MAINWINDOW_H