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
    //�������ƶ��¼����¼�Ҳ�õ���mousePressEvent,����仰��Ϊ�˲��������Ӱ�죬�ǵ�Slider��������Ӧ��������¼�
    QSlider::mousePressEvent(ev);
    //��ȡ��ǰ���λ����Ի�����λ��
    double pos = ev->pos().x() / (double) width();

    //�趨������λ��
    this->setValue(pos * (maximum() - minimum()) + minimum());
    //�����Զ���������¼�
    emit costomSliderClicked();
}