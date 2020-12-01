#ifndef LISTWINDOW_H
#define LISTWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QGridLayout>


#include "listelement_gui.h"
#include "newelementwindow.h"
#include "connectablelist.h"


class ListWindow : public QMainWindow
{
    Q_OBJECT
public:
    ListWindow(ListWindow&){}
    explicit ListWindow(ConnectableList* source, QString title, QWidget *parent = nullptr);

private:

    //Signals handlers
    void addButton_Clicked();
    void claerButton_Clicked();
    void disableSelectedButton_Clicked();
    void element_SelectedStateChanged(bool state);
    void enableSelectedButton_Clicked();
    void newElWin_elementCreated();
    void newElWin_elementDeleted();
    void removeSelectedButton_Clicked();
    void source_ElementAdded(int i);
    void source_ElementRemoved();

    //Other methods
    void init();

    //GUI fields
    QWidget* mainWidget;
    QGridLayout* mainGrid;
    QWidget* listWidget;
    QVBoxLayout* listLayout;
    QScrollArea* scrollArea;

    QPushButton* removeSelectedButton;
    QPushButton* disableSelectedButton;
    QPushButton* enableSelectedButton;
    QPushButton* clearButton;
    QPushButton* addButton;

    ConnectableList* source;

    QList<ListElement_GUI*>* selectedElements;
    ListElement_GUI* deletedElement;

signals:

};

#endif // LISTWINDOW_H
