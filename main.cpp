#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTextCodec>
#include<vector>
#include<string>
#include "jsonData.h"
#include <Windows.h>
#include "function.h"
using namespace std;
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    /* ָ���ļ� */
    QFile file("./style.qss");    //�����û�����úã�ע�͵��Ϳ�������ʹ��

    /* �ж��ļ��Ƿ���� */
    if (file.exists()) 
    {
        /* ��ֻ���ķ�ʽ�� */
        file.open(QFile::ReadOnly);
        /* ���ַ����ķ�ʽ��������Ľ�� */
        QString styleSheet = QLatin1String(file.readAll());
        /* ����ȫ����ʽ */
        qApp->setStyleSheet(styleSheet);
        /* �ر��ļ� */
        file.close();
    }

    MainWindow w;
    w.show();
    return a.exec();
}