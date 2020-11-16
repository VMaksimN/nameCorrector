#include "listelement.h"

ListElement::ListElement() : QObject(){}
ListElement::ListElement(ListElement&){}

ListElement::ListElement(QString name, QString info, QString type, bool is_editable, bool is_closable) : QObject()
{
    *this->name = name;
    *this->info = info;
    this->type = type;
    this->is_editable = is_editable;
    this->is_closable = is_closable;
}

int ListElement::getId()
{
    return id;
}

QString ListElement::getName()
{
    return *name;
}

bool ListElement::isEnabled()
{
    return is_enabled;
}

bool ListElement::isEditable()
{
    return is_editable;
}

QString ListElement::getInfo()
{
    return *info;
}

bool ListElement::isClosable()
{
    return is_closable;
}




void ListElement::setName(QString name)
{
    *(this->name) = name;
}

void ListElement::setEnabled(bool val)
{
    *is_enabled = val;
}

void ListElement::setEditable(bool val)
{
    is_editable = val;
}

void ListElement::setInfo(QString info)
{
    *this->info = info;
}

void ListElement::setClosable(bool val)
{
    is_closable = val;
}

QString ListElement::getType()
{
    return type;
}
