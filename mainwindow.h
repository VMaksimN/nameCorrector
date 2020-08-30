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
    void correctButtonClicked();
    void addRuleButtonClicked();
    void removeRuleButtonClicked();
    void applyButtonClicked();
    void ruleComboBoxTextChanged(const QString& text);
    void createApplyButton(QGridLayout* layout, int row, int column);
    void createRemoveButton(QGridLayout* layout, int row, int column);
    void correctFiles_CheckBox_Clicked();
    void correctFolders_CheckBox_Clicked();

    //Functional methods
    QString correctName(QString old_name);
    QString replace(QString old, QString* args);
    QString remove(QString old, QString* args);
    QString removeFromTo(QString old, QString* args);
    QString addStringTo(QString old, QString* args);
    void renameFiles(QString path, QStringList* old_names, QStringList* new_names);
    void renameDirs(QString path, QStringList* old_names, QStringList* new_names);
    void reset();

    //GUI fields
    QWidget* mainWidget;
    QGridLayout* mainGrid;

    QPushButton* correctButton;
    QPushButton* addRuleButton;
    QCheckBox* correctFiles_CheckBox;
    QCheckBox* correctFolders_CheckBox;
    QPushButton* resetButton;
    QPushButton* fullResetButton;
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

    QList<QPair<QString, QStringList>*> oldFileNames;
    QList<QPair<QString, QStringList>*> oldDirNames;

    QMap<QString, QQueue<QStringList>> oldFileNamesLog;
    QMap<QString, QQueue<QStringList>> oldDirNamesLog;

    QString currentPath;
};
#endif // MAINWINDOW_H
