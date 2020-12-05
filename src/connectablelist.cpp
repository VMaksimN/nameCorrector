/*    This file is part of name_corrector
    Copyright (C) 2020 Вершинин Максим

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
















