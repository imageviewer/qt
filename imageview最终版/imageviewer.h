#include <QMainWindow>
#include <QScrollArea>
#include <QScrollBar>
#include <QLabel>
#include <QPushButton>
#include <QToolBar>
#include <QSlider>
#include <QObject>

QT_BEGIN_NAMESPACE
namespace Ui { class ImageViewer; }
QT_END_NAMESPACE

class MySlider;

class ImageViewer : public QMainWindow
{
    Q_OBJECT

    friend class MySlider;
public:
    MySlider *brightslider;
    MySlider *contrastslider;
    MySlider *warmslider;
    MySlider *saturationslider;
    ImageViewer(QWidget *parent = nullptr);
    bool loadFile(const QString &);
    bool loadFile1(const QString &fileName);

    void open();
    void saveAs();
    void save();
    void Bright(int brightness);
    void Bright1();
    void Darker1();
    void lightContrast(int light, int delta);
    void lightContrast1();
    void lightContrast2();
    void reverse_contrast(int contrast);
    void greyScale();
    void warm(int dalta);
    void warm1();
    void cold1();
    void saturation(int dalta);
    void saturation1();
    void saturation2();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void about();
    std::vector<QImage> image_Vector;
    std::vector<QImage>::iterator it;
    std::vector<std::pair<int,int> > q;
    std::vector<std::pair<int,int> >::iterator q_it;
    void VectorChange(int kindno, int delta);
    void sliderrest();
    void undo();
    void redo();

    void createActions();
    void createbuttom();
    void createMenus();
    void updateActions();
    void fitToWindow1();
    bool saveFile(const QString &fileName);
    void setImage(const QImage &newImage);
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    QImage image;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor = 1;
    QString path;

    QAction *saveAsAct;
    QAction *saveAct;
    QAction *contrastaddAct;
    QAction *contrastdelAct;
    QAction *brightAct;
    QAction *darkAct;
    QAction *greyAct;
    QAction *warmAct;
    QAction *coldAct;
    QAction *saturateaddAct;
    QAction *saturatedelAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
private:
    Ui::ImageViewer *ui;
};

class MySlider : public QSlider
{
public:
    int kindno;
    int changed_value;
    ImageViewer *image;
    MySlider(int kindno_, QWidget *parent = nullptr);
    ~MySlider();
    void mousePressEvent(QMouseEvent *ev);
};
