#include "listwindow.h"

ListWindow::ListWindow(ConnectableList* source, QString title, QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle(title);
    this->source = source;
    connect(this->source, &ConnectableList::elementWasAdded,
            this, &ListWindow::addGUIElement);
    connect(this->source, &ConnectableList::elementWasRemoved,
            this, &ListWindow::removeGUIElement);
    init();
}

void ListWindow::init()
{
    //init main widget and layout
    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    mainGrid = new QGridLayout(mainWidget);

    //Init list area
    listWidget = new QWidget(mainWidget);

    scrollArea = new QScrollArea(mainWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(listWidget);
    scrollArea->setMinimumWidth(400);
    mainGrid->addWidget(scrollArea, 0, 0, 1, 3, Qt::AlignTop);

    listLayout = new QVBoxLayout(listWidget);
    listLayout->setAlignment(Qt::AlignTop);

    for(int i = 0; i < source->count(); i++)
    {
        ListElement_GUI* leg = new ListElement_GUI(source->at(i), listWidget);
        connect(leg, &ListElement_GUI::selectedStateChanged, this, &ListWindow::elementSelectedStateChanged);
        connect(leg, &ListElement_GUI::deleted, this, &ListWindow::elementDeleted);
        listLayout->addWidget(leg);
    }

    removeSelectedButton = new QPushButton("Remove selected", mainWidget);
    connect(removeSelectedButton, &QPushButton::clicked, this, &ListWindow::removeSelected_ButtonClicked);
    mainGrid->addWidget(removeSelectedButton, 1, 0, 1, 1);

    disableSelectedButton = new QPushButton("Disable selected", mainWidget);
    connect(disableSelectedButton, &QPushButton::clicked, this, &ListWindow::disableSelected_ButtonClicked);
    mainGrid->addWidget(disableSelectedButton, 1, 1, 1, 1);

    enableSelectedButton = new QPushButton("Enable selected", mainWidget);
    connect(enableSelectedButton, &QPushButton::clicked, this, &ListWindow::enableSelected_ButtonClicked);
    mainGrid->addWidget(enableSelectedButton, 1, 2, 1, 1);

    clearListButton = new QPushButton("Clear list", mainWidget);
    connect(clearListButton, &QPushButton::clicked, this, &ListWindow::claerList_ButtonClicked);
    mainGrid->addWidget(clearListButton, 2, 0, 1, 1);

    addButton = new QPushButton("Add", mainWidget);
    connect(addButton, &QPushButton::clicked, this, &ListWindow::addButtonClicked);
    mainGrid->addWidget(addButton, 2, 1, 2, 1);

    selectedElements = new QList<ListElement_GUI*>();
}


void ListWindow::claerList_ButtonClicked()
{
    for(int i = listLayout->count(); i > -1; i--)
    {
        if(listWidget->children().at(i) != (QLayout*)listLayout &&
           listWidget->children().at(i) != (QLayout*)scrollArea)
        {
            delete listWidget->children().at(i);
        }
    }
    selectedElements = new QList<ListElement_GUI*>();
}

void ListWindow::disableSelected_ButtonClicked()
{
    for(int i = 0; i < selectedElements->count(); i++)
    {
        selectedElements->at(i)->setEnabled(false);
    }
}

void ListWindow::removeSelected_ButtonClicked()
{
    for(int i = selectedElements->count() - 1; i > -1; i--)
    {
        ListElement_GUI* deleted = selectedElements->at(i);
        selectedElements->removeOne(deleted);
        delete deleted;
    }
}

void ListWindow::enableSelected_ButtonClicked()
{
    for(int i = 0; i < selectedElements->count(); i++)
    {
        selectedElements->at(i)->setEnabled(true);
    }
}

void ListWindow::addButtonClicked()
{
    NewElementWindow* nelw = new NewElementWindow(windowTitle());
    connect(nelw, &NewElementWindow::elementWasCreated, this, &ListWindow::elementWasCreated);
    QApplication::setActiveWindow(nelw);
    nelw->show();
}

void ListWindow::elementSelectedStateChanged(bool state)
{
    if(state)
    {
        selectedElements->push_back((ListElement_GUI*)sender());
        return;
    }
    selectedElements->removeOne((ListElement_GUI*)sender());
}


void ListWindow::elementWasCreated()
{
    ListElement* a = ((NewElementWindow*)sender())->getResult();
    source->push_back(a);
}

void ListWindow::addGUIElement(int i)
{
    ListElement_GUI* leg = new ListElement_GUI(source->at(i), listWidget);
    connect(leg, &ListElement_GUI::selectedStateChanged, this, &ListWindow::elementSelectedStateChanged);
    connect(leg, &ListElement_GUI::deleted, this, &ListWindow::elementDeleted);
    listLayout->addWidget(leg);
}

void ListWindow::elementDeleted()
{
    deletedElement = (ListElement_GUI*)sender();
    selectedElements->removeOne(deletedElement);

    int del_id = (deletedElement)->getSource().getId();
    for(int i = 0; i < source->count(); i++)
    {
        if(source->at(i)->getId() == del_id)
        {
            source->remove(i);
            return;
        }
    }
}

void ListWindow::removeGUIElement()
{
    /*listLayout->removeWidget(deletedElement);
    if(deletedElement != )
    {
        delete deletedElement;
        deletedElement = NULL;
    }*/
}




















