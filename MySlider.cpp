#include "myslider.h"
#include <QMouseEvent>
#include <QDebug>

MySlider::MySlider():QSlider()
{

}

MySlider::~MySlider()
{

}

void MySlider::mousePressEvent(QMouseEvent* ev)
{
    //滑动条移动事件等事件也用到了mousePressEvent,加这句话是为了不对其产生影响，是的Slider能正常相应其他鼠标事件
    QSlider::mousePressEvent(ev);
    //获取当前点击位置相对滑动条位置
    double pos = ev->pos().x() / (double) width();

    //设定滑动条位置
    this->setValue(pos * (maximum() - minimum()) + minimum());
    //发送自定义鼠标点击事件
    emit costomSliderClicked();
}