#ifndef LISTELEMENT_GUI_H
#define LISTELEMENT_GUI_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include "listelement.h"
class ListElement_GUI : public QWidget
{
    Q_OBJECT
public:
    ListElement_GUI(ListElement_GUI&);
    ListElement_GUI(ListElement* source, QWidget *parent);

    void removeButton_Clicked();
    void editButton_Clicked();
    void enableCheckBox_Clicked();

    QString getTitle();
    QString getInfo();
    ListElement getSource();

    void setTitle(QString text);
    void setInfo(QString text);

private:
    //GUI fields
    QHBoxLayout* mainStack;
    QLabel* title;
    QLabel* info;
    QCheckBox* enableCheckBox;
    QPushButton* removeButton;
    QPushButton* editButton;

    //Other
    ListElement* source;
signals:

};

#endif // LISTELEMENT_GUI_H
