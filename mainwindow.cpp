#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    widgets = new QList<QList<QWidget*>*>();
    widgets->push_back(new QList<QWidget*>());

    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    mainGrid = new QGridLayout(mainWidget);
    mainWidget->setLayout(mainGrid);

    fixButton = new QPushButton("Fix names", mainWidget);
    connect(fixButton, &QPushButton::clicked, this, &MainWindow::fixButtonClicked);
    mainGrid->addWidget(fixButton, 0, 1);
    widgets->first()->push_back(fixButton);

    addRuleButton = new QPushButton("New rule", mainWidget);
    connect(addRuleButton, &QPushButton::clicked, this, &MainWindow::addRuleButtonClicked);
    mainGrid->addWidget(addRuleButton, 0, 0);
    widgets->first()->push_back(fixButton);
}

MainWindow::~MainWindow()
{
}

void MainWindow::addRuleButtonClicked()
{
    rulesNumber++;
    widgets->push_back(new QList<QWidget*>());
    addRuleButton->setEnabled(false);

    ruleComboBox = new QComboBox(mainWidget);
    ruleComboBox->addItem("None");
    ruleComboBox->addItem("Replace");
    ruleComboBox->addItem("Remove");
    ruleComboBox->addItem("Remove from");
    connect(ruleComboBox, &QComboBox::currentTextChanged, this, &MainWindow::ruleComboBoxTextChanged);
    mainGrid->addWidget(ruleComboBox, rulesNumber, 0);
    widgets->last()->push_back(ruleComboBox);

    createRemoveButton(mainGrid, rulesNumber, 1);
    /*QPushButton* removeRuleButton = new QPushButton("Remove", mainWidget);
    connect(removeRuleButton, &QPushButton::clicked, this, &MainWindow::removeRuleButtonClicked);
    mainGrid->addWidget(removeRuleButton, rulesNumber, 1);
    widgets->last()->push_back(removeRuleButton);*/

    createApplyButton(mainGrid, rulesNumber, 2);
    /*QPushButton* applyButton = new QPushButton(mainWidget);
    applyButton->setText("Apply");
    connect(applyButton, &QPushButton::clicked, this, &MainWindow::applyButtonClicked);
    mainGrid->addWidget(applyButton, rulesNumber, 2);
    widgets->last()->push_back(applyButton);*/
}


void MainWindow::fixButtonClicked()
{
    QFileDialog dialog(this);
    QString path = dialog.getExistingDirectory();
    QDir dir(path);
    QStringList path_list = dir.entryList();
    for(uint i = 0; i < dir.count(); i++)
    {
        if(QFile::rename(path + "/" + path_list[i], path + "/" + fix_name(path_list[i])) != 0)
        {
            QMessageBox box(this);
            box.setText("Operation fault! You are loser");
        }
    }
}

void MainWindow::removeRuleButtonClicked()
{
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

        /*QPushButton* removeRuleButton = new QPushButton("Remove", mainWidget);
        connect(removeRuleButton, &QPushButton::clicked, this, &MainWindow::removeRuleButtonClicked);
        mainGrid->addWidget(removeRuleButton, rulesNumber, 4);*/

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
        //widgets->last()->push_back(removeRuleButton);
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


}

void MainWindow::ruleComboBoxTextChanged(const QString& text)
{
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
        /*QPushButton* removeRuleButton = new QPushButton("Remove", mainWidget);
        connect(removeRuleButton, &QPushButton::clicked, this, &MainWindow::removeRuleButtonClicked);
        mainGrid->addWidget(removeRuleButton, rulesNumber, 4);
        widgets->last()->push_back(removeRuleButton);*/

        createApplyButton(mainGrid, rulesNumber, 5);
        /*QPushButton* applyButton = new QPushButton(mainWidget);
        applyButton->setText("Apply");
        connect(applyButton, &QPushButton::clicked, this, &MainWindow::applyButtonClicked);
        mainGrid->addWidget(applyButton, rulesNumber, 5);
        widgets->last()->push_back(applyButton);*/

        return;
    }
    if(text == "Remove")
    {
        removeTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(removeTextBox, rulesNumber, 1);
        widgets->last()->push_back(removeTextBox);

        createRemoveButton(mainGrid, rulesNumber, 2);
        /*QPushButton* removeRuleButton = new QPushButton("Remove", mainWidget);
        connect(removeRuleButton, &QPushButton::clicked, this, &MainWindow::removeRuleButtonClicked);
        mainGrid->addWidget(removeRuleButton, rulesNumber, 2);
        widgets->last()->push_back(removeRuleButton);*/

        createApplyButton(mainGrid, rulesNumber, 3);
        /*QPushButton* applyButton = new QPushButton(mainWidget);
        applyButton->setText("Apply");
        connect(applyButton, &QPushButton::clicked, this, &MainWindow::applyButtonClicked);
        mainGrid->addWidget(applyButton, rulesNumber, 3);
        widgets->last()->push_back(applyButton);*/

        return;
    }
}

void MainWindow::createApplyButton(QGridLayout* layout, int row, int column)
{
    QPushButton* applyButton = new QPushButton(mainWidget);
    applyButton->setText("Apply");
    connect(applyButton, &QPushButton::clicked, this, &MainWindow::applyButtonClicked);
    layout->addWidget(applyButton, row, column);
    widgets->at(row)->push_back(applyButton);
}

void MainWindow::createRemoveButton(QGridLayout* layout, int row, int column)
{
    QPushButton* removeRuleButton = new QPushButton("Remove", mainWidget);
    connect(removeRuleButton, &QPushButton::clicked, this, &MainWindow::removeRuleButtonClicked);
    layout->addWidget(removeRuleButton, row, column);
    widgets->at(row)->push_back(removeRuleButton);
}

QString MainWindow::fix_name(QString old_name)
{
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
