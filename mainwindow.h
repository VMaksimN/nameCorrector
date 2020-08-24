#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QPushButton>
#include <QApplication>
#include <QFileInfo>
#include <QString>
#include <QMessageBox>
#include <QWidget>
#include <QGridLayout>
#include <QSizePolicy>
#include <QComboBox>
#include <QTextEdit>
#include <QLabel>
#include <QPair>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //GUI methods
    void fixButtonClicked();
    void addRuleButtonClicked();
    void removeRuleButtonClicked();
    void applyButtonClicked();
    void ruleComboBoxTextChanged(const QString& text);
    void createApplyButton(QGridLayout* layout, int row, int column);
    void createRemoveButton(QGridLayout* layout, int row, int column);

    //Text methods
    QString fix_name(QString old_name);
    QString replace(QString old, QString* args);
    QString remove(QString old, QString* args);
    void reset();

    //GUI fields
    QWidget* mainWidget;
    QGridLayout* mainGrid;

    QPushButton* fixButton;
    QPushButton* addRuleButton;
    QPushButton* resetButton;

    QComboBox* ruleComboBox = nullptr;
    QTextEdit* replacedTextBox;
    QTextEdit* replaceWithTextBox;
    QTextEdit* removeTextBox;
    QList<QList<QWidget*>*>* widgets;

    //Other fields
    int rulesNumber = 0;
    QList<QPair<QString, QString*>*> rules;
    QStringList oldNames;
    QString currentPath;
};
#endif // MAINWINDOW_H
