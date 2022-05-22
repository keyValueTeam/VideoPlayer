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
    QMediaPlayer* music = NULL;//����

    QMediaPlaylist* playlist = NULL;//�����б�1
    QMediaPlaylist* playlist_2 = NULL;//�����б�2
    QMediaPlaylist* playlist_3 = NULL;//�����б�3
    QMediaPlaylist* playlist_4 = NULL;//��ʷ

    QPixmap pix;//ͼƬ
    QPalette pal;//��ɫ��

    QAction* action1;//�������������
    QAction* action2;//�������ϲ��
    QAction* action3;//��������Ŷ���
    QAction* add;//���Ӳ˵�
    QAction* location;//��λ

    QAction* action4;//͸��������
    QAction* action4_1;//͸���ȼ�
    QAction* action4_2;//͸���ȼ�
    QAction* action5;//Ƥ������
    QAction* action5_1;//Ĭ��Ƥ��1
    QAction* action5_2;//Ĭ��Ƥ��2
    QAction* action5_4;
    QAction* action5_3;//�Զ���Ƥ��
    QAction* action6;//����
    QAction* action7;//ʹ�ý���
    QAction* action8;//�˳�
    QAction* ClearALL;//����˵�
    QAction* ClearALL_1;//�����������
    QAction* ClearALL_2;//�����ϲ��
    QAction* ClearALL_3;//������Ŷ���
    QAction* ClearALL_4;//�������
    QAction* ClearHistory;
    //�Ҽ��˵�
    QAction* action9;//��һ��
    QAction* action10;//��һ��
    QAction* action11;//����
    QAction* action11_1;//��ͣ
    QAction* action12;//����ģʽ
    QAction* action12_1;//˳�򲥷�
    QAction* action12_2;//�������
    QAction* action12_3;//����ѭ��
    QAction* action13;//�˳�
    QAction* detila;//��������
    QMenu* menu;//�Ҽ��˵�
    QPoint offset;//��
    double transparency;//͸����

   // QSqlTableModel* model;//�����б�1ģ��
   // QSqlTableModel* model_2;//�����б�2ģ��
   // QSqlTableModel* model_3;//�����б�3ģ��

    QList<int> load;//ӳ��
    QStringList labelText1;
    QStringList labelText2;
    Dialog* Music = NULL;//������

   
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

