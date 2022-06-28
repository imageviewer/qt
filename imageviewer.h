#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QScrollArea>
#include <QScrollBar>
#include <QLabel>
#include <QSlider>
#include <QObject>
#include <QSpinBox>

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
    void Bright1(int brightness);
    void Darker1(int brightness);
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
    QAction *redoAct;
//    QAction *copyAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;

};
#endif // IMAGEVIEWER_H


