#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QPushButton>
#include <QApplication>
#include <QFileInfo>
#include <QString>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QPushButton* butt;
    void Do();
    QString fix_name(QString name);
};
#endif // MAINWINDOW_H
