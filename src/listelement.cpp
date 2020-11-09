#include "listelement.h"

ListElement::ListElement() : QObject()
{

}

ListElement::ListElement(ListElement&)
{

}

ListElement::ListElement(QString name, QString desc, QString type, bool is_editable, bool is_closable) : QObject()
{
    *(this->name) = name;
    *(this->description) = desc;
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

QList<QPair<QString, QString>>* ListElement::getData()
{
    return data;
}

bool ListElement::isEditable()
{
    return is_editable;
}

QString ListElement::getDescription()
{
    return *description;
}

bool ListElement::isClosable()
{
    return is_closable;
}

bool ListElement::wasCreated()
{
    return was_created;
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

void ListElement::setDescription(QString desc)
{
    *description = desc;
}

void ListElement::setClosable(bool val)
{
    is_closable = val;
}

void ListElement::setCreated(bool val)
{
    was_created = val;
}
