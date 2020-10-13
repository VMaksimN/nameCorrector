#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //Init main widgets
    mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    mainGrid = new QGridLayout(mainWidget);
    mainWidget->setLayout(mainGrid);

    //Init directories GUI
    addDirectoryButton = new QPushButton("Add", mainWidget);
    mainGrid->addWidget(addDirectoryButton, 0, 0);

    clearDirList_Button = new QPushButton("Clear", mainWidget);
    mainGrid->addWidget(clearDirList_Button, 0, 1);

    showDirWindow_Button = new QPushButton("Win", mainWidget);
    mainGrid->addWidget(showDirWindow_Button, 0, 2);

    dirBox = new QWidget(mainWidget);
    mainGrid->addWidget(dirBox, 1, 0, 1, 3, Qt::AlignTop);

    dirLayout = new QVBoxLayout(mainWidget);
    dirLayout->addWidget(new QLabel("Directories"));
    dirBox->setLayout(dirLayout);


    //Init rules GUI
    addRuleButton = new QPushButton("Add", mainWidget);
    mainGrid->addWidget(addRuleButton, 2, 0);

    clearRuleList_Button = new QPushButton("Clear", mainWidget);
    mainGrid->addWidget(clearRuleList_Button, 2, 1);

    showRuleWindow_Button = new QPushButton("Win", mainWidget);
    mainGrid->addWidget(showRuleWindow_Button, 2, 2);

    ruleBox = new QWidget(mainWidget);
    mainGrid->addWidget(ruleBox, 3, 0, 1, 3, Qt::AlignTop);

    ruleLayout = new QVBoxLayout(mainWidget);
    ruleLayout->addWidget(new QLabel("Rules"));
    ruleBox->setLayout(ruleLayout);

    //Init Log GUI
    logBlock = new QTextEdit(mainWidget);
    mainGrid->addWidget(logBlock, 0, 3, 3, 3);
    logOut("Reset and full reset", LogStatus::Reset);
    logOut("Something went wrong", LogStatus::Failed);
    logOut("Success", LogStatus::Success);
    logOut("Some information", LogStatus::Info);

    clearLogButton = new QPushButton("Clear log", mainWidget);
    mainGrid->addWidget(clearLogButton, 3, 3, 1, 3);

    //Init Exceptions GUI
    correctFiles_CheckBox = new QCheckBox("Correct files", mainWidget);
    mainGrid->addWidget(correctFiles_CheckBox, 4, 3);

    correctDirs_CheckBox = new QCheckBox("Correct directories", mainWidget);
    mainGrid->addWidget(correctDirs_CheckBox, 4, 4);

    //Init correct, reset and full reset
    resetButton = new QPushButton("Reset", mainWidget);
    mainGrid->addWidget(resetButton, 5, 3);

    fullResetButton = new QPushButton("Full reset", mainWidget);
    mainGrid->addWidget(fullResetButton, 5, 4);

    correctButton = new QPushButton("Correct names", mainWidget);
    mainGrid->addWidget(correctButton, 5, 5);
}

MainWindow::~MainWindow(){}


void MainWindow::logOut(QString log, LogStatus st)
{
    //Add a html tag to the log text according the log type
    if(st == LogStatus::Info)
    {
        log.prepend("<p style=\"color:cyan\">");
    }
    else if(st == LogStatus::Success)
    {
        log.prepend("<p style=\"color:green;\">");
    }
    else if(st == LogStatus::Failed)
    {
        log.prepend("<p style=\"color: red; font-weight: bold;\">");
    }
    else if(st == LogStatus::Reset)
    {
        log.prepend("<p style=\"color: purple; font-weight: bold;\">");
    }
    //Log it out
    log.push_back("</p>");
    logBlock->append(log);
}
