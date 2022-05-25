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

    /* 指定文件 */
    QFile file("./style.qss");    //这个还没有配置好，注释掉就可以正常使用

    /* 判断文件是否存在 */
    if (file.exists()) 
    {
        /* 以只读的方式打开 */
        file.open(QFile::ReadOnly);
        /* 以字符串的方式保存读出的结果 */
        QString styleSheet = QLatin1String(file.readAll());
        /* 设置全局样式 */
        qApp->setStyleSheet(styleSheet);
        /* 关闭文件 */
        file.close();
    }
    //SetConsoleOutputCP(65001); //改成UTF-8编码

    MainWindow w;
    w.show();
    QString str = "./myVideo/AGA - Wonderful U (Demo).mp3";
    cout << isMP3(str) << endl;
    return a.exec();
}