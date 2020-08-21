#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    mainGrid = new QGridLayout(mainWidget);
    mainWidget->setLayout(mainGrid);

    fixButton = new QPushButton("Fix names");
    connect(fixButton, &QPushButton::clicked, this, &MainWindow::fixButtonClicked);
    mainGrid->addWidget(fixButton, 0, 1);

    addRuleButton = new QPushButton("New rule");
    connect(addRuleButton, &QPushButton::clicked, this, &MainWindow::addRuleButtonClicked);
    mainGrid->addWidget(addRuleButton, 0, 0);
}

MainWindow::~MainWindow()
{
}

void MainWindow::addRuleButtonClicked()
{
    rulesNumber++;

    addRuleButton->setEnabled(false);

    ruleComboBox = new QComboBox(mainWidget);
    ruleComboBox->addItem("None");
    ruleComboBox->addItem("Replace");
    ruleComboBox->addItem("Remove");
    ruleComboBox->addItem("Remove from");
    connect(ruleComboBox, &QComboBox::currentTextChanged, this, &MainWindow::ruleComboBoxTextChanged);
    mainGrid->addWidget(ruleComboBox, rulesNumber, 0);

    QPushButton* removeRuleButton = new QPushButton("Remove");
    connect(removeRuleButton, &QPushButton::clicked, this, &MainWindow::removeRuleButtonClicked);
    removeRuleButton->setObjectName(QString::fromStdString(std::to_string(rulesNumber)));
    mainGrid->addWidget(removeRuleButton, rulesNumber, 6);

    QPushButton* applyButton = new QPushButton(mainWidget);
    applyButton->setText("Apply");
    connect(applyButton, &QPushButton::clicked, this, &MainWindow::applyButtonClicked);
    mainGrid->addWidget(applyButton, rulesNumber, 7);
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
    removeRow(mainGrid, getWidgetPosition(mainGrid, (QWidget*)sender()).first, true);
    addRuleButton->setEnabled(true);
    rulesNumber--;
    delete sender();
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

        QLabel* withLabel = new QLabel("with", mainWidget);
        mainGrid->addWidget(withLabel, rulesNumber, 2);

        replaceWithTextBox = new QTextEdit(mainWidget);
        mainGrid->addWidget(replaceWithTextBox, rulesNumber, 3);
    }
}

QString MainWindow::fix_name(QString name)
{
    //int pos = name.indexOf('-');
    //name = name.replace(0, pos + 1, "");
    //name = name.replace(0, 4, "");
    //name = name.replace(' ', '_');
    //name = name.replace("_-_", "-");
    //name = name.replace(",_", "_");
    //name = name.replace("'_", "_");
    return name;
}

void MainWindow::removeRow(QGridLayout *layout, int row, bool deleteWidgets)
{
    remove(layout, row, -1, deleteWidgets);
    layout->setRowMinimumHeight(row, 0);
    layout->setRowStretch(row, 0);
}

void MainWindow::remove(QGridLayout *layout, int row, int column, bool deleteWidgets)
{
    // Avoid usage of QGridLayout::itemAtPosition() here to improve performance.
    for (int i = layout->count() - 1; i >= 0; i--)
    {
        int r, c, rs, cs;
        layout->getItemPosition(i, &r, &c, &rs, &cs);
        if ((r <= row && r + rs - 1 >= row) || (c <= column && c + cs - 1 >= column))
        {
            // This layout item is subject to deletion.
            QLayoutItem *item = layout->takeAt(i);
            if (deleteWidgets)
            {
                deleteChildWidgets(item);
            }
            delete item;
        }
    }
}


void MainWindow::deleteChildWidgets(QLayoutItem *item)
{
    if (item->layout())
    {
        // Process all child items recursively.
        for (int i = 0; i < item->layout()->count(); i++) {
            deleteChildWidgets(item->layout()->itemAt(i));
        }
    }
    delete item->widget();
}


QString MainWindow::replace(QString* old, QString* args)
{
    return old->replace(args[0], args[1]);
}


QPair<int,int> MainWindow::getWidgetPosition(QGridLayout* layout, QWidget* widget)
{
    QPair<int,int> res;
    int row;
    int column;
    int rs;
    int cs;
    // Avoid usage of QGridLayout::itemAtPosition() here to improve performance.
    for (int i = layout->count() - 1; i >= 0; i--)
    {
        layout->getItemPosition(i, &row, &column, &rs, &cs);
        if(layout->takeAt(i)->widget() == widget)
        {
            res.first = row;
            res.second = column;
            break;
        }
    }

    return res;
}
