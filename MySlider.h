#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QObject>
#include <QSlider>

class MySlider : public QSlider
{
    Q_OBJECT
public:
    MySlider();
    ~MySlider();
protected:
    void mousePressEvent(QMouseEvent* ev);   //鼠标点击滑动条位置事件
signals:
    void costomSliderClicked();   //自定义鼠标点击事件，用于捕获处理
};

#endif // MYSLIDER_H