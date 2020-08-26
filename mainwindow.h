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
#include <QCheckBox>
#include <QQueue>

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
    void fixFiles_CheckBox_Clicked();
    void fixFolders_CheckBox_Clicked();

    //Text methods
    QString fixName(QString old_name);
    QString replace(QString old, QString* args);
    QString remove(QString old, QString* args);
    QString removeFromTo(QString old, QString* args);
    QString addStringTo(QString old, QString* args);
    void reset();

    //GUI fields
    QWidget* mainWidget;
    QGridLayout* mainGrid;

    QPushButton* fixButton;
    QPushButton* addRuleButton;
    QPushButton* resetButton;
    QCheckBox* fixFiles_CheckBox;
    QCheckBox* fixFolders_CheckBox;
    QLabel* logLabel;

    QComboBox* ruleComboBox = nullptr;
    QTextEdit* replacedTextBox;
    QTextEdit* replaceWithTextBox;
    QTextEdit* removeTextBox;
    QTextEdit* fromTextBox;
    QTextEdit* toTextBox;
    QTextEdit* addTextBox;
    QList<QList<QWidget*>*>* widgets;

    //Other fields
    int rulesNumber = 0;
    QList<QPair<QString, QString*>*> rules;
    QQueue<QStringList> oldFileNames;
    QQueue<QStringList> oldDirNames;
    QString currentPath;
};
#endif // MAINWINDOW_H
