#include <QApplication>


#include "mainwindow.h"
#include "listwindow.h"
#include "newelementwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*ConnectableList* source = new ConnectableList();
    ListWindow lw(source, "Rules");
    lw.show();*/

    /*ListElement* lel = new ListElement();
    NewElementWindow nelw("rules", lel);
    nelw.show();*/


    MainWindow w;
    w.show();
    return a.exec();
}
