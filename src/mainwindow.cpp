#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //INit main widgets
    mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    mainGrid = new QGridLayout(mainWidget);


    //INit directories fields
    addDirectoryButton = new QPushButton("Add", mainWidget);
    connect(addDirectoryButton, &QPushButton::clicked, this, &MainWindow::addDirectoryButton_Clicked);
    mainGrid->addWidget(addDirectoryButton, 0, 0);

    clearDirList_Button = new QPushButton("Clear", mainWidget);
    connect(clearDirList_Button, &QPushButton::clicked, this, &MainWindow::clearDirListButton_Clicked);
    mainGrid->addWidget(clearDirList_Button, 0, 1);

    showDirWindow_Button = new QPushButton("Win", mainWidget);
    connect(showDirWindow_Button, &QPushButton::clicked, this, &MainWindow::winButtonClicked);
    mainGrid->addWidget(showDirWindow_Button, 0, 2);

    dirBox = new QWidget(mainWidget);

    dirScrollArea = new QScrollArea(mainWidget);
    dirScrollArea->setWidgetResizable(true);
    dirScrollArea->setWidget(dirBox);
    dirScrollArea->setMinimumWidth(400);
    dirScrollArea->setMinimumHeight(250);
    mainGrid->addWidget(dirScrollArea, 1, 0, 1, 3, Qt::AlignTop);

    dirLayout = new QVBoxLayout(dirBox);
    dirLayout->setAlignment(Qt::AlignTop);
    dirLayout->addWidget(new QLabel("Directories"));

    connect(&directoriesList, &ConnectableList::elementWasAdded, this, &MainWindow::addDir_GUI);
    selectedDirs = new QList<ListElement_GUI*>();


    //INit rules fields
    addRuleButton = new QPushButton("Add", mainWidget);
    connect(addRuleButton, &QPushButton::clicked, this, &MainWindow::addRuleButton_Clicked);
    mainGrid->addWidget(addRuleButton, 2, 0);

    clearRuleList_Button = new QPushButton("Clear", mainWidget);
    connect(clearRuleList_Button, &QPushButton::clicked, this, &MainWindow::clearRuleListButton_Clicked);
    mainGrid->addWidget(clearRuleList_Button, 2, 1);

    showRuleWindow_Button = new QPushButton("Win", mainWidget);
    connect(showRuleWindow_Button, &QPushButton::clicked, this, &MainWindow::winButtonClicked);
    mainGrid->addWidget(showRuleWindow_Button, 2, 2);

    ruleBox = new QWidget(mainWidget);

    ruleScrollArea = new QScrollArea(mainWidget);
    ruleScrollArea->setWidgetResizable(true);
    ruleScrollArea->setWidget(ruleBox);
    ruleScrollArea->setMinimumWidth(400);
    ruleScrollArea->setMinimumHeight(250);
    mainGrid->addWidget(ruleScrollArea, 3, 0, 1, 3, Qt::AlignTop);

    ruleLayout = new QVBoxLayout(ruleBox);
    ruleLayout->setAlignment(Qt::AlignTop);
    ruleLayout->addWidget(new QLabel("Rules"));

    connect(&rules, &ConnectableList::elementWasAdded, this, &MainWindow::addRule_GUI);
    selectedRules = new QList<ListElement_GUI*>();


    //INit Log GUI
    logBlock = new QTextEdit(mainWidget);
    mainGrid->addWidget(logBlock, 0, 3, 3, 3);
    logOut("Reset and full reset", LogStatus::Reset);
    logOut("Something went wrong", LogStatus::Failed);
    logOut("Success", LogStatus::Success);
    logOut("Some information", LogStatus::Info);

    clearLogButton = new QPushButton("Clear log", mainWidget);
    connect(clearLogButton, &QPushButton::clicked, this, &MainWindow::clearLog);
    mainGrid->addWidget(clearLogButton, 3, 3, 1, 3, Qt::AlignBottom);



    //INit Exceptions GUI
    correctFiles_CheckBox = new QCheckBox("Correct files", mainWidget);
    mainGrid->addWidget(correctFiles_CheckBox, 4, 3);

    correctDirs_CheckBox = new QCheckBox("Correct directories", mainWidget);
    mainGrid->addWidget(correctDirs_CheckBox, 4, 4);



    //INit correct, reset and full reset
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

void MainWindow::winButtonClicked()
{
    ListWindow* lw;
    if(sender() == showDirWindow_Button)
    {
        lw = new ListWindow(&directoriesList, "Directories");
        QApplication::setActiveWindow(lw);
        lw->show();
        return;
    }

    lw = new ListWindow(&rules, "Rules");
    QApplication::setActiveWindow(lw);
    lw->show();
}

void MainWindow::addDirectoryButton_Clicked()
{
    NewElementWindow* nelw = new NewElementWindow("Dirs");
    connect(nelw, &NewElementWindow::elementWasCreated, this, &MainWindow::addDir);
    QApplication::setActiveWindow(nelw);
    nelw->show();
}

void MainWindow::addRuleButton_Clicked()
{
    NewElementWindow* nelw = new NewElementWindow("Rules");
    connect(nelw, &NewElementWindow::elementWasCreated, this, &MainWindow::addRule);
    QApplication::setActiveWindow(nelw);
    nelw->show();
}

void MainWindow::clearDirListButton_Clicked()
{
    directoriesList.clear();
    for(int i = dirLayout->count(); i > -1; i--)
    {
        if(dirBox->children().at(i) != (QLayout*)dirLayout &&
           dirBox->children().at(i) != (QLayout*)dirScrollArea)
        {
            delete dirBox->children().at(i);
        }
    }
    selectedDirs = new QList<ListElement_GUI*>();

}

void MainWindow::clearRuleListButton_Clicked()
{
    rules.clear();
    for(int i = ruleLayout->count(); i > -1; i--)
    {
        if(ruleBox->children().at(i) != (QLayout*)ruleLayout &&
           ruleBox->children().at(i) != (QLayout*)ruleScrollArea)
        {
            delete ruleBox->children().at(i);
        }
    }
    selectedRules = new QList<ListElement_GUI*>();
}


void MainWindow::addRule()
{
    ListElement* a = ((NewElementWindow*)sender())->getResult();
    rules.push_back(a);
}

void MainWindow::addDir()
{
    ListElement* a = ((NewElementWindow*)sender())->getResult();
    directoriesList.push_back(a);
}

void MainWindow::addRule_GUI(int i)
{
    ListElement_GUI* leg = new ListElement_GUI(rules.at(i), ruleBox);
    connect(leg, &ListElement_GUI::selectedStateChanged, this, &MainWindow::elementSelectedStateChanged);
    ruleLayout->addWidget(leg);
}

void MainWindow::addDir_GUI(int i)
{
    ListElement_GUI* leg = new ListElement_GUI(directoriesList.at(i), dirBox);
    connect(leg, &ListElement_GUI::selectedStateChanged, this, &MainWindow::elementSelectedStateChanged);
    dirLayout->addWidget(leg);
}

void MainWindow::elementSelectedStateChanged(bool state)
{
    if(((ListElement_GUI*)sender())->getSource().getType() == "Rules")
    {
        if(state)
        {
            selectedRules->push_back((ListElement_GUI*)sender());
            return;
        }
        selectedRules->removeOne((ListElement_GUI*)sender());
        return;
    }

    if(state)
    {
        selectedDirs->push_back((ListElement_GUI*)sender());
        return;
    }
    selectedDirs->removeOne((ListElement_GUI*)sender());
    return;
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
















