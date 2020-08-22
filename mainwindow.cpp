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

    QPushButton* removeRuleButton = new QPushButton("Remove", mainWidget);
    connect(removeRuleButton, &QPushButton::clicked, this, &MainWindow::removeRuleButtonClicked);
    removeRuleButton->setObjectName(QString::fromStdString(std::to_string(rulesNumber)));
    mainGrid->addWidget(removeRuleButton, rulesNumber, 6);
    widgets->last()->push_back(removeRuleButton);

    QPushButton* applyButton = new QPushButton(mainWidget);
    applyButton->setText("Apply");
    connect(applyButton, &QPushButton::clicked, this, &MainWindow::applyButtonClicked);
    mainGrid->addWidget(applyButton, rulesNumber, 7);
    widgets->last()->push_back(applyButton);
}


void MainWindow::fixButtonClicked()
{
    QFileDialog dialog(this);
    QString path = dialog.getExistingDirectory();
    QDir dir(path);
    QStringList path_list = dir.entryList();
    for(uint i = 0; i < dir.count(); i++)
    {
        /*if(QFile::rename(path + "/" + path_list[i], path + "/" + fix_name(path_list[i])) != 0)
        {
            QMessageBox box(this);
            box.setText("Operation fault! You are loser");
        }*/
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

        QLabel* replaceWithLabel = new QLabel(mainWidget);
        replaceWithLabel->setText(replaceWithTextBox->toPlainText());
        mainGrid->addWidget(replaceWithLabel, rulesNumber, 3);

        QString* args = new QString[2];
        args[0] = replacedTextBox->toPlainText();
        args[1] = replaceWithTextBox->toPlainText();

        rules.insert("Replace", args);

        delete replacedTextBox;
        delete replaceWithTextBox;
    }
    delete ruleComboBox;
    delete sender();
}

void MainWindow::ruleComboBoxTextChanged(const QString& text)
{
    if(text == "Replace")
    {
        replacedTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(replacedTextBox, rulesNumber, 1);
        widgets->last()->push_front(replacedTextBox);

        QLabel* withLabel = new QLabel("with", mainWidget);
        mainGrid->addWidget(withLabel, rulesNumber, 2);
        widgets->last()->push_front(withLabel);

        replaceWithTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(replaceWithTextBox, rulesNumber, 3);
        widgets->last()->push_front(replaceWithTextBox);
    }
}

QString MainWindow::replace(QString* old, QString* args)
{
    return old->replace(args[0], args[1]);
}
