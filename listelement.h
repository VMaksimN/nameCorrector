#ifndef LISTELEMENT_H
#define LISTELEMENT_H

#include<QObject>

class ListElement : QObject
{
    Q_OBJECT
public:
    explicit ListElement();

    int getId();
    QString getName();
    bool isEnabled();
    QString getData();
    bool isEditable();

    void setName(QString name);
    void setEnabled(bool val);
    void setData(QString data);
    void setEditable(bool val);

private:
    int id;
    QString name;
    bool is_enabled;
    QString data;
    bool is_editable;

signals:

};

#endif // LISTELEMENT_H
