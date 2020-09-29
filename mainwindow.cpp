#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    fileMenu = menuBar()->addMenu("&File");

    saveRuleListAction = new QAction("Save rule list");
    fileMenu->addAction(saveRuleListAction);
    connect(saveRuleListAction, &QAction::triggered, this, &MainWindow::saveRuleList);

    loadRuleListAction = new QAction("Load rule list");
    fileMenu->addAction(loadRuleListAction);
    connect(loadRuleListAction, &QAction::triggered, this, &MainWindow::loadRuleList);

    quitAction = new QAction("Quit");
    fileMenu->addAction(quitAction);
    connect(quitAction, &QAction::triggered, this, &QApplication::exit);

    aboutMenu = menuBar()->addMenu("&About");

    aboutAction = new QAction("About");
    aboutMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showHelp);


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
    createApplyButton(mainGrid, rulesNumber  + reserved_rows - 1, 2, false);

    //Apply button is disabled until the user set the rule
    widgets->last()->last()->setEnabled(false);
}

void MainWindow::correctButtonClicked()
{
    //User open a directory
    QFileDialog dialog(this);
    currentPath = dialog.getExistingDirectory();
    if(currentPath == "")
    {
        return;
    }
    QDir directory(currentPath);

    //Load files and directories names
    QStringList file_list = directory.entryList(QDir::Files);
    QStringList dir_list = directory.entryList(QDir::Dirs);

    //Correct files if the file-checkbox was checked
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
    if(correctFolders_CheckBox->checkState() == Qt::Checked)
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
            //Also remove the rule
            rules.removeAt(i - reserved_rows);
            break;
        }
    }
    addRuleButton->setEnabled(true);
    rulesNumber--;
    if(rulesNumber == 0)
    {
        correctButton->setEnabled(false);
    }
}

void MainWindow::applyButtonClicked()
{
    //Replace current widgets with new inactive ones according the selected rule
    addRuleButton->setEnabled(true);
    correctButton->setEnabled(true);

    QLabel* ruleLabel = new QLabel(mainWidget);
    ruleLabel->setText(QString::fromStdString(std::to_string(rulesNumber))
                       + ". " + ruleComboBox->currentText());
    mainGrid->addWidget(ruleLabel, rulesNumber  + reserved_rows - 1, 0);

    if(ruleComboBox->currentText() == "Replace")
    {
        addReplaceRule(ruleLabel, replacedTextBox->toPlainText(),
                       replaceWithTextBox->toPlainText(), true);
    }
    else if(ruleComboBox->currentText() == "Remove")
    {
        addRemoveRule(ruleLabel, removeTextBox->toPlainText(), true);
    }
    else if(ruleComboBox->currentText() == "RemoveFromTo")
    {
        addRemoveFromToRule(ruleLabel, removeTextBox->toPlainText(),
                            fromTextBox->toPlainText(),
                            toTextBox->toPlainText(), true);
    }
    else if(ruleComboBox->currentText() == "AddTo")
    {
        addAddToRule(ruleLabel, addTextBox->toPlainText(),
                     toTextBox->toPlainText(), true);
    }
}

void MainWindow::addReplaceRule(QLabel* ruleLabel, QString replaced, QString replace_with, bool is_new)
{
    QLabel* replacedLabel = new QLabel(mainWidget);
    replacedLabel->setText(replaced);
    mainGrid->addWidget(replacedLabel, rulesNumber  + reserved_rows - 1, 1);

    QLabel* withLabel = new QLabel("with", mainWidget);
    mainGrid->addWidget(withLabel, rulesNumber  + reserved_rows - 1, 2);

    QLabel* replaceWithLabel = new QLabel(mainWidget);
    replaceWithLabel->setText(replace_with);
    mainGrid->addWidget(replaceWithLabel, rulesNumber  + reserved_rows - 1, 3);

    QString* args = new QString[2];
    args[0] = replaced;
    args[1] = replace_with;
    rules.push_back(new QPair<QString, QString*>("Replace", args));

    if(is_new)
    {
        for(int i = 0; i < widgets->last()->count(); i++)
        {
            delete widgets->last()->at(i);
        }
        widgets->removeLast();
    }

    widgets->push_back(new QList<QWidget*>());
    widgets->last()->push_back(ruleLabel);
    widgets->last()->push_back(replacedLabel);
    widgets->last()->push_back(withLabel);
    widgets->last()->push_back(replaceWithLabel);
    createRemoveButton(mainGrid, rulesNumber  + reserved_rows - 1, 4, "1");
}

void MainWindow::addRemoveRule(QLabel* ruleLabel, QString remove, bool is_new)
{
    QLabel* removeLabel = new QLabel(mainWidget);
    removeLabel->setText(remove);
    mainGrid->addWidget(removeLabel, rulesNumber  + reserved_rows - 1, 1);

    QString* args = new QString[1];
    args[0] = remove;

    rules.push_back(new QPair<QString, QString*>("Remove", args));

    if(is_new)
    {
        for(int i = 0; i < widgets->last()->count(); i++)
        {
            delete widgets->last()->at(i);
        }
        widgets->removeLast();
    }

    widgets->push_back(new QList<QWidget*>());
    widgets->last()->push_back(ruleLabel);
    widgets->last()->push_back(removeLabel);
    createRemoveButton(mainGrid, rulesNumber  + reserved_rows - 1, 2, "1");
}

void MainWindow::addRemoveFromToRule(QLabel* ruleLabel, QString remove, QString from_p, QString to_p, bool is_new)
{
    ruleLabel->setText(QString::fromStdString(std::to_string(rulesNumber))
                       + ". " + "Remove");

    QLabel* removeLabel = new QLabel(mainWidget);
    removeLabel->setText(remove);
    if(removeLabel->text() == "")
    {
        removeLabel->setText("All");
    }
    mainGrid->addWidget(removeLabel, rulesNumber  + reserved_rows - 1, 1);

    QLabel* from = new QLabel("from", mainWidget);
    mainGrid->addWidget(from, rulesNumber  + reserved_rows - 1, 2);

    QLabel* fromLabel = new QLabel(mainWidget);
    fromLabel->setText(from_p);
    mainGrid->addWidget(fromLabel, rulesNumber  + reserved_rows - 1, 3);

    QLabel* to = new QLabel("to", mainWidget);
    mainGrid->addWidget(to, rulesNumber  + reserved_rows - 1, 4);

    QLabel* toLabel = new QLabel(mainWidget);
    toLabel->setText(to_p);
    mainGrid->addWidget(toLabel, rulesNumber  + reserved_rows - 1, 5);

    QString* args = new QString[3];
    args[0] = remove;
    args[1] = from_p;
    args[2] = to_p;

    rules.push_back(new QPair<QString, QString*>("RemoveFromTo", args));

    if(is_new)
    {
        for(int i = 0; i < widgets->last()->count(); i++)
        {
            delete widgets->last()->at(i);
        }
        widgets->removeLast();
    }

    widgets->push_back(new QList<QWidget*>());
    widgets->last()->push_back(ruleLabel);
    widgets->last()->push_back(removeLabel);
    widgets->last()->push_back(from);
    widgets->last()->push_back(fromLabel);
    widgets->last()->push_back(to);
    widgets->last()->push_back(toLabel);
    createRemoveButton(mainGrid, rulesNumber  + reserved_rows - 1, 6,"1");
}

void MainWindow::addAddToRule(QLabel* ruleLabel, QString addition, QString to_p, bool is_new)
{
    QLabel* addLabel = new QLabel(mainWidget);
    addLabel->setText(addition);
    mainGrid->addWidget(addLabel, rulesNumber  + reserved_rows - 1, 1);

    QLabel* to = new QLabel("to", mainWidget);
    mainGrid->addWidget(to, rulesNumber  + reserved_rows - 1, 2);

    QLabel* toLabel = new QLabel(mainWidget);
    toLabel->setText(to_p);
    mainGrid->addWidget(toLabel, rulesNumber  + reserved_rows - 1, 3);

    QString* args = new QString[2];
    args[0] = addition;
    args[1] = to_p;

    rules.push_back(new QPair<QString, QString*>("AddTo", args));

    if(is_new)
    {
        for(int i = 0; i < widgets->last()->count(); i++)
        {
            delete widgets->last()->at(i);
        }
        widgets->removeLast();
    }

    widgets->push_back(new QList<QWidget*>());
    widgets->last()->push_back(ruleLabel);
    widgets->last()->push_back(addLabel);
    widgets->last()->push_back(to);
    widgets->last()->push_back(toLabel);
    createRemoveButton(mainGrid, rulesNumber  + reserved_rows - 1, 4, "1");
}

void MainWindow::ruleComboBoxTextChanged(const QString& text)
{
    correctButton->setEnabled(false);

    //Replace current widgets with new ones according the selected rule
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
        createApplyButton(mainGrid, rulesNumber  + reserved_rows - 1, 5, false);

        return;
    }
    if(text == "Remove")
    {
        removeTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(removeTextBox, rulesNumber  + reserved_rows - 1, 1);
        widgets->last()->push_back(removeTextBox);
        connect(removeTextBox, &QTextEdit::textChanged, this, &MainWindow::checkTextBox);

        createRemoveButton(mainGrid, rulesNumber  + reserved_rows - 1, 2, "0");
        createApplyButton(mainGrid, rulesNumber  + reserved_rows - 1, 3, false);

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
        createApplyButton(mainGrid, rulesNumber  + reserved_rows - 1, 7, false);

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
        createApplyButton(mainGrid, rulesNumber  + reserved_rows - 1, 5, false);

        return;
    }
}

void MainWindow::createApplyButton(QGridLayout* layout, int row, int column, bool enabled)
{
    //Add a new applyButton to the selected grid cell
    QPushButton* applyButton = new QPushButton(mainWidget);
    applyButton->setText("Apply");
    applyButton->setEnabled(enabled);
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
    //At least one of these checkboxes must be checked
    if(correctFiles_CheckBox->checkState() == Qt::Unchecked)
    {
        correctFolders_CheckBox->setCheckState(Qt::Checked);
    }
}

void MainWindow::correctFolders_CheckBox_Clicked()
{
    //At least one of these checkboxes must be checked
    if(correctFolders_CheckBox->checkState() == Qt::Unchecked)
    {
        correctFiles_CheckBox->setCheckState(Qt::Checked);
    }
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

void MainWindow::clearLog()
{
    //No comments
    logBlock->clear();
}

void MainWindow::checkTextBox()
{
    QPushButton* applyButt;

    //Search for the required line
    for(int i = widgets->count() - 1; i > -1; i--)
    {
        for(int j = 0; j < widgets->at(i)->count(); j++)
        {
            if(sender() == widgets->at(i)->at(j))
            {
                applyButt = (QPushButton*)widgets->at(i)->last();
            }
        }
    }
    //Palette for errors
    QPalette red_pal;
    red_pal.setColor(QPalette::Background, QColor::fromRgb(255, 0, 0));
    red_pal.setColor(QPalette::Text, QColor::fromRgb(255, 0, 0));

    //Standart palette
    QPalette def_pal;
    def_pal.setColor(QPalette::Background, QColor::fromRgb(0, 100, 255));
    def_pal.setColor(QPalette::Text, QColor::fromRgb(255, 255, 255));

    if(ruleComboBox->currentText() == "Replace")
    {
        //At least the replacedTextBox must have a value
        QString text = replacedTextBox->toPlainText();
        if(text.isNull() || text.isEmpty())
        {
            applyButt->setEnabled(false);
            replacedTextBox->setPalette(red_pal);
        }
        else
        {
            applyButt->setEnabled(true);
            replacedTextBox->setPalette(def_pal);
        }
    }
    else if(ruleComboBox->currentText() == "Remove")
    {
        //removeTextBox must have a value(program needs to know what to remove)
        QString text = removeTextBox->toPlainText();
        if(text.isNull() || text.isEmpty())
        {
            applyButt->setEnabled(false);
            removeTextBox->setPalette(red_pal);
        }
        else
        {
            applyButt->setEnabled(true);
            removeTextBox->setPalette(def_pal);
        }
    }
    else if(ruleComboBox->currentText() == "RemoveFromTo")
    {
        //fromTextBox and toTextBox must have a value (program needs to know the range)
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
             applyButt->setEnabled(true);
            return;
        }
         applyButt->setEnabled(false);
    }
    else if(ruleComboBox->currentText() == "AddTo")
    {
        //toTextBox must have a value (program needs to know the position)
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
                 applyButt->setEnabled(true);
                return;
            }
        }
        else
        {
            toTextBox->setPalette(red_pal);
        }

         applyButt->setEnabled(false);
    }
}

void MainWindow::saveRuleList()
{
    if(rulesNumber == 0)
    {
        QMessageBox msg(this);
        msg.setText("Nothing to save");
        msg.exec();
        return;
    }

    QString data = "";
    for(int i = 0; i < rulesNumber; i++)
    {
        if(rules.at(i)->first == "Replace")
        {
            data.push_back(rules.at(i)->first + " " +
                           rules.at(i)->second[0] + " " +
                           rules.at(i)->second[1]);
        }
        else if(rules.at(i)->first == "Remove")
        {
            data.push_back(rules.at(i)->first + " " +
                           rules.at(i)->second[0]);
        }
        else if(rules.at(i)->first == "RemoveFromTo")
        {
            data.push_back(rules.at(i)->first + " " +
                           rules.at(i)->second[0] + " " +
                           rules.at(i)->second[1] +  " " +
                           rules.at(i)->second[2]);
        }
        else if(rules.at(i)->first == "AddTo")
        {
            data.push_back(rules.at(i)->first + " " +
                           rules.at(i)->second[0] + " " +
                           rules.at(i)->second[1]);
        }
        data.push_back("&");
    }
    data.remove(data.length() - 1, 1);
    QFileDialog dialog(this);
    QString path = dialog.getSaveFileName();
    QFile file(path);

    if(file.open(QFile::WriteOnly))
    {
        file.write(data.toUtf8());
        file.close();
    }
}

void MainWindow::loadRuleList()
{
    QString data = "";
    QFileDialog dialog(this);
    dialog.setFilter(QDir::Files);
    QString path = dialog.getOpenFileName();
    if(path == "")
    {
        return;
    }
    QFile file(path);
    if(file.open(QFile::ReadOnly))
    {
        data = QString::fromUtf8(file.readAll());
        file.close();
    }

    QStringList rules = data.split('&');
    for(int i = 0; i < rules.count(); i++)
    {
        rulesNumber++;
        QStringList args = rules.at(i).split(' ');

        if(rules.at(i).startsWith("Replace "))
        {
            QLabel* ruleLabel = new QLabel(mainWidget);
            ruleLabel->setText(QString::fromStdString(std::to_string(rulesNumber))
                               + ". " + "Replace");
            mainGrid->addWidget(ruleLabel, rulesNumber  + reserved_rows - 1, 0);

            addReplaceRule(ruleLabel, args.at(1), args.at(2), false);
        }
        else if(rules.at(i).startsWith("Remove "))
        {
            QLabel* ruleLabel = new QLabel(mainWidget);
            ruleLabel->setText(QString::fromStdString(std::to_string(rulesNumber))
                               + ". " + "Remove");
            mainGrid->addWidget(ruleLabel, rulesNumber  + reserved_rows - 1, 0);

            addRemoveRule(ruleLabel, args.at(1), false);
        }
        else if(rules.at(i).startsWith("RemoveFromTo "))
        {
            QLabel* ruleLabel = new QLabel(mainWidget);
            ruleLabel->setText(QString::fromStdString(std::to_string(rulesNumber))
                               + ". " + "RemoveFromTo");
            mainGrid->addWidget(ruleLabel, rulesNumber  + reserved_rows - 1, 0);
            if(args.count() == 4)
            {
                addRemoveFromToRule(ruleLabel, args.at(1), args.at(2), args.at(3), false);
            }
            else
            {
                addRemoveFromToRule(ruleLabel, "", args.at(1), args.at(2), false);
            }
        }
        else if(rules.at(i).startsWith("AddTo "))
        {
            QLabel* ruleLabel = new QLabel(mainWidget);
            ruleLabel->setText(QString::fromStdString(std::to_string(rulesNumber))
                               + ". " + "AddTo");
            mainGrid->addWidget(ruleLabel, rulesNumber  + reserved_rows - 1, 0);

            addAddToRule(ruleLabel, args.at(1), args.at(2), false);
        }
    }
}

void MainWindow::showHelp()
{

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
        else if(rules[i]->first == "MakeList")
        {
            old_name = makeList(old_name, rules[i]->second);
        }
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

///////////////////
//////////////////
///////////////////
