#ifndef LISTELEMENT_GUI_H
#define LISTELEMENT_GUI_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "listelement.h"
class ListElement_GUI : public QWidget
{
    Q_OBJECT
public:
    explicit ListElement_GUI(QWidget *parent = nullptr);

private:
    QHBoxLayout* mainStack;
    QLabel* title;
    QPushButton* removeButton;
    QPushButton* acceptButton;

signals:

};

#endif // LISTELEMENT_GUI_H
