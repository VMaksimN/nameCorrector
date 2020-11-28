#ifndef LISTELEMENT_H
#define LISTELEMENT_H

#include<QObject>
#include<QList>
#include<QSet>
#include<QWidget>
#include<QLayout>
#include<QPair>

class ListElement : public QObject
{
    Q_OBJECT
public:
    explicit ListElement();
    ListElement(ListElement&);
    explicit ListElement(QString name, QString info = "", QString type = "rule", bool is_editable = false, bool is_closable = true);

    //Get-methods
    int getId();
    QString getInfo();
    QString getName();
    QString getType();
    bool isClosable();
    bool isEditable();
    bool isEnabled();

    //Set-methods
    void setClosable(bool val);
    void setEditable(bool val);
    void setEnabled(bool val);
    void setInfo(QString info);
    void setName(QString name);

    //Other
    void addGUI(QWidget* gui);
    void removeGUI();

private:

    QSet<QWidget*> connectedGUI;
    int id;
    static int id_counter;
    QString* info = new QString();
    bool is_closable;
    bool is_editable;
    bool* is_enabled = new bool();
    QString* name = new QString();
    QString type;

signals:
    void deleted(int id);
};

#endif // LISTELEMENT_H
