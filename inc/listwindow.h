#ifndef LISTWINDOW_H
#define LISTWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QGridLayout>
#include "listelement_gui.h"
#include "newelementwindow.h"
class ListWindow : public QMainWindow
{
    Q_OBJECT
public:
    ListWindow(ListWindow&){}
    explicit ListWindow(QList<ListElement>* source, QString title, QWidget *parent = nullptr);
private:

    void init();
    void claerList_ButtonClicked();
    void disableSelected_ButtonClicked();
    void enableSelected_ButtonClicked();
    void removeSelected_ButtonClicked();
    void elementSelectedStateChanged(bool state);
    void addButtonClicked();
    void elementWasCreated();

    QWidget* mainWidget;
    QGridLayout* mainGrid;
    QWidget* listWidget;
    QVBoxLayout* listLayout;
    QScrollArea* scrollArea;

    QPushButton* removeSelectedButton;
    QPushButton* disableSelectedButton;
    QPushButton* enableSelectedButton;
    QPushButton* clearListButton;
    QPushButton* addButton;

    QList<ListElement>* source;

    QList<ListElement_GUI*>* selectedElements;

signals:

};

#endif // LISTWINDOW_H
