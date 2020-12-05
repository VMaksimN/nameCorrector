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

#ifndef NEWELEMENTWINDOW_H
#define NEWELEMENTWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QDir>
#include <QLabel>
#include <QApplication>
#include <QComboBox>
#include <QPushButton>
#include <QString>
#include <QTextEdit>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QToolTip>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>


#include "listelement.h"

class NewElementWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit NewElementWindow(QString type, QWidget *parent = nullptr);
    ~NewElementWindow(){}

    //Get method
    ListElement* getResult();

private:

    //Signals handlers
    void browseButton_Clicked();
    void cancelButton_Clicked();
    void createButton_Clicked();
    void typeComboBox_ItemChanged();

    //Other methods
    void checkTextBox();
    void writeDataToResult();

    //GUI fields
    QWidget* mainWidget;
    QVBoxLayout* mainLayout;

    QHBoxLayout* typeLayout;
    QHBoxLayout* ruleLayout;
    QHBoxLayout* buttonsLayout;

    QLabel* typeLabel;
    QComboBox* typeComboBox;
    QPushButton* helpButton;
    QPushButton* browseButton;


    QLabel* ruleLabel;

    QTextEdit* pathTextBox;
    QTextEdit* replacedTextBox;
    QTextEdit* replaceWithTextBox;
    QTextEdit* removeTextBox;
    QTextEdit* fromTextBox;
    QTextEdit* toTextBox;
    QTextEdit* addTextBox;
    QComboBox* positionComboBox;
    QComboBox* listType_ComboBox;



    QPushButton* cancelButton;
    QPushButton* clearButton;
    QPushButton* createButton;


    //Other fields
    ListElement* result;
    bool was_created = false;

    char* dangerous_symbols;
    QPalette def_pal;
    QPalette red_pal;

signals:
    void elementCreated();
};

#endif // NEWELEMENTWINDOW_H
