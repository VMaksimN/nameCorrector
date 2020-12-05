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

#ifndef LISTWINDOW_H
#define LISTWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QGridLayout>


#include "listelement_gui.h"
#include "newelementwindow.h"
#include "connectablelist.h"


class ListWindow : public QMainWindow
{
    Q_OBJECT
public:
    ListWindow(ListWindow&){}
    explicit ListWindow(ConnectableList* source, QString title, QWidget *parent = nullptr);

private:

    //Signals handlers
    void addButton_Clicked();
    void claerButton_Clicked();
    void disableSelectedButton_Clicked();
    void element_SelectedStateChanged(bool state);
    void enableSelectedButton_Clicked();
    void newElWin_elementCreated();
    void newElWin_elementDeleted();
    void removeSelectedButton_Clicked();
    void source_ElementAdded(int i);
    void source_ElementRemoved();

    //Other methods
    void init();

    //GUI fields
    QWidget* mainWidget;
    QGridLayout* mainGrid;
    QWidget* listWidget;
    QVBoxLayout* listLayout;
    QScrollArea* scrollArea;

    QPushButton* removeSelectedButton;
    QPushButton* disableSelectedButton;
    QPushButton* enableSelectedButton;
    QPushButton* clearButton;
    QPushButton* addButton;

    ConnectableList* source;

    QList<ListElement_GUI*>* selectedElements;
    ListElement_GUI* deletedElement;

signals:

};

#endif // LISTWINDOW_H
