#include "listelement_gui.h"


ListElement_GUI::ListElement_GUI(ListElement* source, QWidget *parent) : QPushButton(parent)
{
    //INit this
    this->source = source;
    connect(this, &QPushButton::clicked, this, &ListElement_GUI::this_clicked);

    //INit layout
    mainStack = new QHBoxLayout(this);
    mainStack->setMargin(0);
    setLayout(mainStack);

    //INit lables and other gui's
    title = new QLabel(source->getName(), this);
    title->setMargin(5);
    mainStack->addWidget(title);

    info = new QLabel(source->getInfo(), this);
    mainStack->addWidget(info);

    enableCheckBox = new QCheckBox("Enabled", this);
    if(source->isEnabled())
    {
        enableCheckBox->setCheckState(Qt::Checked);
    }
    else
    {
        enableCheckBox->setCheckState(Qt::Unchecked);
    }
    connect(enableCheckBox, &QCheckBox::stateChanged, this, &ListElement_GUI::enableCheckBox_StateChanged);
    mainStack->addWidget(enableCheckBox);

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
    is_selected = false;

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

bool ListElement_GUI::isSelected()
{
    return is_selected;
}

void ListElement_GUI::setSelected(bool value)
{
    is_selected = value;
    selectedStateChanged(value);
    if(value)
    {
        title->setStyleSheet(selectedStyleSheet);
        return;
    }
    title->setStyleSheet(defaultStyleSheet);
}

void ListElement_GUI::removeButton_Clicked()
{
    //((QWidget*)parent())->layout()->removeWidget(this);
    deleted();
    delete this;
}

void ListElement_GUI::editButton_Clicked()
{

}

void ListElement_GUI::enableCheckBox_StateChanged()
{
    source->setEnabled(enableCheckBox->isChecked());
}

bool ListElement_GUI::isEnabled()
{
    return source->isEnabled();
}

void ListElement_GUI::setEnabled(bool value)
{
    source->setEnabled(value);

    if(value)
    {
        enableCheckBox->setCheckState(Qt::Checked);
        return;
    }
    enableCheckBox->setCheckState(Qt::Unchecked);
}

void ListElement_GUI::this_clicked()
{
    is_selected = !is_selected;
    selectedStateChanged(is_selected);
    if(is_selected)
    {
        title->setStyleSheet(selectedStyleSheet);
        return;
    }
    title->setStyleSheet(defaultStyleSheet);
}
