#include "newelementwindow.h"


NewElementWindow::NewElementWindow(QString type, QWidget *parent) : QMainWindow(parent)
{
    //INit result
    result = new ListElement("", "", type);

    //INit main
    mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    mainLayout = new QVBoxLayout(mainWidget);
    mainWidget->setLayout(mainLayout);


    //INit the type row
    typeLayout = new QHBoxLayout(mainWidget);
    mainLayout->addLayout(typeLayout);

    typeLabel = new QLabel("Select the element type");
    typeLayout->addWidget(typeLabel);

    typeComboBox = new QComboBox(mainWidget);
    if(type == "Rules")
    {
        typeComboBox->addItem("None");
        typeComboBox->addItem("Replace");
        typeComboBox->addItem("Remove");
        typeComboBox->addItem("Add");
        typeComboBox->addItem("MakeList");
    }
    else //type == directories
    {
        typeComboBox->addItem("None");
        typeComboBox->addItem("Directory");
    }
    connect(typeComboBox, &QComboBox::currentTextChanged,
            this, &NewElementWindow::typeComboBox_ItemChanged);
    typeLayout->addWidget(typeComboBox);

    helpButton = new QPushButton("Help", mainWidget);
    typeLayout->addWidget(helpButton);


    //INit the rule row
    ruleLayout = new QHBoxLayout(mainWidget);
    mainLayout->addLayout(ruleLayout);

    ruleLabel = new QLabel("");
    ruleLayout->addWidget(ruleLabel);


    //INit the buttons row
    buttonsLayout = new QHBoxLayout(mainWidget);
    mainLayout->addLayout(buttonsLayout);

    cancelButton = new QPushButton("Cancel", mainWidget);
    connect(cancelButton, &QPushButton::clicked, this, &NewElementWindow::cancelButton_Clicked);
    buttonsLayout->addWidget(cancelButton);

    clearButton = new QPushButton("Clear", mainWidget);
    buttonsLayout->addWidget(clearButton);

    createButton = new QPushButton("Create", mainWidget);
    createButton->setEnabled(false);
    connect(createButton, &QPushButton::clicked, this, &NewElementWindow::createButton_Clicked);
    buttonsLayout->addWidget(createButton);


    //INit error palette
    red_pal.setColor(QPalette::Background, QColor::fromRgb(255, 0, 0));
    red_pal.setColor(QPalette::Text, QColor::fromRgb(255, 0, 0));

    //INit default palette
    def_pal.setColor(QPalette::Background, QColor::fromRgb(0, 100, 255));
    def_pal.setColor(QPalette::Text, QColor::fromRgb(255, 255, 255));


    //INit dangerous symbols array
    dangerous_symbols = new char[10];
    dangerous_symbols[0] = '<';
    dangerous_symbols[1] = '>';
    dangerous_symbols[3] = ':';
    dangerous_symbols[4] = '"';
    dangerous_symbols[5] = '/';
    dangerous_symbols[6] = '\\';
    dangerous_symbols[7] = '|';
    dangerous_symbols[8] = '?';
    dangerous_symbols[9] = '*';
}




/////Get methods

ListElement* NewElementWindow::getResult()
{
    return result;
}

//////////////
//////////////
//////////////







/////Signals handlers

void NewElementWindow::browseButton_Clicked()
{
    //User open a directory
    QFileDialog dialog(this);
    QString currentPath = dialog.getExistingDirectory();
    if(currentPath == "")
    {
        return;
    }
    pathTextBox->setText(currentPath);
}

void NewElementWindow::cancelButton_Clicked()
{
    close();
}

void NewElementWindow::createButton_Clicked()
{
    elementCreated();
    close();
}

void NewElementWindow::typeComboBox_ItemChanged()
{
    //Delete old GUI
    int cc = mainWidget->children().count();
    int rc = ruleLayout->count();

    for(int i = cc - 1; i > cc - rc; i--)
    {
        delete mainWidget->children().at(i);
    }
    ruleLabel->setText("");

    createButton->setEnabled(false);
    ruleLabel->setText(typeComboBox->currentText());

    //Create new GUI according the selected rule
    if(typeComboBox->currentText() == "Directory")
    {
        pathTextBox = new QTextEdit();
        connect(pathTextBox, &QTextEdit::textChanged, this, &NewElementWindow::checkTextBox);
        ruleLayout->addWidget(pathTextBox);

        browseButton = new QPushButton("Browse");
        connect(browseButton, &QPushButton::clicked, this, &NewElementWindow::browseButton_Clicked);
        ruleLayout->addWidget(browseButton);
        return;
    }
    else if(typeComboBox->currentText() == "Replace")
    {
        replacedTextBox = new QTextEdit();
        connect(replacedTextBox, &QTextEdit::textChanged, this, &NewElementWindow::checkTextBox);
        ruleLayout->addWidget(replacedTextBox);

        ruleLayout->addWidget(new QLabel("with"));

        replaceWithTextBox = new QTextEdit();
        connect(replaceWithTextBox, &QTextEdit::textChanged, this, &NewElementWindow::checkTextBox);
        ruleLayout->addWidget(replaceWithTextBox);
        return;
    }
    else if(typeComboBox->currentText() == "Remove")
    {
        removeTextBox = new QTextEdit();
        connect(removeTextBox, &QTextEdit::textChanged, this, &NewElementWindow::checkTextBox);
        ruleLayout->addWidget(removeTextBox);

        ruleLayout->addWidget(new QLabel("from"));

        fromTextBox = new QTextEdit();
        connect(fromTextBox, &QTextEdit::textChanged, this, &NewElementWindow::checkTextBox);
        ruleLayout->addWidget(fromTextBox);

        ruleLayout->addWidget(new QLabel("to"));

        toTextBox = new QTextEdit();
        connect(toTextBox, &QTextEdit::textChanged, this, &NewElementWindow::checkTextBox);
        ruleLayout->addWidget(toTextBox);
        return;
    }
    else if(typeComboBox->currentText() == "Add")
    {
        addTextBox = new QTextEdit();
        connect(addTextBox, &QTextEdit::textChanged, this, &NewElementWindow::checkTextBox);
        ruleLayout->addWidget(addTextBox);

        ruleLayout->addWidget(new QLabel("to"));

        toTextBox = new QTextEdit();
        connect(toTextBox, &QTextEdit::textChanged, this, &NewElementWindow::checkTextBox);
        ruleLayout->addWidget(toTextBox);
        return;
    }
    else if(typeComboBox->currentText() == "MakeList")
    {
        ruleLabel->setText("Make");
        positionComboBox = new QComboBox();
        positionComboBox->addItem("prefix");
        positionComboBox->addItem("postfix");
        positionComboBox->setCurrentIndex(0);
        ruleLayout->addWidget(positionComboBox);

        listType_ComboBox = new QComboBox();
        listType_ComboBox->addItem("numeric");
        listType_ComboBox->addItem("alphabetic");
        listType_ComboBox->setCurrentIndex(0);
        ruleLayout->addWidget(listType_ComboBox);

        ruleLayout->addWidget(new QLabel("list with"));
        ruleLayout->addWidget(new QLabel("separator"));

        addTextBox = new QTextEdit();
        ruleLayout->addWidget(addTextBox);
        connect(addTextBox, &QTextEdit::textChanged, this, &NewElementWindow::checkTextBox);

        return;
    }
}

//////////////
//////////////
//////////////







/////Other methods

void NewElementWindow::checkTextBox()
{
    if(typeComboBox->currentText() == "Replace")
    {
        //At least the replacedTextBox must have a value
        QString text = replacedTextBox->toPlainText();
        if(text.isNull() || text.isEmpty())
        {
            createButton->setEnabled(false);
            replacedTextBox->setPalette(red_pal);
            QRect rect(pos().x(), pos().y(), 150, 70);
            QToolTip::showText(replacedTextBox->mapToGlobal(replacedTextBox->pos()),
                               "This textbox must not be empty",
                               replacedTextBox, rect, 4000);
        }
        else
        {
            createButton->setEnabled(true);
            replacedTextBox->setPalette(def_pal);
            writeDataToResult();
            return;
        }
    }
    else if(typeComboBox->currentText() == "Remove")
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
            createButton->setEnabled(true);
            writeDataToResult();
            return;
        }

        QRect rect(0,0,0,0);
        QToolTip::showText(fromTextBox->mapToGlobal(fromTextBox->pos()),
                           "The range is wrong", fromTextBox, rect, 4000);
        createButton->setEnabled(false);
        return;
    }
    else if(typeComboBox->currentText() == "Add")
    {
        //Both textBoxes must have a value
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
                QString text = addTextBox->toPlainText();
                if(!text.isNull() && !text.isEmpty())
                {
                    toTextBox->setPalette(def_pal);
                    createButton->setEnabled(true);
                    writeDataToResult();
                    return;
                }
                else
                {
                    QRect rect(0,0,0,0);
                    QToolTip::showText(addTextBox->mapToGlobal(addTextBox->pos()),
                                       "This textbox must not be empty",
                                       addTextBox, rect, 4000);
                    addTextBox->setPalette(red_pal);
                    createButton->setEnabled(false);
                    return;
                }
            }
        }
        else
        {
            toTextBox->setPalette(red_pal);
        }

        QRect rect(0,0,0,0);
        QToolTip::showText(toTextBox->mapToGlobal(toTextBox->pos()),
                           "The position is wrong",
                           toTextBox, rect, 4000);

        createButton->setEnabled(false);
        return;
    }
    else if(typeComboBox->currentText() == "MakeList")
    {
        for(int i = 0; i < 10 ; i++)
        {
            if(addTextBox->toPlainText().contains(dangerous_symbols[i]))
            {
                addTextBox->setPalette(red_pal);
                QRect rect(0,0,0,0);
                QToolTip::showText(addTextBox->mapToGlobal(addTextBox->pos()),
                                   "Please, pay your attention to that some OS not support these symbols in files or/and directories names",
                                   addTextBox, rect, 10000);
                return;
            }
        }
        addTextBox->setPalette(def_pal);
        createButton->setEnabled(true);
        writeDataToResult();
        return;
    }
    else if(typeComboBox->currentText() == "Directory")
    {
        QDir dir(pathTextBox->toPlainText());
        if(!dir.exists())
        {
            createButton->setEnabled(false);
            pathTextBox->setPalette(red_pal);
            QRect rect(pos().x(), pos().y(), 150, 70);
            QToolTip::showText(pathTextBox->mapToGlobal(pathTextBox->pos()),
                               "The directory does not exist",
                               pathTextBox, rect, 4000);
            return;
        }

        pathTextBox->setPalette(def_pal);
        createButton->setEnabled(true);
        writeDataToResult();
        return;
    }
}

void NewElementWindow::writeDataToResult()
{
    //Rule_name arg arg0 arg1
    result->setName(typeComboBox->currentText());
    if(typeComboBox->currentText() == "Replace")
    {
        result->setInfo("Replace " + replacedTextBox->toPlainText()
                                    + " " + replaceWithTextBox->toPlainText());
    }
    else if(typeComboBox->currentText() == "Remove")
    {
        result->setInfo("Remove " + removeTextBox->toPlainText()
                                   + " " + fromTextBox->toPlainText()
                                   + " " + toTextBox->toPlainText());
    }
    else if(typeComboBox->currentText() == "Add")
    {
        result->setInfo("Add " + addTextBox->toPlainText()
                               + " " + toTextBox->toPlainText());
    }
    else if(typeComboBox->currentText() == "MakeList")
    {
        result->setInfo("MakeList " + positionComboBox->currentText()
                               + " " + listType_ComboBox->currentText()
                               +  addTextBox->toPlainText() + " as separator");
    }
    else if(typeComboBox->currentText() == "Directory")
    {
        result->setInfo(pathTextBox->toPlainText());
    }
}







