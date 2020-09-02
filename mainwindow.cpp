#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //Init widgets storage
    widgets = new QList<QList<QWidget*>*>();
    widgets->push_back(new QList<QWidget*>());
    widgets->push_back(new QList<QWidget*>());
    widgets->push_back(new QList<QWidget*>());

    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    mainGrid = new QGridLayout(mainWidget);
    mainWidget->setLayout(mainGrid);

    //Init button adding new rules
    addRuleButton = new QPushButton("New rule", mainWidget);
    connect(addRuleButton, &QPushButton::clicked, this, &MainWindow::addRuleButtonClicked);
    mainGrid->addWidget(addRuleButton, 0, 0);
    widgets->first()->push_back(addRuleButton);

    //Init button starting correct operations
    correctButton = new QPushButton("Correct names", mainWidget);
    correctButton->setEnabled(false);
    connect(correctButton, &QPushButton::clicked, this, &MainWindow::correctButtonClicked);
    mainGrid->addWidget(correctButton, 0, 1);
    widgets->first()->push_back(correctButton);

    //Init Checkboxes including(excluding) files or directories
    correctFiles_CheckBox = new QCheckBox("Correct files", mainWidget);
    correctFiles_CheckBox->setCheckState(Qt::Checked);
    connect(correctFiles_CheckBox, &QCheckBox::clicked, this, &MainWindow::correctFiles_CheckBox_Clicked);
    mainGrid->addWidget(correctFiles_CheckBox, 1, 0);
    widgets->at(1)->push_back(correctFiles_CheckBox);

    correctFolders_CheckBox = new QCheckBox("Correct folders", mainWidget);
    correctFolders_CheckBox->setCheckState(Qt::Unchecked);
    connect(correctFolders_CheckBox, &QCheckBox::clicked, this, &MainWindow::correctFolders_CheckBox_Clicked);
    mainGrid->addWidget(correctFolders_CheckBox, 1, 1);
    widgets->at(1)->push_back(correctFolders_CheckBox);

    //Init buttons replacing new names with old ones
    resetButton = new QPushButton("Reset", mainWidget);
    resetButton->setEnabled(false);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::reset);
    mainGrid->addWidget(resetButton, 2, 0);
    widgets->at(2)->push_back(resetButton);

    fullResetButton = new QPushButton("Full reset", mainWidget);
    fullResetButton->setEnabled(false);
    connect(fullResetButton, &QPushButton::clicked, this, &MainWindow::reset);
    mainGrid->addWidget(fullResetButton, 2, 1);
    widgets->at(2)->push_back(fullResetButton);

    //Init log gui
    logBlock = new QTextEdit(mainWidget);
    logBlock->setHtml("<p>Log:</p>");
    logBlock->setMinimumSize(150, 50);
    mainGrid->addWidget(logBlock, 0, 8, 7, 10);
    widgets->first()->push_back(logBlock);

    logOut("Reset and full reset", LogStatus::Reset);
    logOut("Something went wrong", LogStatus::Failed);
    logOut("Success", LogStatus::Success);
    logOut("Some information", LogStatus::Info);

    clearLogButton = new QPushButton(mainWidget);
    clearLogButton->setText("Clear log");
    mainGrid->addWidget(clearLogButton, 0, 2);
    widgets->first()->push_back(clearLogButton);
    connect(clearLogButton, &QPushButton::clicked, this, &MainWindow::clearLog);
}

MainWindow::~MainWindow(){}

///////////////////
///GUI METHODS
///////////////////
void MainWindow::addRuleButtonClicked()
{
    //Add a new line and index it
    rulesNumber++;
    widgets->push_back(new QList<QWidget*>());
    addRuleButton->setEnabled(false);

    //Fill the new line with blank rule (comboBox and two buttons)
    ruleComboBox = new QComboBox(mainWidget);
    ruleComboBox->addItem("None");
    ruleComboBox->addItem("Replace");
    ruleComboBox->addItem("Remove");
    ruleComboBox->addItem("RemoveFromTo");
    ruleComboBox->addItem("AddTo");
    connect(ruleComboBox, &QComboBox::currentTextChanged, this, &MainWindow::ruleComboBoxTextChanged);
    mainGrid->addWidget(ruleComboBox, rulesNumber  + reserved_rows - 1, 0);
    widgets->last()->push_back(ruleComboBox);

    createRemoveButton(mainGrid, rulesNumber  + reserved_rows - 1, 1, "0");
    createApplyButton(mainGrid, rulesNumber  + reserved_rows - 1, 2);

    widgets->last()->last()->setEnabled(false);
}

void MainWindow::correctButtonClicked()
{
    //User open a directory and the program start correct operations and remember old names
    QFileDialog dialog(this);
    currentPath = dialog.getExistingDirectory();
    QDir directory(currentPath);

    QStringList file_list = directory.entryList(QDir::Files);
    QStringList dir_list = directory.entryList(QDir::Dirs);
    oldFileNames.push_back(new QPair<QString, QStringList>(currentPath, file_list));
    oldDirNames.push_back(new QPair<QString, QStringList>(currentPath, dir_list));

    if(correctFiles_CheckBox->checkState() == Qt::Checked)
    {
        QStringList corrected_names;
        for(int i = 0; i < file_list.count(); i++)
        {
            corrected_names.push_back(correctName(file_list.at(i)));
        }
        renameFiles(currentPath, &file_list, &corrected_names);
    }
    if(correctFolders_CheckBox->checkState() == Qt::Checked)
    {
        QStringList corrected_names;
        for(int i = 0; i < dir_list.count(); i++)
        {
            corrected_names.push_back(correctName(dir_list.at(i)));
        }
        renameDirs(currentPath, &dir_list, &corrected_names);
    }
    resetButton->setEnabled(true);
    fullResetButton->setEnabled(true);
}

void MainWindow::removeRuleButtonClicked()
{
    //Search for sender and remove all widgets in the selected line
    for(int i = widgets->count() - 1; i > -1; i--)
    {
        if(sender() == widgets->at(i)->last() ||
                sender() == widgets->at(i)->at(widgets->at(i)->count() - 2))
        {
            for(int j = widgets->at(i)->count() - 1; j > -1; j--)
            {
                delete widgets->at(i)->at(j);
            }
            widgets->removeAt(i);
            rules.removeAt(i - reserved_rows);
            break;
        }
    }
    addRuleButton->setEnabled(true);
    rulesNumber--;
}

void MainWindow::applyButtonClicked()
{
    //Replace current widgets with new inactive ones according selected rule
    addRuleButton->setEnabled(true);

    QLabel* ruleLabel = new QLabel(mainWidget);
    ruleLabel->setText(QString::fromStdString(std::to_string(rulesNumber))
                       + ". " + ruleComboBox->currentText());
    mainGrid->addWidget(ruleLabel, rulesNumber  + reserved_rows - 1, 0);

    if(ruleComboBox->currentText() == "Replace")
    {
        QLabel* replacedLabel = new QLabel(mainWidget);
        replacedLabel->setText(replacedTextBox->toPlainText());
        mainGrid->addWidget(replacedLabel, rulesNumber  + reserved_rows - 1, 1);

        QLabel* withLabel = new QLabel("with", mainWidget);
        mainGrid->addWidget(withLabel, rulesNumber  + reserved_rows - 1, 2);

        QLabel* replaceWithLabel = new QLabel(mainWidget);
        replaceWithLabel->setText(replaceWithTextBox->toPlainText());
        mainGrid->addWidget(replaceWithLabel, rulesNumber  + reserved_rows - 1, 3);

        QString* args = new QString[2];
        args[0] = replacedTextBox->toPlainText();
        args[1] = replaceWithTextBox->toPlainText();
        rules.push_back(new QPair<QString, QString*>("Replace", args));

        for(int i = 0; i < widgets->last()->count(); i++)
        {
            delete widgets->last()->at(i);
        }
        widgets->removeLast();

        widgets->push_back(new QList<QWidget*>());
        widgets->last()->push_back(ruleLabel);
        widgets->last()->push_back(replacedLabel);
        widgets->last()->push_back(withLabel);
        widgets->last()->push_back(replaceWithLabel);
        createRemoveButton(mainGrid, rulesNumber  + reserved_rows - 1, 4, "1");
    }
    else if(ruleComboBox->currentText() == "Remove")
    {
        QLabel* removeLabel = new QLabel(mainWidget);
        removeLabel->setText(removeTextBox->toPlainText());
        mainGrid->addWidget(removeLabel, rulesNumber  + reserved_rows - 1, 1);

        QString* args = new QString[1];
        args[0] = removeTextBox->toPlainText();

        rules.push_back(new QPair<QString, QString*>("Remove", args));

        for(int i = 0; i < widgets->last()->count(); i++)
        {
            delete widgets->last()->at(i);
        }
        widgets->removeLast();

        widgets->push_back(new QList<QWidget*>());
        widgets->last()->push_back(ruleLabel);
        widgets->last()->push_back(removeLabel);
        createRemoveButton(mainGrid, rulesNumber  + reserved_rows - 1, 2, "1");
    }
    else if(ruleComboBox->currentText() == "RemoveFromTo")
    {
        ruleLabel->setText(QString::fromStdString(std::to_string(rulesNumber))
                           + ". " + "Remove");

        QLabel* removeLabel = new QLabel(mainWidget);
        removeLabel->setText(removeTextBox->toPlainText());
        if(removeLabel->text() == "")
        {
            removeLabel->setText("All");
        }
        mainGrid->addWidget(removeLabel, rulesNumber  + reserved_rows - 1, 1);

        QLabel* from = new QLabel("from", mainWidget);
        mainGrid->addWidget(from, rulesNumber  + reserved_rows - 1, 2);

        QLabel* fromLabel = new QLabel(mainWidget);
        fromLabel->setText(fromTextBox->toPlainText());
        mainGrid->addWidget(fromLabel, rulesNumber  + reserved_rows - 1, 3);

        QLabel* to = new QLabel("to", mainWidget);
        mainGrid->addWidget(to, rulesNumber  + reserved_rows - 1, 4);

        QLabel* toLabel = new QLabel(mainWidget);
        toLabel->setText(toTextBox->toPlainText());
        mainGrid->addWidget(toLabel, rulesNumber  + reserved_rows - 1, 5);

        QString* args = new QString[3];
        args[0] = removeTextBox->toPlainText();
        args[1] = fromTextBox->toPlainText();
        args[2] = toTextBox->toPlainText();

        rules.push_back(new QPair<QString, QString*>("RemoveFromTo", args));

        for(int i = 0; i < widgets->last()->count(); i++)
        {
            delete widgets->last()->at(i);
        }
        widgets->removeLast();

        widgets->push_back(new QList<QWidget*>());
        widgets->last()->push_back(ruleLabel);
        widgets->last()->push_back(removeLabel);
        widgets->last()->push_back(from);
        widgets->last()->push_back(fromLabel);
        widgets->last()->push_back(to);
        widgets->last()->push_back(toLabel);
        createRemoveButton(mainGrid, rulesNumber  + reserved_rows - 1, 6,"1");
    }
    else if(ruleComboBox->currentText() == "AddTo")
    {
        QLabel* addLabel = new QLabel(mainWidget);
        addLabel->setText(addTextBox->toPlainText());
        mainGrid->addWidget(addLabel, rulesNumber  + reserved_rows - 1, 1);

        QLabel* to = new QLabel("to", mainWidget);
        mainGrid->addWidget(to, rulesNumber  + reserved_rows - 1, 2);

        QLabel* toLabel = new QLabel(mainWidget);
        toLabel->setText(toTextBox->toPlainText());
        mainGrid->addWidget(toLabel, rulesNumber  + reserved_rows - 1, 3);

        QString* args = new QString[2];
        args[0] = addTextBox->toPlainText();
        args[1] = toTextBox->toPlainText();

        rules.push_back(new QPair<QString, QString*>("AddTo", args));

        for(int i = 0; i < widgets->last()->count(); i++)
        {
            delete widgets->last()->at(i);
        }
        widgets->removeLast();

        widgets->push_back(new QList<QWidget*>());
        widgets->last()->push_back(ruleLabel);
        widgets->last()->push_back(addLabel);
        widgets->last()->push_back(to);
        widgets->last()->push_back(toLabel);
        createRemoveButton(mainGrid, rulesNumber  + reserved_rows - 1, 4, "1");
    }
}

void MainWindow::ruleComboBoxTextChanged(const QString& text)
{
    correctButton->setEnabled(false);

    //Replace current widgets with new ones according selected rule
    for(int i = 0; i < widgets->last()->count(); i++)
    {
        if(widgets->last()->at(i) != ruleComboBox)
        {
           QWidget* old = widgets->last()->at(i);
           widgets->last()->removeAt(i);
           delete old;
           i = 0;
        }
    }
    if(text == "Replace")
    {
        replacedTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(replacedTextBox, rulesNumber  + reserved_rows - 1, 1);
        widgets->last()->push_back(replacedTextBox);
        connect(replacedTextBox, &QTextEdit::textChanged, this, &MainWindow::checkTextBox);

        QLabel* withLabel = new QLabel("with", mainWidget);
        mainGrid->addWidget(withLabel, rulesNumber  + reserved_rows - 1, 2);
        widgets->last()->push_back(withLabel);

        replaceWithTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(replaceWithTextBox, rulesNumber  + reserved_rows - 1, 3);
        widgets->last()->push_back(replaceWithTextBox);

        createRemoveButton(mainGrid, rulesNumber  + reserved_rows - 1, 4, "0");
        createApplyButton(mainGrid, rulesNumber  + reserved_rows - 1, 5);

        return;
    }
    if(text == "Remove")
    {
        removeTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(removeTextBox, rulesNumber  + reserved_rows - 1, 1);
        widgets->last()->push_back(removeTextBox);
        connect(removeTextBox, &QTextEdit::textChanged, this, &MainWindow::checkTextBox);

        createRemoveButton(mainGrid, rulesNumber  + reserved_rows - 1, 2, "0");
        createApplyButton(mainGrid, rulesNumber  + reserved_rows - 1, 3);

        return;
    }
    if(text == "RemoveFromTo")
    {
        removeTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(removeTextBox, rulesNumber  + reserved_rows - 1, 1);
        widgets->last()->push_back(removeTextBox);
        connect(removeTextBox, &QTextEdit::textChanged, this, &MainWindow::checkTextBox);

        QLabel* fromLabel = new QLabel("from", mainWidget);
        mainGrid->addWidget(fromLabel, rulesNumber  + reserved_rows - 1, 2);
        widgets->last()->push_back(fromLabel);

        fromTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(fromTextBox, rulesNumber  + reserved_rows - 1, 3);
        widgets->last()->push_back(fromTextBox);
        connect(fromTextBox, &QTextEdit::textChanged, this, &MainWindow::checkTextBox);

        QLabel* toLabel = new QLabel("to", mainWidget);
        mainGrid->addWidget(toLabel, rulesNumber  + reserved_rows - 1, 4);
        widgets->last()->push_back(toLabel);

        toTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(toTextBox, rulesNumber  + reserved_rows - 1, 5);
        widgets->last()->push_back(toTextBox);
        connect(toTextBox, &QTextEdit::textChanged, this, &MainWindow::checkTextBox);

        createRemoveButton(mainGrid, rulesNumber  + reserved_rows - 1, 6, "0");
        createApplyButton(mainGrid, rulesNumber  + reserved_rows - 1, 7);

        return;
    }
    if(text == "AddTo")
    {
        addTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(addTextBox, rulesNumber  + reserved_rows - 1, 1);
        widgets->last()->push_back(addTextBox);
        connect(addTextBox, &QTextEdit::textChanged, this, &MainWindow::checkTextBox);

        QLabel* toLabel = new QLabel("to", mainWidget);
        mainGrid->addWidget(toLabel, rulesNumber  + reserved_rows - 1, 2);
        widgets->last()->push_back(toLabel);

        toTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(toTextBox, rulesNumber  + reserved_rows - 1, 3);
        widgets->last()->push_back(toTextBox);
        connect(toTextBox, &QTextEdit::textChanged, this, &MainWindow::checkTextBox);

        createRemoveButton(mainGrid, rulesNumber  + reserved_rows - 1, 4, "0");
        createApplyButton(mainGrid, rulesNumber  + reserved_rows - 1, 5);

        return;
    }
}

void MainWindow::createApplyButton(QGridLayout* layout, int row, int column)
{
    //Add a new applyButton to the selected grid cell
    QPushButton* applyButton = new QPushButton(mainWidget);
    applyButton->setText("Apply");
    connect(applyButton, &QPushButton::clicked, this, &MainWindow::applyButtonClicked);
    layout->addWidget(applyButton, row, column);
    widgets->at(row)->push_back(applyButton);
}

void MainWindow::createRemoveButton(QGridLayout* layout, int row, int column, QString status)
{
    //Add a new removeButton to the selected grid cell
    QPushButton* removeRuleButton = new QPushButton("Remove", mainWidget);
    removeRuleButton->setWindowTitle(status);
    connect(removeRuleButton, &QPushButton::clicked, this, &MainWindow::removeRuleButtonClicked);
    layout->addWidget(removeRuleButton, row, column);
    widgets->at(row)->push_back(removeRuleButton);
}

void MainWindow::correctFiles_CheckBox_Clicked()
{
    if(correctFiles_CheckBox->checkState() == Qt::Unchecked)
    {
        correctFolders_CheckBox->setCheckState(Qt::Checked);
    }
}

void MainWindow::correctFolders_CheckBox_Clicked()
{
    if(correctFolders_CheckBox->checkState() == Qt::Unchecked)
    {
        correctFiles_CheckBox->setCheckState(Qt::Checked);
    }
}

void MainWindow::logOut(QString log, LogStatus st)
{
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
    log.push_back("</p>");
    logBlock->append(log);
}

void MainWindow::clearLog()
{
    logBlock->clear();
}

void MainWindow::checkTextBox()
{
    QPalette red_pal;
    red_pal.setColor(QPalette::Background, QColor::fromRgb(255, 0, 0));
    red_pal.setColor(QPalette::Text, QColor::fromRgb(255, 0, 0));

    QPalette def_pal;
    def_pal.setColor(QPalette::Background, QColor::fromRgb(0, 100, 255));
    def_pal.setColor(QPalette::Text, QColor::fromRgb(255, 255, 255));

    if(ruleComboBox->currentText() == "Replace")
    {
        QString text = replacedTextBox->toPlainText();
        if(text.isNull() || text.isEmpty())
        {
            correctButton->setEnabled(false);
            replacedTextBox->setPalette(red_pal);
        }
        else
        {
            correctButton->setEnabled(true);
            replacedTextBox->setPalette(def_pal);
        }
    }
    else if(ruleComboBox->currentText() == "Remove")
    {
        QString text = removeTextBox->toPlainText();
        if(text.isNull() || text.isEmpty())
        {
            correctButton->setEnabled(false);
            removeTextBox->setPalette(red_pal);
        }
        else
        {
            correctButton->setEnabled(true);
            removeTextBox->setPalette(def_pal);
        }
    }
    else if(ruleComboBox->currentText() == "RemoveFromTo")
    {
        int from_num = 0;
        bool from_ok;
        int to_num = 0;
        bool to_ok;

        from_num = fromTextBox->toPlainText().toInt(&from_ok);
        to_num = toTextBox->toPlainText().toInt(&to_ok);

        if(from_ok)
        {
            if(from_num < 0 || from_num > to_num)
            {
                fromTextBox->setPalette(red_pal);
                from_ok = false;
            }
            else
            {
                fromTextBox->setPalette(def_pal);
            }
        }
        else
        {
            fromTextBox->setPalette(red_pal);
        }
        if(to_ok)
        {
            if(to_num < 0 || to_num < from_num)
            {
                toTextBox->setPalette(red_pal);
                to_ok = false;
            }
            else
            {
                toTextBox->setPalette(def_pal);
            }
        }
        else
        {
            toTextBox->setPalette(red_pal);
        }
        if(from_ok && to_ok)
        {
            correctButton->setEnabled(true);
            return;
        }
        correctButton->setEnabled(false);
    }
    else if(ruleComboBox->currentText() == "AddTo")
    {
        int to_num = 0;
        bool to_ok;

        to_num = toTextBox->toPlainText().toInt(&to_ok);

        if(to_ok)
        {
            if(to_num < 0)
            {
                toTextBox->setPalette(red_pal);
                to_ok = false;
            }
            else
            {
                toTextBox->setPalette(def_pal);
                correctButton->setEnabled(true);
                return;
            }
        }
        else
        {
            toTextBox->setPalette(red_pal);
        }

        correctButton->setEnabled(false);
    }
}
///////////////////
//////////////////
///////////////////



//////////////////
///FUNCTIONAL METHODS
//////////////////////
QString MainWindow::correctName(QString old_name)
{
    //correct all names according the rule list
    for(int i = 0; i < rules.count(); i++)
    {
        if(rules[i]->first == "Replace")
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
    }
    return old_name;
}

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

void MainWindow::reset()
{
    //This operation is similar to the correct one
    QPushButton* but = (QPushButton*)sender();

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

    QDir directory(oldDirNames.last()->first);
    QStringList file_list = directory.entryList(QDir::Files);
    QStringList dir_list = directory.entryList(QDir::Dirs);

    renameFiles(oldFileNames.last()->first, &file_list, &(oldFileNames.last()->second));
    renameDirs(oldDirNames.last()->first, &dir_list, &(oldDirNames.last()->second));

    oldDirNames.removeLast();
    oldFileNames.removeLast();
    if(oldDirNames.empty() && oldFileNames.empty())
    {
        fullResetButton->setEnabled(false);
        resetButton->setEnabled(false);
    }
    logOut("///////////", LogStatus::Reset);
    logOut("///////////", LogStatus::Reset);
    logOut(" ", LogStatus::Reset);
}

void MainWindow::renameFiles(QString path, QStringList* old_names, QStringList* new_names)
{
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


