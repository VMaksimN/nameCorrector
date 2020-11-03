#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //Init main widgets
    mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    mainGrid = new QGridLayout(mainWidget);

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

    area = new QScrollArea(mainWidget);
    area->setWidgetResizable(true);
    area->setWidget(ruleBox);
    area->setMinimumWidth(400);
    mainGrid->addWidget(area, 3, 0, 1, 3, Qt::AlignTop);


    ruleLayout = new QVBoxLayout(ruleBox);
    ruleLayout->addWidget(new QLabel("Rules"));

    for(int i = 0; i < 10; i++)
    {
        ListElement* el = new ListElement("QW", "qw", "rule", true);
        ListElement_GUI* elg = new ListElement_GUI(el, ruleBox);
        ruleLayout->addWidget(elg);
    }

    //Init Log GUI
    logBlock = new QTextEdit(mainWidget);
    mainGrid->addWidget(logBlock, 0, 3, 3, 3);
    logOut("Reset and full reset", LogStatus::Reset);
    logOut("Something went wrong", LogStatus::Failed);
    logOut("Success", LogStatus::Success);
    logOut("Some information", LogStatus::Info);

    clearLogButton = new QPushButton("Clear log", mainWidget);
    connect(clearLogButton, &QPushButton::clicked, this, &MainWindow::clearLog);
    mainGrid->addWidget(clearLogButton, 3, 3, 1, 3, Qt::AlignBottom);

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


//////////           ////////
//////////           ////////
//////////GUI Methods////////
//////////           ////////


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


void MainWindow::clearLog()
{
    //No comments
    logBlock->clear();
}
////////////////////////
////////////////////////
////////////////////////
////////////////////////













//////////           ///////////////
//////////           ///////////////
//////////Functional Methods////////
//////////           ///////////////


QString MainWindow::correctName(QString old_name)
{
    //correct all names according the rule list
    for(int i = 0; i < rules.count(); i++)
    {
        /*if(rules[i]->first == "Replace")
        {
            old_name = replace(old_name, rules[i]->second);
        }
        else if(rules[i]->first == "Remove")
        {
            old_name = remove(old_name, rules[i]->second);
        }
        else if(rules[i]->first == "RemoveFromTo")
        {
            old_name = removeFromTo(old_name, rules[i]->second);
        }
        else if(rules[i]->first == "AddTo")
        {
            old_name = addStringTo(old_name, rules[i]->second);
        }
        else if(rules[i]->first == "MakeList")
        {
            old_name = makeList(old_name, rules[i]->second);
        }*/
    }
    return old_name;
}

////***/////
///SEE README.md file for more INFORMATION
QString MainWindow::replace(QString old, QString* args)
{
    if(old.length() >= args[0].length())
    {
        return old.replace(args[0], args[1]);
    }
    return old;
}

QString MainWindow::remove(QString old, QString* args)
{
    return old.replace(args[0], "");
}

QString MainWindow::removeFromTo(QString old, QString *args)
{
    int first = args[1].toInt();
    int second = args[2].toInt();
    if(first < 0 || second < 0)
    {
        return old;
    }

    if(args[0] != "")
    {
        QString modified = old.mid(first, second);
        QString new_str = old.mid(first, second).replace(args[0], "");
        old = old.replace(modified, new_str);
    }
    else
    {
        old = old.remove(first, second - first + 1);
    }
    return old;
}

QString MainWindow::addStringTo(QString old, QString* args)
{
    if(args[1].toInt() < 0)
    {
        return old;
    }
    return old.insert(args[1].toInt(), args[0]);
}

QString MainWindow::makeList(QString old, QString* args)
{
    if(args[0] == "numeric")
    {
        if(args[1] == "prefix")
        {
            old = old.prepend(QString::fromStdString(std::to_string(last_num)) + args[2]);
        }
        else
        {
            old = old.append(args[2] + QString::fromStdString(std::to_string(last_num)));
        }
        last_num++;
    }
    else
    {
        if(args[1] == "prefix")
        {
            old = old.prepend(last_char + args[2]);
        }
        else
        {
            old = old.append(args[2] + last_char);
        }
        if(last_char < 'Z')
        {
            last_char++;
        }
    }
    return old;
}

void MainWindow::reset()
{
    /*QMessageBox box("Warning", "", QMessageBox::NoIcon,
                    QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel, this);
    box.setText("Please, do not add new files and directories or remove existing ones before reset. "
                " It may cause errors and damage your files. "
                " Do you want to continue?");
    box.exec();

    if(box.result() == QMessageBox::No || box.result() == QMessageBox::Cancel)
    {
        logOut("Reset aborted", LogStatus::Info);
        return;
    }*/

    //This operation is similar to the correct one
    QPushButton* but = (QPushButton*)sender();

    //Reset all the operations
    if(but == fullResetButton)
    {
        logOut("FULL RESET", LogStatus::Reset);
        while(!oldFileNames.empty())
        {
            QDir directory(oldFileNames.last()->first);
            QStringList file_list = directory.entryList(QDir::Files);

            int index = -1;
            QString path = oldFileNames.last()->first;
            for(int i = oldFileNames.count() - 1; i > -1; i--)
            {
                if(oldFileNames[i]->first != path)
                {
                    index = i;
                    break;
                }
            }
            renameFiles(path, &file_list, &(oldFileNames[index + 1]->second));

            for(int i = oldFileNames.count() - 1; i > index; i--)
            {
                oldFileNames.removeAt(i);
            }
        }
        if(!oldDirNames.empty())
        {
            QDir directory(oldDirNames.last()->first);
            QStringList dir_list = directory.entryList(QDir::Dirs);

            int index = -1;
            QString path = oldDirNames.last()->first;
            for(int i = oldDirNames.count() - 1; i > -1; i--)
            {
                if(oldDirNames[i]->first != path)
                {
                    index = i;
                    break;
                }
            }
            renameDirs(path, &dir_list, &(oldDirNames[index + 1]->second));

            for(int i = oldDirNames.count() - 1; i > index; i--)
            {
                oldDirNames.removeAt(i);
            }
        }
        fullResetButton->setEnabled(false);
        resetButton->setEnabled(false);
        logOut("///////////", LogStatus::Reset);
        logOut("///////////", LogStatus::Reset);
        logOut(" ", LogStatus::Reset);
        return;
    }

    logOut("Reset", LogStatus::Reset);

    if(!oldFileNames.isEmpty())
    {
        renameFiles(oldFileNames.last()->first, &newFileNames.last()->second, &(oldFileNames.last()->second));
        oldFileNames.removeLast();
        newFileNames.removeLast();
    }
    if(!oldDirNames.isEmpty())
    {
        renameDirs(oldDirNames.last()->first, &newDirNames.last()->second, &(oldDirNames.last()->second));
        oldDirNames.removeLast();
        newDirNames.removeLast();
    }

    if(oldDirNames.empty() && oldFileNames.empty())
    {
        fullResetButton->setEnabled(false);
        resetButton->setEnabled(false);
    }
    logOut("///////////", LogStatus::Reset);
    logOut("///////////", LogStatus::Reset);
    logOut(" ", LogStatus::Reset);
}
////***/////



void MainWindow::renameFiles(QString path, QStringList* old_names, QStringList* new_names)
{
    //Try to rename the file and log out the result
    for(int i = 0; i < old_names->count(); i++)
    {
        if(QFile::rename(path + "/" + (*old_names)[i],
                         path + "/" + (*new_names)[i]))
        {
            logOut("FILE " + old_names->at(i) +
                   " has been renamed the " + new_names->at(i), LogStatus::Success);
            continue;
        }
        logOut("FILE " + old_names->at(i) +
               " has not been renamed the " + new_names->at(i), LogStatus::Failed);
    }
}

void MainWindow::renameDirs(QString path, QStringList* old_names, QStringList* new_names)
{
    //Try to rename the directory and log out the result
    for(int i = 0; i < old_names->count(); i++)
    {
        QDir dir(path + "/" + (*old_names)[i]);
        if(dir.rename(path + "/" + (*old_names)[i],
                      path + "/" + (*new_names)[i]))
        {
            logOut("DIR " + old_names->at(i) +
                   " has been renamed the " + new_names->at(i), LogStatus::Success);
            continue;
        }
        logOut("DIR " + old_names->at(i) +
               " has not been renamed the " + new_names->at(i), LogStatus::Failed);
    }
}
