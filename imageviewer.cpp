// the imageviewer project
#include "imageviewer.h"

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

//! [0]
ImageViewer::ImageViewer(QWidget *parent)
   : QMainWindow(parent), imageLabel(new QLabel)
   , scrollArea(new QScrollArea)
{
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setVisible(false);
    setCentralWidget(scrollArea);

    createActions();

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

//! [0]
//! [2]

bool ImageViewer::loadFile(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
//    if (newImage.isNull()) {
//        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
//                                 tr("Cannot load %1: %2")
//                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
//        return false;
//    }
//! [2]
    path = fileName;
    setImage(newImage);

    setWindowFilePath(fileName);

    const QString description = image.colorSpace().isValid()
        ? image.colorSpace().description() : tr("unknown");
    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4 (%5)")
        .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height())
        .arg(image.depth()).arg(description);
    statusBar()->showMessage(message);
    return true;
}

void ImageViewer::setImage(const QImage &newImage)
{
    image = newImage;
    if (image.colorSpace().isValid())
        image.convertToColorSpace(QColorSpace::SRgb);
    imageLabel->setPixmap(QPixmap::fromImage(image));
//! [4]
    scaleFactor = 1.0;

    scrollArea->setVisible(true);
//    printAct->setEnabled(true);
    fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}

//! [4]

bool ImageViewer::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);

    if (!writer.write(image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), writer.errorString()));
        return false;
    }
    const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);
    return true;
}

void ImageViewer::save(){
    saveFile(path);
}
//! [1]

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

void ImageViewer::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().constFirst())) {}
}
//! [1]

void ImageViewer::saveAs()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().constFirst())) {}
}

//! [5]
//void ImageViewer::print()
////! [5] //! [6]
//{
//    Q_ASSERT(!imageLabel->pixmap(Qt::ReturnByValue).isNull());
//#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
////! [6] //! [7]
//    QPrintDialog dialog(&printer, this);
////! [7] //! [8]
//    if (dialog.exec()) {
//        QPainter painter(&printer);
//        QPixmap pixmap = imageLabel->pixmap(Qt::ReturnByValue);
//        QRect rect = painter.viewport();
//        QSize size = pixmap.size();
//        size.scale(rect.size(), Qt::KeepAspectRatio);
//        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
//        painter.setWindow(pixmap.rect());
//        painter.drawPixmap(0, 0, pixmap);
//    }
//#endif
//}
////! [8]

//void ImageViewer::copy()
//{
//#ifndef QT_NO_CLIPBOARD
//    QGuiApplication::clipboard()->setImage(image);
//#endif // !QT_NO_CLIPBOARD
//}

//#ifndef QT_NO_CLIPBOARD
//static QImage clipboardImage()
//{
//    if (const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData()) {
//        if (mimeData->hasImage()) {
//            const QImage image = qvariant_cast<QImage>(mimeData->imageData());
//            if (!image.isNull())
//                return image;
//        }
//    }
//    return QImage();
//}
//#endif // !QT_NO_CLIPBOARD

//void ImageViewer::paste()
//{
//#ifndef QT_NO_CLIPBOARD
//    const QImage newImage = clipboardImage();
//    if (newImage.isNull()) {
//        statusBar()->showMessage(tr("No image in clipboard"));
//    } else {
//        setImage(newImage);
//        setWindowFilePath(QString());
//        const QString message = tr("Obtained image from clipboard, %1x%2, Depth: %3")
//            .arg(newImage.width()).arg(newImage.height()).arg(newImage.depth());
//        statusBar()->showMessage(message);
//    }
//#endif // !QT_NO_CLIPBOARD
//}

void ImageViewer::Bright1()
{

    int brightness = 10;
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
//        if (image.colorSpace().isValid())
//            image.convertToColorSpace(QColorSpace::SRgb);
        imageLabel->setPixmap(QPixmap::fromImage(image));

        if (!fitToWindowAct->isChecked())
            imageLabel->adjustSize();
}

void ImageViewer::Darker1()
{

    int brightness = -10;
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
        imageLabel->setPixmap(QPixmap::fromImage(image));

        if (!fitToWindowAct->isChecked())
            imageLabel->adjustSize();
}

void ImageViewer::lightContrast()
{
    int light = 120;
    int Contrast = 160;
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            float r = light * 0.01 * qRed(image.pixel(x, y)) - 150 + Contrast;
            float g = light * 0.01 * qGreen(image.pixel(x, y)) - 150 + Contrast;
            float b = light * 0.01 * qBlue(image.pixel(x, y)) - 150 + Contrast;
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

void ImageViewer::greyScale(){

    QColor oldColor;

    for(int x = 0; x<image.width(); x++){
        for(int y = 0; y<image.height(); y++){
            oldColor = QColor(image.pixel(x,y));
            int average = (oldColor.red()+oldColor.green()+oldColor.blue())/3;
            image.setPixel(x,y,qRgb(average,average,average));
        }
    }

    imageLabel->setPixmap(QPixmap::fromImage(image));

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}

void ImageViewer::warm(){
    int delta = 10;
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

    imageLabel->setPixmap(QPixmap::fromImage(image));

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}

void ImageViewer::cold(){
    int delta = 10;
    QColor oldColor;
    int r,g,b;

    for(int x=0; x<image.width(); x++){
        for(int y=0; y<image.height(); y++){
            oldColor = QColor(image.pixel(x,y));

            r = oldColor.red();
            g = oldColor.green();
            b = oldColor.blue() + delta;

            //we check if the new values are between 0 and 255
            r = qBound(0, r, 255);
            g = qBound(0, g, 255);

            image.setPixel(x,y, qRgb(r,g,b));
        }
    }

    imageLabel->setPixmap(QPixmap::fromImage(image));

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}

void ImageViewer::saturation(){
    QColor oldColor;
    QColor newColor;
    int h,s,l;

    int delta = 10;
    for(int x=0; x<image.width(); x++){
        for(int y=0; y<image.height(); y++){
            oldColor = QColor(image.pixel(x,y));

            newColor = oldColor.toHsl();
            h = newColor.hue();
            s = newColor.saturation()+delta;
            l = newColor.lightness();

            //we check if the new value is between 0 and 255
            s = qBound(0, s, 255);

            newColor.setHsl(h, s, l);

            image.setPixel(x, y, qRgb(newColor.red(), newColor.green(), newColor.blue()));
        }
    }

    imageLabel->setPixmap(QPixmap::fromImage(image));

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}



//! [9]
void ImageViewer::zoomIn()
//! [9] //! [10]
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

//! [10] //! [11]
void ImageViewer::normalSize()
//! [11] //! [12]
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}
//! [12]

//! [13]
void ImageViewer::fitToWindow()
//! [13] //! [14]
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        normalSize();
    updateActions();
}
//! [14]


//! [15]
void ImageViewer::about()
//! [15] //! [16]
{
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>欢迎使用由阿对对对组出品的图像编辑软件！祝您使用愉快！</p>"));
}
//! [16]

//! [17]
void ImageViewer::createActions()
//! [17] //! [18]
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &ImageViewer::open);
    openAct->setShortcut(QKeySequence::Open);

    saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &ImageViewer::saveAs);
    saveAsAct->setEnabled(false);

    saveAct = fileMenu->addAction(tr("&Save ..."), this, &ImageViewer::save);
    saveAct->setEnabled(false);

//    printAct = fileMenu->addAction(tr("&Print..."), this, &ImageViewer::print);
//    printAct->setShortcut(QKeySequence::Print);
//    printAct->setEnabled(false);

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    brightAct = editMenu->addAction(tr("&Bright"), this, &ImageViewer::Bright1);
    brightAct->setShortcut(tr("Ctrl+B"));
    brightAct->setEnabled(true);

    darkAct = editMenu->addAction(tr("&Dark"), this, &ImageViewer::Darker1);
    darkAct->setShortcut(tr("Ctrl+D"));
    darkAct->setEnabled(true);

    contrastAct = editMenu->addAction(tr("&Contrast"), this, &ImageViewer::lightContrast);
    contrastAct->setShortcut(tr("Ctrl+L"));
    contrastAct->setEnabled(true);

    greyAct = editMenu->addAction(tr("&Grey"), this, &ImageViewer::greyScale);
    greyAct->setShortcut(tr("Ctrl+G"));
    greyAct->setEnabled(true);

    warmAct = editMenu->addAction(tr("&Warm"), this, &ImageViewer::warm);
    warmAct->setShortcut(tr("Ctrl+W"));
    warmAct->setEnabled(true);

    coldAct = editMenu->addAction(tr("&Cold"), this, &ImageViewer::cold);
    coldAct->setShortcut(tr("Ctrl+E"));
    coldAct->setEnabled(true);

    saturateAct = editMenu->addAction(tr("&Saturate"), this, &ImageViewer::saturation);
    saturateAct->setShortcut(tr("Ctrl+T"));
    saturateAct->setEnabled(true);
//    QAction *pasteAct = editMenu->addAction(tr("&Paste"), this, &ImageViewer::paste);
//    pasteAct->setShortcut(QKeySequence::Paste);

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
    helpMenu->addAction(tr("About &Qt"), this, &QApplication::aboutQt);
}
//! [18]

//! [21]
void ImageViewer::updateActions()
//! [21] //! [22]
{
    saveAsAct->setEnabled(!image.isNull());
    saveAct->setEnabled(!image.isNull());
//    copyAct->setEnabled(!image.isNull());
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}
//! [22]

//! [23]
void ImageViewer::scaleImage(double factor)
//! [23] //! [24]
{
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}
//! [24]

//! [25]
void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
//! [25] //! [26]
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}
