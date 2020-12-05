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

#ifndef LISTELEMENT_H
#define LISTELEMENT_H

#include<QObject>
#include<QList>
#include<QSet>
#include<QWidget>
#include<QLayout>
#include<QPair>

class ListElement : public QObject
{
    Q_OBJECT
public:
    explicit ListElement();
    ListElement(ListElement&);
    explicit ListElement(QString name, QString info = "", QString type = "rule",
                         bool is_editable = false, bool is_closable = true);

    //Get-methods
    int getId();
    QString getInfo();
    QString getName();
    QString getType();
    bool isClosable();
    bool isEditable();
    bool isEnabled();

    //Set-methods
    void setClosable(bool val);
    void setEditable(bool val);
    void setEnabled(bool val);
    void setInfo(QString info);
    void setName(QString name);

    //Other
    void addGUI(QWidget* gui);
    void removeGUI();

private:

    QSet<QWidget*> connectedGUI;
    int id;
    static int id_counter;
    QString* info = new QString();
    bool is_closable;
    bool is_editable;
    bool* is_enabled = new bool();
    QString* name = new QString();
    QString type;

signals:
    void deleted(int id);
};

#endif // LISTELEMENT_H
