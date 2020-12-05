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

#include "listelement.h"

int ListElement::id_counter = 0;

ListElement::ListElement() : QObject(){}
ListElement::ListElement(ListElement&){}

ListElement::ListElement(QString name, QString info, QString type, bool is_editable, bool is_closable) : QObject()
{
    *this->name = name;
    *this->info = info;
    this->type = type;
    this->is_editable = is_editable;
    this->is_closable = is_closable;

    id = id_counter;
    id_counter++;
}


/////Get-methods

int ListElement::getId()
{
    return id;
}

QString ListElement::getInfo()
{
    return *info;
}

QString ListElement::getName()
{
    return *name;
}

QString ListElement::getType()
{
    return type;
}

bool ListElement::isClosable()
{
    return is_closable;
}

bool ListElement::isEditable()
{
    return is_editable;
}

bool ListElement::isEnabled()
{
    return is_enabled;
}

//////////////
//////////////
//////////////



/////Set-methods

void ListElement::setClosable(bool val)
{
    is_closable = val;
}

void ListElement::setEditable(bool val)
{
    is_editable = val;
}

void ListElement::setEnabled(bool val)
{
    *is_enabled = val;
}

void ListElement::setInfo(QString info)
{
    *this->info = info;
}

void ListElement::setName(QString name)
{
    *(this->name) = name;
}

//////////////
//////////////
//////////////


/////Set-methods
void ListElement::addGUI(QWidget* gui)
{
    connectedGUI.insert(gui);
}

void ListElement::removeGUI()
{
    QWidget* deleted;
    for(int i = connectedGUI.count() - 1; i > -1; i--)
    {
        //Delete all of the GUI-elements connected to this
        deleted = *connectedGUI.begin();
        connectedGUI.remove(deleted);
        delete deleted;
    }
    //Sign for all collections containing this element to delete it
    this->deleted(id);
    delete this;
}














