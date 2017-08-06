#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    int wi = w.width();
    int h = w.height();
    w.setFixedSize(wi,h);
    w.setWindowTitle("Draw-It !");
    w.setIconSize(QSize(100,500));
    w.setWindowIcon(QIcon(":/iconeAppli.png"));
    w.show();

    return a.exec();
}
