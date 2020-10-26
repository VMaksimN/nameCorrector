#include "listelement_gui.h"

ListElement_GUI::ListElement_GUI(ListElement* source, QWidget *parent) : QWidget(parent)
{
    this->source = source;
    mainStack = new  QHBoxLayout(this);
    setLayout(mainStack);

    title = new QLabel(source->getName(), this);
    mainStack->addWidget(title);

    info = new QLabel(source->getDescription(), this);
    mainStack->addWidget(info);

    enableCheckBox = new QCheckBox("Enabled", this);
    mainStack->addWidget(enableCheckBox);
    connect(enableCheckBox, &QCheckBox::clicked, this, &ListElement_GUI::enableCheckBox_Clicked);

    if(source->isEditable())
    {
        editButton = new QPushButton("Edit", this);
        mainStack->addWidget(editButton);
        connect(editButton, &QPushButton::clicked, this, &ListElement_GUI::editButton_Clicked);
    }

    if(source->isClosable())
    {
        removeButton = new QPushButton("Remove", this);
        mainStack->addWidget(removeButton);
        connect(removeButton, &QPushButton::clicked, this, &ListElement_GUI::removeButton_Clicked);
    }
}

ListElement_GUI::ListElement_GUI(ListElement_GUI&)
{

}

QString ListElement_GUI::getTitle()
{
    return title->text();
}

ListElement ListElement_GUI::getSource()
{
    return *source;
}

void ListElement_GUI::setTitle(QString text)
{
    title->setText(text);
}

void ListElement_GUI::setInfo(QString text)
{
    info->setText(text);
}

QString ListElement_GUI::getInfo()
{
    return info->text();
}

void ListElement_GUI::removeButton_Clicked()
{
    ((QWidget*)parent())->layout()->removeWidget(this);
    delete this;
}

void ListElement_GUI::editButton_Clicked()
{

}

void ListElement_GUI::enableCheckBox_Clicked()
{
    source->setEnabled(enableCheckBox->isChecked());
}
