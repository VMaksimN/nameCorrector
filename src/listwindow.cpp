#include "listwindow.h"

ListWindow::ListWindow(QString title, QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle(title);
    this->source = new QList<ListElement>();
    init();
}

ListWindow::ListWindow(QList<ListElement>* source, QString title, QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle(title);
    this->source = source;
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
    mainGrid->addWidget(scrollArea, 0, 0, 1, 2, Qt::AlignTop);

    listLayout = new QVBoxLayout(listWidget);

    for(int i = 0; i < 10; i++)
    {
        ListElement* el = new ListElement("QW", "qw", true);
        ListElement_GUI* elg = new ListElement_GUI(el, listWidget);
        listLayout->addWidget(elg);
    }

    removeSelectedButton = new QPushButton("Remove selected", mainWidget);
    mainGrid->addWidget(removeSelectedButton, 1, 0, 1, 1);

    disableSelectedButton = new QPushButton("Disable selected", mainWidget);
    mainGrid->addWidget(disableSelectedButton, 1, 1, 1, 1);

    clearListButton = new QPushButton("Clear list", mainWidget);
    mainGrid->addWidget(clearListButton, 2, 0, 1, 1);

    addButton = new QPushButton("Add", mainWidget);
    mainGrid->addWidget(addButton, 2, 1, 1, 1);
}
