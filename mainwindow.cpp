#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->butt = new QPushButton("Click");
    connect(butt, &QPushButton::clicked, this, &MainWindow::Do);
    setCentralWidget(butt);
}

MainWindow::~MainWindow()
{
}


void MainWindow::Do()
{
    QFileDialog dialog(this);
    QString path = dialog.getExistingDirectory();
    QDir dir(path);
    QStringList path_list = dir.entryList();
    for(uint i = 0; i < dir.count(); i++)
    {
        QFile::rename(path + "/" + path_list[i], path + "/" + fix_name(path_list[i]));
    }
}

QString MainWindow::fix_name(QString name)
{
    QString res = name.replace(0, 5, "");
    res = res.replace(' ', '_');
    return res;
}
