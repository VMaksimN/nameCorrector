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
#include "listwindow.h"
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

    void logOut(QString log, LogStatus st);
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

    QPushButton* addRuleButton;
    QPushButton* clearRuleList_Button;
    QPushButton* showRuleWindow_Button;
    QWidget* ruleBox;
    QVBoxLayout* ruleLayout;

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
};
#endif // MAINWINDOW_H
