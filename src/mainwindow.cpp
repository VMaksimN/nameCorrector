#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //INit main widgets
    mainWidget = new QWidget();
    setCentralWidget(mainWidget);

    mainGrid = new QGridLayout(mainWidget);


    //INit directories fields
    addDirButton = new QPushButton("Add", mainWidget);
    connect(addDirButton, &QPushButton::clicked, this, &MainWindow::addDirButton_Clicked);
    mainGrid->addWidget(addDirButton, 0, 0);

    clearDirsButton = new QPushButton("Clear", mainWidget);
    connect(clearDirsButton, &QPushButton::clicked, this, &MainWindow::clearDirsButton_Clicked);
    mainGrid->addWidget(clearDirsButton, 0, 1);

    dirWindowButton = new QPushButton("Win", mainWidget);
    connect(dirWindowButton, &QPushButton::clicked, this, &MainWindow::winButton_Clicked);
    mainGrid->addWidget(dirWindowButton, 0, 2);

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

    connect(&dirList, &ConnectableList::elementAdded, this, &MainWindow::dirList_ElementAdded);
    //connect(&dirList, &ConnectableList::elementRemoved, this, &MainWindow::ruleDirList_ElementRemoved);
    connect(&dirList, &ConnectableList::cleared, this, &MainWindow::clearDirsButton_Clicked);
    selectedDirs = new QList<ListElement_GUI*>();


    //INit rules fields
    addRuleButton = new QPushButton("Add", mainWidget);
    connect(addRuleButton, &QPushButton::clicked, this, &MainWindow::addRuleButton_Clicked);
    mainGrid->addWidget(addRuleButton, 2, 0);

    clearRulesButton = new QPushButton("Clear", mainWidget);
    connect(clearRulesButton, &QPushButton::clicked, this, &MainWindow::clearRulesButton_Clicked);
    mainGrid->addWidget(clearRulesButton, 2, 1);

    ruleWindowButton = new QPushButton("Win", mainWidget);
    connect(ruleWindowButton, &QPushButton::clicked, this, &MainWindow::winButton_Clicked);
    mainGrid->addWidget(ruleWindowButton, 2, 2);

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

    connect(&ruleList, &ConnectableList::elementAdded, this, &MainWindow::ruleList_ElementAdded);
    //connect(&ruleList, &ConnectableList::elementRemoved, this, &MainWindow::ruleDirList_ElementRemoved);
    connect(&ruleList, &ConnectableList::cleared, this, &MainWindow::clearRulesButton_Clicked);
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
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::reset);
    mainGrid->addWidget(resetButton, 5, 3);

    fullResetButton = new QPushButton("Full reset", mainWidget);
    connect(fullResetButton, &QPushButton::clicked, this, &MainWindow::reset);
    mainGrid->addWidget(fullResetButton, 5, 4);

    correctButton = new QPushButton("Correct names", mainWidget);
    connect(correctButton, &QPushButton::clicked, this, &MainWindow::correctButton_Clicked);
    mainGrid->addWidget(correctButton, 5, 5);
}

MainWindow::~MainWindow()
{
    QApplication::allWidgets().clear();
    QApplication::exit();
}



/////Signals handlers

void MainWindow::addDirButton_Clicked()
{
    NewElementWindow* nelw = new NewElementWindow("Dirs");
    connect(nelw, &NewElementWindow::elementCreated, this, &MainWindow::newElWin_DirCreated);
    QApplication::setActiveWindow(nelw);
    nelw->show();
}

void MainWindow::addRuleButton_Clicked()
{
    NewElementWindow* nelw = new NewElementWindow("Rules");
    connect(nelw, &NewElementWindow::elementCreated, this, &MainWindow::newElWin_RuleCreated);
    QApplication::setActiveWindow(nelw);
    nelw->show();
}

void MainWindow::clearDirsButton_Clicked()
{
    /*if(dirList.count())
    {
        dirList.clear();
    }*/
    for(int i = dirLayout->count(); i > -1; i--)
    {
        if(dirBox->children().at(i) != (QLayout*)dirLayout &&
           dirBox->children().at(i) != (QLayout*)dirScrollArea &&
           ((QLabel*)dirBox->children().at(i))->text() != "Directories")
        {
            ((ListElement_GUI*)dirBox->children().at(i))->removeThis();
            //delete dirBox->children().at(i);
        }
    }
    //selectedDirs->clear();
}

void MainWindow::clearRulesButton_Clicked()
{
    /*if(ruleList.count() > 0)
    {
        ruleList.clear();
    }*/
    for(int i = ruleLayout->count(); i > -1; i--)
    {
        if(ruleBox->children().at(i) != (QLayout*)ruleLayout &&
           ruleBox->children().at(i) != (QLayout*)ruleScrollArea &&
           ((QLabel*)ruleBox->children().at(i))->text() != "Rules")
        {
            ((ListElement_GUI*)ruleBox->children().at(i))->removeThis();
            //delete ruleBox->children().at(i);
        }
    }
    //selectedRules->clear();
}

void MainWindow::correctButton_Clicked()
{
    for(int i = 0; i < dirList.count(); i++)
    {
        if(dirList.at(i)->isEnabled())
        {
            currentPath = dirList.at(i)->getInfo();
            QDir directory(currentPath);

            //Load files and directories names
            QStringList file_list = directory.entryList(QDir::Files);
            QStringList dir_list = directory.entryList(QDir::Dirs);

            //Correct files names
            if(correctFiles_CheckBox->checkState() == Qt::Checked)
            {
                oldFileNames.push_back(new QPair<QString, QStringList>(currentPath, file_list));
                QStringList corrected_names;
                for(int i = 0; i < file_list.count(); i++)
                {
                    corrected_names.push_back(correctName(file_list.at(i)));
                }
                renameFiles(currentPath, &file_list, &corrected_names);
                newFileNames.push_back(new QPair<QString, QStringList>(currentPath, corrected_names));
            }
            //Correct directories if the directory-checkbox was checked
            if(correctDirs_CheckBox->checkState() == Qt::Checked)
            {
                oldDirNames.push_back(new QPair<QString, QStringList>(currentPath, dir_list));
                QStringList corrected_names;
                for(int i = 0; i < dir_list.count(); i++)
                {
                    corrected_names.push_back(correctName(dir_list.at(i)));
                }
                renameDirs(currentPath, &dir_list, &corrected_names);
                newDirNames.push_back(new QPair<QString, QStringList>(currentPath, corrected_names));
            }
        }
    }
    resetButton->setEnabled(true);
    fullResetButton->setEnabled(true);
}

void MainWindow::dirList_ElementAdded(int i)
{
    ListElement_GUI* leg = new ListElement_GUI(dirList.at(i), dirBox);
    connect(leg, &ListElement_GUI::selectedStateChanged, this, &MainWindow::element_SelectedStateChanged);
    connect(leg, &ListElement_GUI::deleted, this, &MainWindow::element_Deleted);

    dirLayout->addWidget(leg);
    leg->setParentLayout(dirLayout);
}

void MainWindow::element_Deleted()
{
    /*ListElement_GUI* deleted = (ListElement_GUI*)sender();

    QString t = deleted->getSource().getType();

    if(t == "Rules")
    {
        selectedRules->removeOne(deleted);
        ruleLayout->removeWidget(deleted);
        ruleList.removeById(deleted->getSource().getId());
        return;
    }

    selectedDirs->removeOne(deleted);
    dirLayout->removeWidget(deleted);
    dirList.removeById(deleted->getSource().getId());*/
}

void MainWindow::element_SelectedStateChanged(bool state)
{
    ListElement_GUI* leg = (ListElement_GUI*)sender();
    if(leg->getSource().getType() == "Rules")
    {
        if(state)
        {
            selectedRules->push_back(leg);
            leg->linkCollection(selectedRules);
            return;
        }
        selectedRules->removeOne(leg);
        leg->unlinkCollection(selectedRules);
        return;
    }

    if(state)
    {
        selectedDirs->push_back(leg);
        leg->linkCollection(selectedDirs);
        return;
    }
    selectedDirs->removeOne(leg);
    leg->unlinkCollection(selectedDirs);
    return;
}

void MainWindow::newElWin_DirCreated()
{
    ListElement* a = ((NewElementWindow*)sender())->getResult();
    dirList.push_back(a);
}

void MainWindow::newElWin_RuleCreated()
{
    ListElement* a = ((NewElementWindow*)sender())->getResult();
    ruleList.push_back(a);
}

void MainWindow::ruleList_ElementAdded(int i)
{
    ListElement_GUI* leg = new ListElement_GUI(ruleList.at(i), ruleBox);
    connect(leg, &ListElement_GUI::selectedStateChanged, this, &MainWindow::element_SelectedStateChanged);
    connect(leg, &ListElement_GUI::deleted, this, &MainWindow::element_Deleted);

    ruleLayout->addWidget(leg);
    leg->setParentLayout(ruleLayout);
}

void MainWindow::winButton_Clicked()
{
    ListWindow* lw;
    if(sender() == dirWindowButton)
    {
        lw = new ListWindow(&dirList, "Directories");
        QApplication::setActiveWindow(lw);
        lw->show();
        return;
    }

    lw = new ListWindow(&ruleList, "Rules");
    QApplication::setActiveWindow(lw);
    lw->show();
}


////////////////////////
////////////////////////
////////////////////////
////////////////////////





/////Other methods

QString MainWindow::addStringTo(QString old, QStringList args)
{
    if(args[1].toInt() < 0)
    {
        return old;
    }
    return old.insert(args[1].toInt(), args[0]);
}

void MainWindow::clearLog()
{
    logBlock->clear();
}

QString MainWindow::correctName(QString old_name)
{
    QString rule;
    QStringList options;
    //correct all names according the rule list
    for(int i = 0; i < ruleList.count(); i++)
    {
        if(ruleList.at(i)->isEnabled())
        {
            rule = ruleList.at(i)->getInfo().split(" ")[0];
            options = ruleList.at(i)->getInfo().split(" ");
            options.removeAt(0);

            if(rule == "Replace")
            {
                old_name = replace(old_name, options);
            }
            else if(rule == "Remove")
            {
                old_name = remove(old_name, options);
            }
            else if(rule == "RemoveFromTo")
            {
                old_name = removeFromTo(old_name, options);
            }
            else if(rule == "AddTo")
            {
                old_name = addStringTo(old_name, options);
            }
            else if(rule == "MakeList")
            {
                old_name = makeList(old_name, options);
            }
        }
    }
    return old_name;
}

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

QString MainWindow::makeList(QString old, QStringList args)
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

QString MainWindow::remove(QString old, QStringList args)
{
    return old.replace(args[0], "");
}

QString MainWindow::removeFromTo(QString old, QStringList args)
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

QString MainWindow::replace(QString old, QStringList args)
{
    if(old.length() >= args[0].length())
    {
        return old.replace(args[0], args[1]);
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

















