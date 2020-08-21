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
#include <QMessageBox>
#include <QWidget>
#include <QGridLayout>
#include <QSizePolicy>
#include <QComboBox>
#include <QTextEdit>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void fixButtonClicked();
    void addRuleButtonClicked();
    void removeRuleButtonClicked();
    void applyButtonClicked();
    void ruleComboBoxTextChanged(const QString& text);
    QString fix_name(QString name);
    QString replace(QString* old, QString* args);

    void removeRow(QGridLayout *layout, int row, bool deleteWidgets);
    void remove(QGridLayout *layout, int row, int column, bool deleteWidgets);
    void deleteChildWidgets(QLayoutItem *item);

    QWidget* mainWidget;
    QGridLayout* mainGrid;

    QPushButton* fixButton;
    QPushButton* addRuleButton;

    QComboBox* ruleComboBox = nullptr;
    QTextEdit* replacedTextBox;
    QTextEdit* replaceWithTextBox;

    int rulesNumber = 0;
    QMap<QString, QString*> rules;
};
#endif // MAINWINDOW_H
