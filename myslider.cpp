#include "myslider.h"
#include "imageviewer.h"
#include <QMouseEvent>
#include <QSlider>

MySlider::MySlider(QWidget *parent): QSlider(parent)
{

}
MySlider::~MySlider()
{

}

void MySlider::mousePressEvent(QMouseEvent *ev)
{
    int X = ev->pos().x();
    double per = X*1.0/this->width();
    int value = per*(this->maximum() - this->minimum()) + this->minimum();
    this->setValue(value);

    QSlider::mousePressEvent(ev);
}
