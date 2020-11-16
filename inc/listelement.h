#ifndef LISTELEMENT_H
#define LISTELEMENT_H

#include<QObject>
#include<QList>
#include<QPair>

class ListElement : QObject
{
    Q_OBJECT
public:
    explicit ListElement();
    ListElement(ListElement&);
    explicit ListElement(QString name, QString info = "", QString type = "rule", bool is_editable = false, bool is_closable = true);

    int getId();
    QString getName();
    bool isEnabled();
    bool isEditable();
    QString getInfo();
    bool isClosable();
    QString getType();

    void setName(QString name);
    void setEnabled(bool val);
    void setEditable(bool val);
    void setInfo(QString info);
    void setClosable(bool val);

private:
    int id;
    QString* name = new QString();
    bool* is_enabled = new bool();
    bool is_editable;
    QString* info = new QString();
    bool is_closable;
    QString type;

signals:

};

#endif // LISTELEMENT_H
