#include "imageviewer.h"

#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageViewer w;

    w.show();
    return a.exec();
}
