#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QMouseEvent>

#include "components/dashboard.h"
#include "components/menu.h"
#include "components/setting.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:

private:
    void setupUI();
    void slideToIndex(int newIndex);

    Dashboard* dashboard;
    Menu* menu;
    Setting* setting;

    QStackedWidget* centralWidget;
    QWidget* topBar;

    int startPosX;
    int endPosX;
    bool dragging = false;
    int swipeThreshold;

};
#endif // MAINWINDOW_H
