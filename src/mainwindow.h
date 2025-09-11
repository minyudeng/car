#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QMouseEvent>
#include <QPoint>

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

    Dashboard* dashboard;
    Menu* menu;
    Setting* setting;

    QStackedWidget* centralWidget;
    QWidget* topBar;

    QPoint dragStartPosition;
    bool dragging;
    int swipeThreshold; // 滑动阈值

};
#endif // MAINWINDOW_H
