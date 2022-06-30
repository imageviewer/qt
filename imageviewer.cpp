#include "imageviewer.h"
#include "ui_imageviewer.h"
#include <QApplication>
#include <QClipboard>
#include <QColorSpace>
#include <QDir>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QStandardPaths>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QSplitter>
#include <QButtonGroup>
#include <QIcon>
#include <QSlider>
#include <QMouseEvent>

ImageViewer::ImageViewer(QWidget *parent)
   : QMainWindow(parent), imageLabel(new QLabel)
   , scrollArea(new QScrollArea),ui(new Ui::ImageViewer)
{

    image_Vector.resize(2);
    // 设置背景为灰色
    imageLabel->setBackgroundRole(QPalette::Dark);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
    imageLabel->setAlignment(Qt::AlignCenter);
    // 添加scrollarea使得label可以自由滑动
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setVisible(false);
    // 设置中心布局
    QWidget* tmp = new QWidget;
    setCentralWidget(tmp);
    // 添加button
    QVBoxLayout *mainLayout = new QVBoxLayout;
    const QSize btnSize = QSize(48,48);

    QPushButton* but1 = new QPushButton();
    but1->setFixedSize(btnSize);
    but1->setStyleSheet("background-image: url(':/new/prefix1/Open.png'); background-repeat: no-repeat; border: 0px");
    connect(but1, &QPushButton::released, this, &ImageViewer::open);

    QPushButton* but2 = new QPushButton();
    but2->setFixedSize(btnSize);
    but2->setStyleSheet("background-image: url(':/new/prefix1/Save.png'); background-repeat: no-repeat; border: 0px");
    connect(but2, &QPushButton::released, this, &ImageViewer::save);

    QPushButton* but3 = new QPushButton();
    but3->setFixedSize(btnSize);
    but3->setStyleSheet("background-image: url(':/new/prefix1/SaveAs.png'); background-repeat: no-repeat; border: 0px");
    connect(but3, &QPushButton::released, this, &ImageViewer::saveAs);

    QPushButton* but8 = new QPushButton();
    but8->setFixedSize(btnSize);
    but8->setStyleSheet("background-image: url(':/new/prefix2/undo.png'); background-repeat: no-repeat; border: 0px");
    connect(but8, &QPushButton::released, this, &ImageViewer::undo);

    QPushButton* but9 = new QPushButton();
    but9->setFixedSize(btnSize);
    but9->setStyleSheet("background-image: url(':/new/prefix2/redo.png'); background-repeat: no-repeat; border: 0px");
    connect(but9, &QPushButton::released, this, &ImageViewer::redo);

    QPushButton* but4 = new QPushButton();
    but4->setFixedSize(btnSize);
    but4->setStyleSheet("background-image: url(':/new/prefix1/ZoomIn.png'); background-repeat: no-repeat; border: 0px");
    connect(but4, &QPushButton::released, this, &ImageViewer::zoomIn);

    QPushButton* but5 = new QPushButton();
    but5->setFixedSize(btnSize);
    but5->setStyleSheet("background-image: url(':/new/prefix1/ZoomOut.png'); background-repeat: no-repeat; border: 0px");
    connect(but5, &QPushButton::released, this, &ImageViewer::zoomOut);

    QPushButton* but6 = new QPushButton();
    but6->setFixedSize(btnSize);
    but6->setStyleSheet("background-image: url(':/new/prefix1/normal.png'); background-repeat: no-repeat; border: 0px");
    connect(but6, &QPushButton::released, this, &ImageViewer::normalSize);

    QPushButton* but7 = new QPushButton();
    but7->setFixedSize(btnSize);
    but7->setStyleSheet("background-image: url(':/new/prefix1/Fit.png'); background-repeat: no-repeat; border: 0px");
    connect(but7, &QPushButton::released, this, &ImageViewer::fitToWindow1);

    QPushButton* but10 = new QPushButton();
    but10->setFixedSize(btnSize);
    but10->setStyleSheet("background-image: url(':/fig/grey.png'); background-repeat: no-repeat; border: 0px");
    connect(but10, &QPushButton::released, this, &ImageViewer::greyScale);

    QHBoxLayout *tmpLayout = new QHBoxLayout;
    tmpLayout->addWidget(but1);
    tmpLayout->addWidget(but2);
    tmpLayout->addWidget(but3);
    tmpLayout->addWidget(but8);
    tmpLayout->addWidget(but9);
    tmpLayout->addWidget(but4);
    tmpLayout->addWidget(but5);
    tmpLayout->addWidget(but6);
    tmpLayout->addWidget(but7);
    tmpLayout->addWidget(but10);
    tmpLayout->addStretch(1);
    mainLayout->addLayout(tmpLayout);
    scrollArea->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(scrollArea);
    mainLayout->setSpacing(0);
    // 添加slider
    int nmin = -100;
    int nmax = 100;
    int nsinglestep = 10;
    brightslider = new MySlider(1, this);
    brightslider->image = this;
    brightslider->setOrientation(Qt::Vertical);
    brightslider->setMinimum(nmin);
    brightslider->setMaximum(nmax);
    brightslider->setSingleStep(nsinglestep);


    contrastslider = new MySlider(2, this);
    contrastslider->image = this;
    contrastslider->setOrientation(Qt::Vertical);
    contrastslider->setMinimum(nmin);
    contrastslider->setMaximum(nmax);
    contrastslider->setSingleStep(nsinglestep);

    warmslider = new MySlider(3, this);
    warmslider->image = this;
    warmslider->setOrientation(Qt::Vertical);
    warmslider->setMinimum(nmin);
    warmslider->setMaximum(nmax);
    warmslider->setSingleStep(nsinglestep);

    saturationslider = new MySlider(4, this);
    saturationslider->image = this;
    saturationslider->setOrientation(Qt::Vertical);
    saturationslider->setMinimum(0);
    saturationslider->setMaximum(nmax);
    saturationslider->setSingleStep(nsinglestep);

    QHBoxLayout *Layout = new QHBoxLayout;

    QVBoxLayout *brightLayout = new QVBoxLayout;
    QLabel *bright = new QLabel;
    bright->setText(" 亮度  ");
    brightLayout->addWidget(bright);
    brightLayout->addWidget(brightslider);
    brightLayout->setAlignment(Qt::AlignCenter);

    QVBoxLayout *contrastLayout = new QVBoxLayout;
    QLabel *contrast = new QLabel;
    contrast->setText("对比度 ");
    contrastLayout->addWidget(contrast);
    contrastLayout->addWidget(contrastslider);

    QVBoxLayout *warmLayout = new QVBoxLayout;
    QLabel *warm = new QLabel;
    warm->setText("冷暖度 ");
    warmLayout->addWidget(warm);
    warmLayout->addWidget(warmslider);

    QVBoxLayout *saturationLayout = new QVBoxLayout;
    QLabel *saturation = new QLabel;
    saturation->setText("饱和度 ");
    saturationLayout->addWidget(saturation);
    saturationLayout->addWidget(saturationslider);


    Layout->addLayout(mainLayout);
    Layout->addLayout(brightLayout);
    Layout->addLayout(contrastLayout);
    Layout->addLayout(warmLayout);
    Layout->addLayout(saturationLayout);

    centralWidget()->setLayout(Layout);

    createActions();

    resize(1200,605);

    loadFile1(":/new/prefix2/bg.jpg");
    fitToWindow1();
}

/* 初始化函数，用于初始化界面 */
bool ImageViewer::loadFile1(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();

    setImage(newImage);

    setWindowFilePath("欢迎使用imageview");
    return true;
}

/* 记录图片的变化 */
void ImageViewer::VectorChange(int kindno, int delta)
{
    if(it == image_Vector.end() - 1) {
        image_Vector.push_back(image);
        it=image_Vector.end()-1;
    }
    else {
        *(++it) = image;
    }
    if(q_it == q.end() - 1) {
        q.push_back(std::make_pair(kindno, delta));
        q_it=q.end()-1;
    }
    else {
        *(++q_it) = std::make_pair(kindno, delta);
    }
}

void ImageViewer::sliderrest()
{
    brightslider->setValue(0);
    brightslider->changed_value = 0;
    contrastslider->setValue(0);
    contrastslider->changed_value = 0;
    warmslider->setValue(0);
    warmslider->changed_value = 0;
    saturationslider->setValue(0);
    saturationslider->changed_value = 0;
}

bool ImageViewer::loadFile(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();

    path = fileName;
    setImage(newImage);

    setWindowFilePath(fileName);
    // 在窗口显示图片的基本信息
    const QString description = image.colorSpace().isValid()
        ? image.colorSpace().description() : tr("unknown");
    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4 (%5)")
        .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height())
        .arg(image.depth()).arg(description);
    statusBar()->showMessage(message);
    brightslider->changed_value = 0;
    brightslider->setValue(0);
    return true;
}

void ImageViewer::setImage(const QImage &newImage)
{
    image = newImage;
    if (image.colorSpace().isValid())
        image.convertToColorSpace(QColorSpace::SRgb);
    imageLabel->setPixmap(QPixmap::fromImage(image));

    image_Vector.push_back(image);
    it = image_Vector.end() - 1;

    q.push_back(std::make_pair(0, 0));
    q_it = q.end() - 1;

    sliderrest();

    scaleFactor = 1.0;

    scrollArea->setVisible(true);

    fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}
/* 保存图片 */
bool ImageViewer::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);
    const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);
    return true;
}

void ImageViewer::save(){
    saveFile(path);
}
// 初始化
static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    for (const QByteArray &mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    dialog.setAcceptMode(acceptMode);
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}
// 打开图片
void ImageViewer::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().constFirst())) {}
}
// 另存图片
void ImageViewer::saveAs()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().constFirst())) {}
}


// 拉杆调节控制函数
void ImageViewer::Bright(int brightness)
{

    uchar *line =image.scanLine(0);
    uchar *pixel;

        for (int y = 0; y < image.height(); ++y)
        {
            pixel = line;
            for (int x = 0; x < image.width(); ++x)
            {
                *pixel = qBound(0, *pixel + brightness, 255);
                *(pixel + 1) = qBound(0, *(pixel + 1) + brightness, 255);
                *(pixel + 2) = qBound(0, *(pixel + 2) + brightness, 255);
                pixel += 4;
            }

            line += image.bytesPerLine();
        }

        VectorChange(1, brightness);

        imageLabel->setPixmap(QPixmap::fromImage(image));

        if (!fitToWindowAct->isChecked())
            imageLabel->adjustSize();
}
// 快捷键调节控制函数，其中Bright1为增加单位亮度，Dark1为减少单位亮度，下同
void ImageViewer::Bright1() {
    this->Bright(10);
    brightslider->setValue(brightslider->value()+10);
}

void ImageViewer::Darker1() {
    this->Bright(-10);
    brightslider->setValue(brightslider->value()-10);
}

void ImageViewer::lightContrast(int contrast, int delta)
{
    double param = 1.0/(1-contrast*1.0/(100.0))-1.0;
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            float r = qRed(image.pixel(x, y));
            float g = qGreen(image.pixel(x,y));
            float b = qBlue(image.pixel(x,y));

            r = r + (r - 127) * param;
            g = g + (g-127)*param;
            b = b+(b-127)*param;

            r = qBound(0.0, r, 255.0);
            g = qBound(0.0, g, 255.0);
            b = qBound(0.0, b, 255.0);

            image.setPixel(x, y, qRgb(r, g, b));
        }

    }

    VectorChange(2, delta);

    imageLabel->setPixmap(QPixmap::fromImage(image));

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}

void ImageViewer::reverse_contrast(int contrast){
    double param = (100.0-contrast*1.0)/100.0;
    double bias = 127.0*contrast/100.0;
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            float r = qRed(image.pixel(x, y));
            float g = qGreen(image.pixel(x,y));
            float b = qBlue(image.pixel(x,y));

            r = param*r+bias;
            g = param*g+bias;
            b = param*b+bias;

            r = qBound(0.0, r, 255.0);
            g = qBound(0.0, g, 255.0);
            b = qBound(0.0, b, 255.0);

            image.setPixel(x, y, qRgb(r, g, b));
        }

    }

    imageLabel->setPixmap(QPixmap::fromImage(image));

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}

void ImageViewer::lightContrast1() {
    this->lightContrast(20, 20);
    contrastslider->setValue(contrastslider->value()+20);
}

void ImageViewer::lightContrast2() {
    this->reverse_contrast(20);
    contrastslider->setValue(contrastslider->value()-20);
}

void ImageViewer::greyScale(){

    QColor oldColor;

    for(int x = 0; x<image.width(); x++){
        for(int y = 0; y<image.height(); y++){
            oldColor = QColor(image.pixel(x,y));
            int average = (oldColor.red()+oldColor.green()+oldColor.blue())/3;
            image.setPixel(x,y,qRgb(average,average,average));
        }
    }

    VectorChange(10, 0);

    imageLabel->setPixmap(QPixmap::fromImage(image));

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}

void ImageViewer::warm(int delta){
    QColor oldColor;
    int r,g,b;

    for(int x=0; x<image.width(); x++){
        for(int y=0; y<image.height(); y++){
            oldColor = QColor(image.pixel(x,y));

            r = oldColor.red() + delta;
            g = oldColor.green() + delta;
            b = oldColor.blue();

            //we check if the new values are between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);

            image.setPixel(x,y, qRgb(r,g,b));
        }
    }

    VectorChange(3, delta);

    imageLabel->setPixmap(QPixmap::fromImage(image));

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}
void ImageViewer::warm1(){
    this->warm(10);
    warmslider->setValue(warmslider->value()+10);
}
void ImageViewer::cold1(){
    this->warm(-10);
    warmslider->setValue(warmslider->value()-10);
}

void ImageViewer::saturation(int delta){

    for(int x=0; x<image.width(); x++){
        for(int y=0; y<image.height(); y++){

            QColor color = image.pixelColor(x, y);

            int s = color.saturation()+delta;
            s = qBound(0, s, 255);

            // modify hue as you'd like and write back to the image
            color.setHsv(color.hue(), s, color.value(), color.alpha());
            image.setPixelColor(x, y, color);
        }
    }


    VectorChange(4,delta);

    imageLabel->setPixmap(QPixmap::fromImage(image));

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}
void ImageViewer::saturation1(){
    this->saturation(10);
    saturationslider->setValue(saturationslider->value()+10);
}

void ImageViewer::saturation2()
{
    this->saturation(-10);
    saturationslider->setValue(saturationslider->value()-10);
}

void ImageViewer::zoomIn()
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

void ImageViewer::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void ImageViewer::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();

    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        normalSize();
    updateActions();
}


void ImageViewer::fitToWindow1()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    fitToWindowAct->setChecked(1^fitToWindow);
    fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        normalSize();
    updateActions();
}

void ImageViewer::about()
{
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>欢迎使用由阿对对对组出品的图像编辑软件！祝您使用愉快！</p>"));
}

void ImageViewer::undo()
{
    if(it - 1 != image_Vector.begin())
    {
        it--;
        image = *it;
        imageLabel->setPixmap(QPixmap::fromImage(*it));
    }
    if(q_it - 1 != q.begin())
    {
        int now;
        switch(q_it->first) {
            case 1: now = brightslider->value() - q_it->second;brightslider->setValue(now); brightslider->changed_value = now; break;
            case 2: now = contrastslider->value() - q_it->second; contrastslider->setValue(now); contrastslider->changed_value = now; break;
            case 3: now = warmslider->value() - q_it->second; warmslider->setValue(now); warmslider->changed_value = now; break;
            case 4: now = saturationslider->value() - q_it->second; saturationslider->setValue(now); saturationslider->changed_value = now; break;
        }
        q_it--;
    }
}

void ImageViewer::redo()
{
    if(it != image_Vector.end() - 1)
    {
        it++;
        image = *it;
        imageLabel->setPixmap(QPixmap::fromImage(*it));
    }
    if(q_it != q.end() - 1)
    {
        q_it ++;
        int now;
        switch(q_it->first) {
            case 1: now = brightslider->value() + q_it->second;brightslider->setValue(now); brightslider->changed_value = now; break;
            case 2: now = q_it->second; contrastslider->setValue(now); contrastslider->changed_value = now; break;
            case 3: now = warmslider->value() + q_it->second; warmslider->setValue(now); warmslider->changed_value = now; break;
            case 4: now = saturationslider->value() + q_it->second; saturationslider->setValue(now); saturationslider->changed_value = now; break;
        }
    }
}

void ImageViewer::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &ImageViewer::open);
    openAct->setShortcut(QKeySequence::Open);

    saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &ImageViewer::saveAs);
    saveAsAct->setEnabled(false);

    saveAct = fileMenu->addAction(tr("&Save ..."), this, &ImageViewer::save);
    saveAct->setEnabled(false);

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("&Exit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    brightAct = editMenu->addAction(tr("&Bright"), this, &ImageViewer::Bright1);
    brightAct->setShortcut(tr("Ctrl+B"));
    brightAct->setEnabled(true);

    darkAct = editMenu->addAction(tr("&Dark"), this, &ImageViewer::Darker1);
    darkAct->setShortcut(tr("Ctrl+D"));
    darkAct->setEnabled(true);

    contrastaddAct = editMenu->addAction(tr("&AddContrast"), this, &ImageViewer::lightContrast1);
    contrastaddAct->setShortcut(tr("Ctrl+L"));
    contrastaddAct->setEnabled(true);

    contrastdelAct = editMenu->addAction(tr("&DelContrast"), this, &ImageViewer::lightContrast2);
    contrastdelAct->setShortcut(tr("Ctrl+K"));
    contrastdelAct->setEnabled(true);

    greyAct = editMenu->addAction(tr("&Grey"), this, &ImageViewer::greyScale);
    greyAct->setShortcut(tr("Ctrl+G"));
    greyAct->setEnabled(true);

    warmAct = editMenu->addAction(tr("&Warm"), this, &ImageViewer::warm1);
    warmAct->setShortcut(tr("Ctrl+W"));
    warmAct->setEnabled(true);

    coldAct = editMenu->addAction(tr("&Cold"), this, &ImageViewer::cold1);
    coldAct->setShortcut(tr("Ctrl+E"));
    coldAct->setEnabled(true);

    saturateaddAct = editMenu->addAction(tr("&AddSaturate"), this, &ImageViewer::saturation1);
    saturateaddAct->setShortcut(tr("Ctrl+T"));
    saturateaddAct->setEnabled(true);

    saturatedelAct = editMenu->addAction(tr("&DelSaturate"), this, &ImageViewer::saturation2);
    saturatedelAct->setShortcut(tr("Ctrl+Y"));
    saturatedelAct->setEnabled(true);

    undoAct = editMenu->addAction(tr("&Undo"), this, &ImageViewer::undo);
    undoAct->setShortcut(tr("Ctrl+U"));
    undoAct->setEnabled(true);

    redoAct = editMenu->addAction(tr("&Redo"), this, &ImageViewer::redo);
    redoAct->setShortcut(tr("Ctrl+R"));
    redoAct->setEnabled(true);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    zoomInAct = viewMenu->addAction(tr("Zoom &In (25%)"), this, &ImageViewer::zoomIn);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);

    zoomOutAct = viewMenu->addAction(tr("Zoom &Out (25%)"), this, &ImageViewer::zoomOut);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);

    normalSizeAct = viewMenu->addAction(tr("&Normal Size"), this, &ImageViewer::normalSize);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);

    viewMenu->addSeparator();

    fitToWindowAct = viewMenu->addAction(tr("&Fit to Window"), this, &ImageViewer::fitToWindow);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(tr("&About"), this, &ImageViewer::about);


}

void ImageViewer::updateActions()
{
    saveAsAct->setEnabled(!image.isNull());
    saveAct->setEnabled(!image.isNull());

    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor)
{
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);
    // 避免图像过大导致内存溢出
    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

MySlider::MySlider(int kindno_, QWidget *parent): QSlider(parent), kindno(kindno_), changed_value(0)
{

}
MySlider::~MySlider()
{

}

void MySlider::mousePressEvent(QMouseEvent *ev)
{
    int Y = ev->pos().y();
    double per = Y*1.0/this->height();
    int value = per*(this->minimum() - this->maximum()) + this->maximum();
    this->setValue(value);

    int to_change = value - changed_value;


    switch(kindno){
    case 1:
        image->Bright(to_change);
        break;
    case 2:
        image->reverse_contrast(changed_value);
        image->lightContrast(value, to_change);
        break;
    case 3:
        image->warm(to_change);
        break;
    case 4:
        image->saturation(to_change);
        break;
    }
    changed_value = value;
}
