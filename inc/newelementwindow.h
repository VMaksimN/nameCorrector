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
#include <QVBoxLayout>
#include <QToolTip>
#include <QFile>
#include <QFileInfo>
#include "listelement.h"

class NewElementWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit NewElementWindow(QString type, QWidget *parent = nullptr);
    ~NewElementWindow(){}

    ListElement* getResult();
private:
    void typeComboBox_ItemChanged();
    void checkTextBox();
    void createButtonClicked();
    void cancelButtonClicked();
    void writeDataToResult();


    QWidget* mainWidget;
    QVBoxLayout* mainLayout;

    QHBoxLayout* typeLayout;
    QHBoxLayout* ruleLayout;
    QHBoxLayout* buttonsLayout;

    QLabel* typeLabel;
    QComboBox* typeComboBox;
    QPushButton* helpButton;


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
    void elementWasCreated();
};

#endif // NEWELEMENTWINDOW_H
