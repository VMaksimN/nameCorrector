#include "newelementwindow.h"


NewElementWindow::NewElementWindow(QString type, QWidget *parent) : QMainWindow(parent)
{
    if(type == "rule")
    {
        //Some code
    }
    else
    {
        //Another code
    }

    //INit main
    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    mainLayout = new QVBoxLayout(mainWidget);
    mainWidget->setLayout(mainLayout);


    //INit the type row
    typeLayout = new QHBoxLayout(mainWidget);
    mainLayout->addLayout(typeLayout);

    typeLabel = new QLabel("Select the element type");
    typeLayout->addWidget(typeLabel);

    typeComboBox = new QComboBox(mainWidget);
    typeComboBox->addItem("Some action");
    typeComboBox->addItem("The second action");
    typeComboBox->addItem("Another action");
    typeLayout->addWidget(typeComboBox);

    helpButton = new QPushButton("Help", mainWidget);
    typeLayout->addWidget(helpButton);


    //INit the rule row
    ruleLayout = new QHBoxLayout(mainWidget);
    mainLayout->addLayout(ruleLayout);

    ruleLabel = new QLabel("");
    ruleLayout->addWidget(ruleLabel);


    //INit the buttons row
    buttonsLayout = new QHBoxLayout(mainWidget);
    mainLayout->addLayout(buttonsLayout);

    cancelButton = new QPushButton("Cancel", mainWidget);
    buttonsLayout->addWidget(cancelButton);

    clearButton = new QPushButton("Clear", mainWidget);
    buttonsLayout->addWidget(clearButton);

    createButton = new QPushButton("Create", mainWidget);
    buttonsLayout->addWidget(createButton);
}



















