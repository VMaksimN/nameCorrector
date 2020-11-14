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

    void insert(ListElement* el, int i);
    void remove(int i);
    void remove(ListElement* el);
    void push_back(ListElement* el);
    void pop_back();
    ListElement* last();
    int count();
private:

    QList<ListElement*>* list;

signals:
    void elementWasAdded(int i);
    void elementWasRemoved(int i);
    void listWasCleared();
};

#endif // CONNECTABLELIST_H
