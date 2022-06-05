#pragma once
#ifndef MUSICINFO_H
#define MUSICINFO_H


#include "stdlib.h"
#include "stdio.h"
#include <QDebug>
#include <QImage>
#include <QMap>
#include <QTextCodec>
#include <QString>
#include <QFile>
#include <QUrl>

using namespace std;
class MP3Header;

typedef struct ID3V2FrameH
{
    char frameID[4];               //��ű�ǩ��ʽ���Ƿ�Ϊid3v2
    unsigned char size[4];         //��ű�ǩ���ݵĴ�С
    char flags[2];
}ID3V2FH;

typedef struct MP3INFO
{
    QString Url;            //������׸����ϸ��ַ
    QString Name;            //����  TIT2
    QString Album;           //ר��  TALB
    QString Singer;          //����  TPE1
    QString Picture_url;      //����ͼƬ���·��
    QString Picture_type;     //ͼƬ���� jpg,png
    int     number;          //�������
    int     beginNum;         //ͼƬ��ʼλ��
    int     lenth;            //ͼƬ���ݳ���
    bool     pic_flag;         //�Ƿ���ͼƬ


}MP3INFO;
typedef struct frameIDStruct
{
    int beginNum;
    int endNum;
    QString FrameId;

}frameIDStruct;

class MP3Header
{
public:
    MP3Header();
    FILE* fp;
    QString m_url;
    unsigned char Header[3];
    unsigned char FrameId[4];     //���֡��ʶ
    unsigned char Header_size[4];
    unsigned int mp3_TagSize;
    unsigned char frameSize[4];      //��Ÿ�֡���ݵĴ�С 
    unsigned int framecount;          //�����֡���ݵĴ�С
    void GetMp3IDV2(const wchar_t* url);
    MP3INFO GetAllInfo(const wchar_t* url, int songNumber);
    void GetPicture(MP3INFO* mp3info);
    void GetFrameId();
    QString GetInfo(QString fId);
    QMap<QString, frameIDStruct> m_IDmap;

};
#endif // !MUSICINFO_H