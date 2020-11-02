#include "mainwindow.h"
#include "listwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*ListWindow lw("Rules");
    lw.show();*/

    MainWindow w;
    w.show();
    return a.exec();
}
