#include "connectablelist.h"

ConnectableList::ConnectableList(QObject *parent) : QObject(parent){ }



void ConnectableList::insert(ListElement* el, int i)
{
    list->insert(i, el);
    elementWasAdded(i);
}

void ConnectableList::remove(int i)
{
    list->removeAt(i);
    elementWasRemoved(i);
}

void ConnectableList::remove(ListElement* el)
{
    int i = list->indexOf(el);
    list->removeAt(i);
    elementWasRemoved(i);
}

void ConnectableList::push_back(ListElement* el)
{
    list->push_back(el);
    elementWasAdded(list->count() - 1);
}

void ConnectableList::pop_back()
{
    list->pop_back();
    elementWasAdded(list->count());
}

ListElement* ConnectableList::last()
{
    return list->last();
}

int ConnectableList::count()
{
    return list->count();
}
