#ifndef LISTELEMENT_H
#define LISTELEMENT_H

#include<QObject>
#include<QList>
#include<QSet>
#include<QWidget>
#include<QLayout>
#include<QPair>

class ListElement : QObject
{
    Q_OBJECT
public:
    explicit ListElement();
    ListElement(ListElement&);
    explicit ListElement(QString name, QString info = "", QString type = "rule", bool is_editable = false, bool is_closable = true);

    bool operator==(ListElement a);

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

    void addGUI(QWidget* gui);
    void removeGUI();

private:
    int id;
    QString* name = new QString();
    bool* is_enabled = new bool();
    bool is_editable;
    QString* info = new QString();
    bool is_closable;
    QString type;

    static int id_counter;
    QSet<QWidget*> connectedGUI;

signals:

};

#endif // LISTELEMENT_H
