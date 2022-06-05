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
    //void finfofun();      //获取文件信息
    MyListWidget();
    ~MyListWidget();
   
protected:
    void contextMenuEvent(QContextMenuEvent* ev);   //鼠标点击滑动条位置事件
   // void MyListWidget::finfofun();      //获取文件信息
};
#endif // !MYLISTWIDGET_H