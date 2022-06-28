#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QScrollArea>
#include <QScrollBar>
#include <QLabel>
#include <QSlider>
#include <QObject>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class ImageViewer; }
QT_END_NAMESPACE

class ImageViewer : public QMainWindow
{
    Q_OBJECT
public:
    ImageViewer(QWidget *parent = nullptr);
//    ImageViewer(const ImageViewer & view);
//    void operator=(const ImageViewer & view);
    bool loadFile(const QString &);

//public slots:
    void open();
    void saveAs();
    void save();
    void Bright1();
    void Darker1();
    void lightContrast();
    void greyScale();
    void warm();
    void cold();
    void saturation();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void about();
    std::vector<QImage> image_Vector;// 存储图像的Vvctor容器
    std::vector<QImage>::iterator it;// vector迭代器
    void undo();
    void redo();

public:
    void createActions();
    void createMenus();
    void updateActions();
    bool saveFile(const QString &fileName);
    void setImage(const QImage &newImage);
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    QImage image;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor = 1;
    QString path;

//#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
//    QPrinter printer;
//#endif

    QAction *saveAsAct;
    QAction *saveAct;
//    QAction *printAct;
    QAction *contrastAct;
    QAction *brightAct;
    QAction *darkAct;
    QAction *greyAct;
    QAction *warmAct;
    QAction *coldAct;
    QAction *saturateAct;
    QAction *undoAct;
//    QAction *copyAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;

//    void mousePressEvent(QMouseEvent *ev);

//    class MySlider : public QSlider
//    {
//    public:
//        MySlider(QWidget *parent = nullptr): QSlider(parent) {}
//        ~MySlider() {}

//        void mousePressEvent(QMouseEvent *ev)
//        {
//            int X = ev->pos().x();
//            double per = X*1.0/this->width();
//            int value = per*(this->maximum() - this->minimum()) + this->minimum();
//            this->setValue(value);

////            ImageViewer *parent = (ImageViewer *) ((char*)this - offsetof(ImageViewer, ));
//            Bright1(value);

//            QSlider::mousePressEvent(ev);
//        }
//    };
};
#endif // IMAGEVIEWER_H


