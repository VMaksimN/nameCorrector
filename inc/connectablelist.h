#ifndef CONNECTABLELIST_H
#define CONNECTABLELIST_H

#include <QObject>
#include <QList>

#include "listelement.h"



class ConnectableList : public QObject
{
    Q_OBJECT
public:
    explicit ConnectableList(QObject *parent = nullptr);

    //Get-methods
    ListElement* at(int i);
    int count();
    ListElement* last();

    //Other
    void clear();
    void insert(ListElement* el, int i);
    void pop_back();
    void push_back(ListElement* el);
    void remove(int i);
    void remove(ListElement* el);
private:

    //Source list
    QList<ListElement*>* list;

signals:
    void elementAdded(int i);
    void elementRemoved();
    void cleared();
};

#endif // CONNECTABLELIST_H
