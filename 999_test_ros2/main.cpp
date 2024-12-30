#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    w.setMinimumHeight(1900);
//    w.setMinimumWidth(3050);
//    w.setMaximumHeight(1900);
//    w.setMaximumWidth(3050);
    w.show();
    //w.showFullScreen();
    return a.exec();
}
