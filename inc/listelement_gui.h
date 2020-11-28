#ifndef LISTELEMENT_GUI_H
#define LISTELEMENT_GUI_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QList>
#include <QPushButton>


#include "connectablelist.h"


class ListElement_GUI : public QPushButton
{
    Q_OBJECT
public:

    ListElement_GUI(ListElement_GUI&);
    ListElement_GUI(ListElement* source, QWidget *parent);
    ~ListElement_GUI();

    //Get-methods
    QString getInfo();
    QLayout* getParentLayout();
    ListElement getSource();
    QString getTitle();
    bool isEnabled();
    bool isSelected();

    //Set-methods
    void setEnabled(bool value);
    void setInfo(QString text);
    void setParentLayout(QLayout* pl);
    void setSelected(bool value);
    void setTitle(QString text);

    //Other methods
    void linkCollection(QList<ListElement_GUI*>* col);
    void unlinkAllCollections();
    void unlinkCollection(QList<ListElement_GUI*>* col);
    void removeThis();
private:

    //Signals handlers
    void editButton_Clicked();
    void enableCheckBox_StateChanged();
    void this_clicked();

    //GUI fields
    QHBoxLayout* mainStack;
    QLabel* title;
    QLabel* info;
    QCheckBox* enableCheckBox;
    QPushButton* editButton;
    QPushButton* removeButton;

    const QString defaultStyleSheet = "QLabel {color : white}";
    const QString selectedStyleSheet = "QLabel {color : red}";

    //Other fields
    QList<QList<ListElement_GUI*>*> linkedCollections;
    QLayout* parentLayout;
    ListElement* source;
    bool is_selected;

signals:
    void selectedStateChanged(bool);
    void deleted();
};

#endif // LISTELEMENT_GUI_H
