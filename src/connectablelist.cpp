#include "connectablelist.h"

ConnectableList::ConnectableList(QObject *parent) : QObject(parent)
{
    list = new QList<ListElement*>();
}

/////Get-methods

ListElement* ConnectableList::at(int i)
{
    return list->at(i);
}

int ConnectableList::count()
{
    return list->count();
}

ListElement* ConnectableList::last()
{
    return list->last();
}

//////////////
//////////////
//////////////



/////Other methods

void ConnectableList::clear()
{
    list->clear();
    cleared();
}

void ConnectableList::insert(ListElement* el, int i)
{
    list->insert(i, el);
    elementAdded(i);
    connect(el, &ListElement::deleted, this, &ConnectableList::removeById);
}

void ConnectableList::pop_back()
{
    list->pop_back();
    elementAdded(list->count());
}

void ConnectableList::push_back(ListElement* el)
{
    list->push_back(el);
    elementAdded(list->count() - 1);
    connect(el, &ListElement::deleted, this, &ConnectableList::removeById);
}

void ConnectableList::remove(ListElement* el)
{
    int i = list->indexOf(el);
    list->removeAt(i);
    elementRemoved();
}

void ConnectableList::removeAt(int i)
{
    list->removeAt(i);
    elementRemoved();
}

void ConnectableList::removeById(int id)
{
    for(int i = 0; i < list->count(); i++)
    {
        if(list->at(i)->getId() == id)
        {
            list->removeAt(i);
            break;
        }
    }
    elementRemoved();
}
















