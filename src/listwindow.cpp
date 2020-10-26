#include "listwindow.h"

ListWindow::ListWindow(QWidget* parent) : QMainWindow(parent)
{
    this->source = new QList<ListElement>();
}

ListWindow::ListWindow(QList<ListElement>* source, QWidget* parent) : QMainWindow(parent)
{
    this->source = source;
}
