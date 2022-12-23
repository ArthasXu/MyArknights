#include "mainwindow.h"
#include "startfrom.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    startFrom w;
    w.move((1600 - w.width()) / 2, (800 - w.height()) / 2);
    w.show();
    return a.exec();
}
