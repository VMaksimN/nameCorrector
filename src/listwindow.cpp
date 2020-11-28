#include "listwindow.h"

ListWindow::ListWindow(ConnectableList* source, QString title, QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle(title);
    this->source = source;
    connect(this->source, &ConnectableList::elementAdded,
            this, &ListWindow::source_ElementAdded);
    /*connect(this->source, &ConnectableList::elementRemoved,
            this, &ListWindow::source_ElementRemoved);*/
    connect(this->source, &ConnectableList::cleared,
            this, &ListWindow::claerButton_Clicked);
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
        connect(leg, &ListElement_GUI::selectedStateChanged, this, &ListWindow::element_SelectedStateChanged);
        //connect(leg, &ListElement_GUI::deleted, this, &ListWindow::elementDeleted);

        listLayout->addWidget(leg);
        leg->setParentLayout(listLayout);
    }

    removeSelectedButton = new QPushButton("Remove selected", mainWidget);
    connect(removeSelectedButton, &QPushButton::clicked, this, &ListWindow::removeSelectedButton_Clicked);
    mainGrid->addWidget(removeSelectedButton, 1, 0, 1, 1);

    disableSelectedButton = new QPushButton("Disable selected", mainWidget);
    connect(disableSelectedButton, &QPushButton::clicked, this, &ListWindow::disableSelectedButton_Clicked);
    mainGrid->addWidget(disableSelectedButton, 1, 1, 1, 1);

    enableSelectedButton = new QPushButton("Enable selected", mainWidget);
    connect(enableSelectedButton, &QPushButton::clicked, this, &ListWindow::enableSelectedButton_Clicked);
    mainGrid->addWidget(enableSelectedButton, 1, 2, 1, 1);

    clearButton = new QPushButton("Clear list", mainWidget);
    connect(clearButton, &QPushButton::clicked, this, &ListWindow::claerButton_Clicked);
    mainGrid->addWidget(clearButton, 2, 0, 1, 1);

    addButton = new QPushButton("Add", mainWidget);
    connect(addButton, &QPushButton::clicked, this, &ListWindow::addButton_Clicked);
    mainGrid->addWidget(addButton, 2, 1, 2, 1);

    selectedElements = new QList<ListElement_GUI*>();
}



//Signal handlers

void ListWindow::addButton_Clicked()
{
    NewElementWindow* nelw = new NewElementWindow(windowTitle());
    connect(nelw, &NewElementWindow::elementCreated, this, &ListWindow::newElWin_elementCreated);
    QApplication::setActiveWindow(nelw);
    nelw->show();
}

void ListWindow::source_ElementAdded(int i)
{
    ListElement_GUI* leg = new ListElement_GUI(source->at(i), listWidget);
    connect(leg, &ListElement_GUI::selectedStateChanged, this, &ListWindow::element_SelectedStateChanged);

    listLayout->addWidget(leg);
    leg->setParentLayout(listLayout);
}

void ListWindow::claerButton_Clicked()
{
    /*if(source->count() > 0)
    {
        source->clear();
    }*/
    for(int i = listLayout->count(); i > -1; i--)
    {
        if(listWidget->children().at(i) != (QLayout*)listLayout &&
           listWidget->children().at(i) != (QLayout*)scrollArea)
        {
            ((ListElement_GUI*)listWidget->children().at(i))->removeThis();
            //delete ((ListElement_GUI*)listWidget->children().at(i));
        }
    }
    //selectedElements->clear();
}

void ListWindow::disableSelectedButton_Clicked()
{
    for(int i = 0; i < selectedElements->count(); i++)
    {
        selectedElements->at(i)->setEnabled(false);
    }
}

void ListWindow::element_SelectedStateChanged(bool state)
{
    ListElement_GUI* leg = (ListElement_GUI*)sender();
    if(state)
    {
        selectedElements->push_back(leg);
        leg->linkCollection(selectedElements);
        return;
    }

    selectedElements->removeOne(leg);
    leg->unlinkCollection(selectedElements);
}

void ListWindow::newElWin_elementCreated()
{
    ListElement* a = ((NewElementWindow*)sender())->getResult();
    source->push_back(a);
}

void ListWindow::element_Deleted()
{
    /*ListElement_GUI* deleted = (ListElement_GUI*)sender();

    selectedElements->removeOne(deleted);
    listLayout->removeWidget(deleted);
    source->removeById(deleted->getSource().getId());*/
}

void ListWindow::enableSelectedButton_Clicked()
{
    for(int i = 0; i < selectedElements->count(); i++)
    {
        selectedElements->at(i)->setEnabled(true);
    }
}

void ListWindow::removeSelectedButton_Clicked()
{
    for(int i = selectedElements->count() - 1; i > -1; i--)
    {
        ListElement_GUI* deleted = selectedElements->at(i);
        deleted->removeThis();
        /*selectedElements->removeOne(deleted);
        delete deleted;*/
    }
}



















