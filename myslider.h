#ifndef MYSLIDER_H
#define MYSLIDER_H

#include "imageviewer.h"
#include <QSlider>
#include <QObject>

class MySlider : public QSlider
{
public:
    ImageViewer *image;
    MySlider(QWidget *parent = nullptr);
    ~MySlider();
    void mousePressEvent(QMouseEvent *ev);
};

#endif // MYSLIDER_H
