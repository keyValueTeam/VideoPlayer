#pragma once
#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include<QListWidget>
#include<QAction>
class MyListWidget:public QListWidget
{
    Q_OBJECT
public:
    QAction* finfo;
    QListWidgetItem* musicItem;
    //void finfofun();      //��ȡ�ļ���Ϣ
    MyListWidget();
    ~MyListWidget();
   
protected:
    void contextMenuEvent(QContextMenuEvent* ev);   //�����������λ���¼�
   // void MyListWidget::finfofun();      //��ȡ�ļ���Ϣ
};
#endif // !MYLISTWIDGET_H