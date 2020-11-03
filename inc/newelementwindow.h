#ifndef NEWELEMENTWINDOW_H
#define NEWELEMENTWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include "listelement.h"

class NewElementWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit NewElementWindow(QString type, QWidget *parent = nullptr);

private:
    QWidget* mainWidget;
    QVBoxLayout* mainLayout;

    QHBoxLayout* typeLayout;
    QHBoxLayout* ruleLayout;
    QHBoxLayout* buttonsLayout;

    QLabel* ruleLabel;
    QComboBox* typeComboBox;
    QPushButton* helpButton;

    QLabel* typeLabel;
    /*Other fields*/

    QPushButton* cancelButton;
    QPushButton* clearButton;
    QPushButton* createButton;

signals:

};

#endif // NEWELEMENTWINDOW_H
