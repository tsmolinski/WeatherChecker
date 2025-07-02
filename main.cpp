#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //QString styleSheet = "QWidget{font-size :16px;}";
    //a.setStyleSheet(styleSheet);

    w.show();
    return a.exec();
}
