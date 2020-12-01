#include "listelement_gui.h"


ListElement_GUI::ListElement_GUI(ListElement* source, QWidget *parent) : QPushButton(parent)
{
    //INit this
    this->source = source;
    source->addGUI(this);
    connect(this, &QPushButton::clicked, this, &ListElement_GUI::this_clicked);

    //INit layout
    mainStack = new QHBoxLayout(this);
    mainStack->setMargin(0);
    setLayout(mainStack);

    //INit lables
    title = new QLabel(source->getName(), this);
    title->setMargin(5);
    mainStack->addWidget(title);

    info = new QLabel(source->getInfo(), this);
    mainStack->addWidget(info);

    //INit enableCheckBox
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

    //INit buttons
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
        connect(removeButton, &QPushButton::clicked, this, &ListElement_GUI::removeThis);
    }

    is_selected = false;

}

ListElement_GUI::ListElement_GUI(ListElement_GUI&){}

ListElement_GUI::~ListElement_GUI()
{
    //Remove useless pointer from everywhere
    parentLayout->removeWidget(this);
    for(int i = 0; i < linkedCollections.count(); i++)
    {
        linkedCollections.at(i)->removeOne(this);
    }
}




/////Get-methods

QString ListElement_GUI::getInfo()
{
    return info->text();
}

QLayout* ListElement_GUI::getParentLayout()
{
    return parentLayout;
}

ListElement ListElement_GUI::getSource()
{
    return *source;
}

QString ListElement_GUI::getTitle()
{
    return title->text();
}

bool ListElement_GUI::isEnabled()
{
    return source->isEnabled();
}

bool ListElement_GUI::isSelected()
{
    return is_selected;
}

//////////////
//////////////
//////////////




/////Set-methods

//It is necessary to set check-state from outside (w/o click)
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

void ListElement_GUI::setInfo(QString text)
{
    info->setText(text);
}

void ListElement_GUI::setParentLayout(QLayout *pl)
{
    parentLayout = pl;
}

//It is necessary to set selected-state from outside (w/o click)
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

void ListElement_GUI::setTitle(QString text)
{
    title->setText(text);
}

//////////////
//////////////
//////////////


/////Other methdos

void ListElement_GUI::linkCollection(QList<ListElement_GUI*>* col)
{
    linkedCollections.push_back(col);
}

void ListElement_GUI::unlinkAllCollections()
{
    linkedCollections.clear();
}

void ListElement_GUI::unlinkCollection(QList<ListElement_GUI*>* col)
{
    linkedCollections.removeOne(col);
}

void ListElement_GUI::removeThis()
{
    //All GUI-elements connected to the source element
    //including this will be deleted
    source->removeGUI();
}

//////////////
//////////////
//////////////



/////Signals handlers

void ListElement_GUI::editButton_Clicked()
{

}

void ListElement_GUI::enableCheckBox_StateChanged()
{
    source->setEnabled(enableCheckBox->isChecked());
}

//Change the selected-state by default method (mouseLB click)
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









