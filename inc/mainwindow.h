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
#include <QToolTip>
#include <QMessageBox>
#include <QWidget>
#include <QGridLayout>
#include <QSizePolicy>
#include <QComboBox>
#include <QTextEdit>
#include <QLabel>
#include <QPair>
#include <QTextBlock>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QMenuBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QQueue>
#include <QScrollArea>


#include "listwindow.h"
#include "connectablelist.h"


enum LogStatus
{
    Info,
    Success,
    Failed,
    Reset
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    //Signals handlers
    void addDirButton_Clicked();
    void addRuleButton_Clicked();
    void clearDirsButton_Clicked();
    void clearRulesButton_Clicked();
    void correctButton_Clicked();
    void correctCheckBox_StateChanged();
    void dirList_ElementAdded(int i);
    void element_SelectedStateChanged(bool state);
    void newElWin_DirCreated();
    void newElWin_RuleCreated();
    void ruleDir_ElementRemoved();
    void ruleList_ElementAdded(int i);
    void winButton_Clicked();



    //Other methods
    QString addStringTo(QString old, QStringList args);
    void clearLog();
    QString correctName(QString old_name);
    void logOut(QString log, LogStatus st);
    QString makeList(QString old, QStringList args);
    QString remove(QString old, QStringList args);
    QString removeFromTo(QString old, QStringList args);
    void renameDirs(QString path, QStringList* old_names, QStringList* new_names);
    void renameFiles(QString path, QStringList* old_names, QStringList* new_names);
    QString replace(QString old, QStringList args);
    void reset();




    //GUI fields
    QWidget* mainWidget;
    QGridLayout* mainGrid;

    QMenu* fileMenu;
    QAction* saveRulesAction;
    QAction* loadRulesAction;
    QAction* quitAction;

    QMenu* aboutMenu;
    QAction* aboutAction;

    QPushButton* addDirButton;
    QPushButton* clearDirsButton;
    QPushButton* dirWindowButton;
    QWidget* dirBox;
    QVBoxLayout* dirLayout;
    QScrollArea* dirScrollArea;

    QPushButton* addRuleButton;
    QPushButton* clearRulesButton;
    QPushButton* ruleWindowButton;
    QWidget* ruleBox;
    QVBoxLayout* ruleLayout;
    QScrollArea* ruleScrollArea;

    QTextEdit* logBlock;
    QPushButton* clearLogButton;

    QCheckBox* correctFiles_CheckBox;
    QCheckBox* correctDirs_CheckBox;
    QPushButton* resetButton;
    QPushButton* fullResetButton;
    QPushButton* correctButton;



    QComboBox* ruleComboBox = nullptr;
    QTextEdit* replacedTextBox;
    QTextEdit* replaceWithTextBox;
    QTextEdit* removeTextBox;
    QTextEdit* fromTextBox;
    QTextEdit* toTextBox;
    QTextEdit* addTextBox;
    QComboBox* positionComboBox;
    QComboBox* typeComboBox;


    //Other fields
    ConnectableList ruleList;
    QList<ListElement_GUI*>* selectedRules;

    QList<QPair<QString, QStringList>*> oldFileNames;
    QList<QPair<QString, QStringList>*> oldDirNames;
    QList<QPair<QString, QStringList>*> newFileNames;
    QList<QPair<QString, QStringList>*> newDirNames;

    ConnectableList dirList;
    QList<ListElement_GUI*>* selectedDirs;

    QString currentPath;
    char last_char = 'A';
    int last_num = 0;
    char* danderous_symbols;
};
#endif // MAINWINDOW_H
