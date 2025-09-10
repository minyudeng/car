#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>

#include "components/dashboard.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUI();

    QStackedWidget* centralWidget;
    Dashboard* dashboard;
    QWidget* topBar;

};
#endif // MAINWINDOW_H
