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
    void mousePressEvent(QMouseEvent* ev);   //�����������λ���¼�
signals:
    void costomSliderClicked();   //�Զ���������¼������ڲ�����
};

#endif // MYSLIDER_H