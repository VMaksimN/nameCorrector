#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //Init widgets storage
    widgets = new QList<QList<QWidget*>*>();
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

    //Init button starting fix operations
    fixButton = new QPushButton("Fix names", mainWidget);
    connect(fixButton, &QPushButton::clicked, this, &MainWindow::fixButtonClicked);
    mainGrid->addWidget(fixButton, 0, 1);
    widgets->first()->push_back(fixButton);

    //Init button replacing new names with old ones
    resetButton = new QPushButton("Reset", mainWidget);
    resetButton->setEnabled(false);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::reset);
    mainGrid->addWidget(resetButton, 0, 2);
    widgets->first()->push_back(resetButton);

    fixFiles_CheckBox = new QCheckBox("Fix files", mainWidget);
    fixFiles_CheckBox->setCheckState(Qt::Checked);
    connect(fixFiles_CheckBox, &QCheckBox::clicked, this, &MainWindow::fixFiles_CheckBox_Clicked);
    mainGrid->addWidget(fixFiles_CheckBox, 0, 3);
    widgets->first()->push_back(fixFiles_CheckBox);

    fixFolders_CheckBox = new QCheckBox("Fix folders", mainWidget);
    fixFolders_CheckBox->setCheckState(Qt::Unchecked);
    connect(fixFolders_CheckBox, &QCheckBox::clicked, this, &MainWindow::fixFolders_CheckBox_Clicked);
    mainGrid->addWidget(fixFolders_CheckBox, 0, 4);
    widgets->first()->push_back(fixFolders_CheckBox);

}

MainWindow::~MainWindow(){}

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
    mainGrid->addWidget(ruleComboBox, rulesNumber, 0);
    widgets->last()->push_back(ruleComboBox);

    createRemoveButton(mainGrid, rulesNumber, 1);
    createApplyButton(mainGrid, rulesNumber, 2);
}

void MainWindow::fixButtonClicked()
{
    //User open a directory and the program start fix operations and remember old names
    QFileDialog dialog(this);
    currentPath = dialog.getExistingDirectory();
    QDir dir(currentPath);
    QStringList path_list = dir.entryList();
    oldNames = path_list;
    for(uint i = 0; i < dir.count(); i++)
    {
        if(QFile::rename(currentPath + "/" + path_list[i], currentPath + "/" + fixName(path_list[i])) != 0)
        {
            QMessageBox box(this);
            box.setText("Operation fault!");
            box.show();
        }
    }
    resetButton->setEnabled(true);
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
    mainGrid->addWidget(ruleLabel, rulesNumber, 0);

    if(ruleComboBox->currentText() == "Replace")
    {
        QLabel* replacedLabel = new QLabel(mainWidget);
        replacedLabel->setText(replacedTextBox->toPlainText());
        mainGrid->addWidget(replacedLabel, rulesNumber, 1);

        QLabel* withLabel = new QLabel("with", mainWidget);
        mainGrid->addWidget(withLabel, rulesNumber, 2);

        QLabel* replaceWithLabel = new QLabel(mainWidget);
        replaceWithLabel->setText(replaceWithTextBox->toPlainText());
        mainGrid->addWidget(replaceWithLabel, rulesNumber, 3);

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
        createRemoveButton(mainGrid, rulesNumber, 4);
    }
    else if(ruleComboBox->currentText() == "Remove")
    {
        QLabel* removeLabel = new QLabel(mainWidget);
        removeLabel->setText(removeTextBox->toPlainText());
        mainGrid->addWidget(removeLabel, rulesNumber, 1);

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
        createRemoveButton(mainGrid, rulesNumber, 2);
    }
    else if(ruleComboBox->currentText() == "RemoveFromTo")
    {
        QLabel* removeLabel = new QLabel(mainWidget);
        removeLabel->setText(removeTextBox->toPlainText());
        mainGrid->addWidget(removeLabel, rulesNumber, 1);

        QLabel* from = new QLabel("from", mainWidget);
        mainGrid->addWidget(from, rulesNumber, 2);

        QLabel* fromLabel = new QLabel(mainWidget);
        fromLabel->setText(fromTextBox->toPlainText());
        mainGrid->addWidget(fromLabel, rulesNumber, 3);

        QLabel* to = new QLabel("to", mainWidget);
        mainGrid->addWidget(to, rulesNumber, 4);

        QLabel* toLabel = new QLabel(mainWidget);
        toLabel->setText(toTextBox->toPlainText());
        mainGrid->addWidget(toLabel, rulesNumber, 5);

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
        createRemoveButton(mainGrid, rulesNumber, 6);
    }
    else if(ruleComboBox->currentText() == "AddTo")
    {
        QLabel* addLabel = new QLabel(mainWidget);
        addLabel->setText(addTextBox->toPlainText());
        mainGrid->addWidget(addLabel, rulesNumber, 1);

        QLabel* to = new QLabel("to", mainWidget);
        mainGrid->addWidget(to, rulesNumber, 2);

        QLabel* toLabel = new QLabel(mainWidget);
        toLabel->setText(toTextBox->toPlainText());
        mainGrid->addWidget(toLabel, rulesNumber, 3);

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
        createRemoveButton(mainGrid, rulesNumber, 4);
    }
}

void MainWindow::ruleComboBoxTextChanged(const QString& text)
{
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
        mainGrid->addWidget(replacedTextBox, rulesNumber, 1);
        widgets->last()->push_back(replacedTextBox);

        QLabel* withLabel = new QLabel("with", mainWidget);
        mainGrid->addWidget(withLabel, rulesNumber, 2);
        widgets->last()->push_back(withLabel);

        replaceWithTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(replaceWithTextBox, rulesNumber, 3);
        widgets->last()->push_back(replaceWithTextBox);

        createRemoveButton(mainGrid, rulesNumber, 4);
        createApplyButton(mainGrid, rulesNumber, 5);

        return;
    }
    if(text == "Remove")
    {
        removeTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(removeTextBox, rulesNumber, 1);
        widgets->last()->push_back(removeTextBox);

        createRemoveButton(mainGrid, rulesNumber, 2);
        createApplyButton(mainGrid, rulesNumber, 3);

        return;
    }
    if(text == "RemoveFromTo")
    {
        removeTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(removeTextBox, rulesNumber, 1);
        widgets->last()->push_back(removeTextBox);

        QLabel* fromLabel = new QLabel("from", mainWidget);
        mainGrid->addWidget(fromLabel, rulesNumber, 2);
        widgets->last()->push_back(fromLabel);

        fromTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(fromTextBox, rulesNumber, 3);
        widgets->last()->push_back(fromTextBox);

        QLabel* toLabel = new QLabel("to", mainWidget);
        mainGrid->addWidget(toLabel, rulesNumber, 4);
        widgets->last()->push_back(toLabel);

        toTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(toTextBox, rulesNumber, 5);
        widgets->last()->push_back(toTextBox);

        createRemoveButton(mainGrid, rulesNumber, 6);
        createApplyButton(mainGrid, rulesNumber, 7);

        return;
    }
    if(text == "AddTo")
    {
        addTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(addTextBox, rulesNumber, 1);
        widgets->last()->push_back(addTextBox);

        QLabel* toLabel = new QLabel("to", mainWidget);
        mainGrid->addWidget(toLabel, rulesNumber, 2);
        widgets->last()->push_back(toLabel);

        toTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(toTextBox, rulesNumber, 3);
        widgets->last()->push_back(toTextBox);

        createRemoveButton(mainGrid, rulesNumber, 4);
        createApplyButton(mainGrid, rulesNumber, 5);

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

void MainWindow::createRemoveButton(QGridLayout* layout, int row, int column)
{
    //Add a new removeButton to the selected grid cell
    QPushButton* removeRuleButton = new QPushButton("Remove", mainWidget);
    connect(removeRuleButton, &QPushButton::clicked, this, &MainWindow::removeRuleButtonClicked);
    layout->addWidget(removeRuleButton, row, column);
    widgets->at(row)->push_back(removeRuleButton);
}

QString MainWindow::fixName(QString old_name)
{
    //Fix all names according the rule list
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
    if(args[1].toInt() < 0 || args[2].toInt() < 0)
    {
        return old;
    }
    QString modified = old.mid(args[1].toInt(), args[2].toInt());
    QString new_str = old.mid(args[1].toInt(), args[2].toInt()).replace(args[0], "");
    old = old.replace(modified, new_str);
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
    //This operation is similar to the fix one
    resetButton->setEnabled(false);
    QDir dir(currentPath);
    QStringList path_list = dir.entryList();
    for(uint i = 0; i < dir.count(); i++)
    {
        if(QFile::rename(currentPath + "/" + path_list[i], currentPath + "/" + oldNames[i]) != 0)
        {
            QMessageBox box(this);
            box.setText("Operation fault!");
            box.show();
        }
    }
}

void MainWindow::fixFiles_CheckBox_Clicked()
{
    if(fixFiles_CheckBox->checkState() == Qt::Unchecked)
    {
        fixFolders_CheckBox->setCheckState(Qt::Checked);
    }
}

void MainWindow::fixFolders_CheckBox_Clicked()
{
    if(fixFolders_CheckBox->checkState() == Qt::Unchecked)
    {
        fixFiles_CheckBox->setCheckState(Qt::Checked);
    }
}



