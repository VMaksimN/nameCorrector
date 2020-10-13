#ifndef LISTWINDOW_H
#define LISTWINDOW_H

#include <QMainWindow>
#include "listelement_gui.h"
class ListWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ListWindow(QWidget *parent = nullptr);

signals:

};

#endif // LISTWINDOW_H
