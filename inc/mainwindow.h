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

    //GUI methods
    void logOut(QString log, LogStatus st);
    void clearLog();
    void winButtonClicked();
    void addDirectoryButton_Clicked();
    void addRuleButton_Clicked();
    void clearDirListButton_Clicked();
    void clearRuleListButton_Clicked();


    void addRule();
    void addDir();
    void addRule_GUI(int i);
    void addDir_GUI(int i);
    void elementSelectedStateChanged(bool state);
    void elementDeleted();
    void removeGUIElement();

    //Functional methods
    QString correctName(QString old_name);
    QString replace(QString old, QString* args);
    QString remove(QString old, QString* args);
    QString removeFromTo(QString old, QString* args);
    QString addStringTo(QString old, QString* args);
    QString makeList(QString old, QString* args);
    void renameFiles(QString path, QStringList* old_names, QStringList* new_names);
    void renameDirs(QString path, QStringList* old_names, QStringList* new_names);
    void reset();

private:

    //GUI fields
    QWidget* mainWidget;
    QGridLayout* mainGrid;

    QMenu* fileMenu;
    QAction* saveRuleListAction;
    QAction* loadRuleListAction;
    QAction* quitAction;

    QMenu* aboutMenu;
    QAction* aboutAction;

    QPushButton* addDirectoryButton;
    QPushButton* clearDirList_Button;
    QPushButton* showDirWindow_Button;
    QWidget* dirBox;
    QVBoxLayout* dirLayout;
    QScrollArea* dirScrollArea;

    QPushButton* addRuleButton;
    QPushButton* clearRuleList_Button;
    QPushButton* showRuleWindow_Button;
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
    int rulesNumber = 0;
    ConnectableList rules;
    QList<ListElement_GUI*>* selectedRules;

    QList<QPair<QString, QStringList>*> oldFileNames;
    QList<QPair<QString, QStringList>*> oldDirNames;

    QList<QPair<QString, QStringList>*> newFileNames;
    QList<QPair<QString, QStringList>*> newDirNames;

    ConnectableList directoriesList;
    QList<ListElement_GUI*>* selectedDirs;

    ListElement_GUI* deletedElement;

    QString currentPath;
    char last_char = 'A';
    int last_num = 0;
    char* danderous_symbols;
};
#endif // MAINWINDOW_H
