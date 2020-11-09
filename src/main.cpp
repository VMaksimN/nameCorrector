#include "mainwindow.h"
#include "listwindow.h"
#include "newelementwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ListWindow lw("Rules");
    lw.show();

    /*ListElement* lel = new ListElement();
    NewElementWindow nelw("rules", lel);
    nelw.show();*/


    /*MainWindow w;
    w.show();*/
    return a.exec();
}
