#include "mainwindow.h"

#include <QVBoxLayout>
#include <QScreen>
#include <QRect>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Car System");
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    this->setObjectName("mainWindow");
    setStyleSheet("#mainWindow { border-image: url(:/images/th.jpg); }");

    swipeThreshold = screenGeometry.width() / 4; // 设置滑动阈值为屏幕宽度的四分之一

    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    centralWidget = new QStackedWidget(this);
    setCentralWidget(centralWidget);

    dashboard = new Dashboard(this);
    centralWidget->addWidget(dashboard);

    menu = new Menu(this);
    centralWidget->addWidget(menu);

    setting = new Setting(this);
    centralWidget->addWidget(setting);


    centralWidget->setCurrentWidget(dashboard);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
    {
        dragStartPosition = event->pos();
        dragging = true;
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (dragging)
    {
        /* code */
    }
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && dragging)
    {
        QPoint dragEndPosition = event->pos();
        int dx = dragEndPosition.x() - dragStartPosition.x();

        // 只处理水平滑动
        if (abs(dx) > swipeThreshold) {
            if (dx > 0) {
                // 向右滑动 - 切换到前一页
                int currentIndex = centralWidget->currentIndex();
                if (currentIndex > 0) {
                    centralWidget->setCurrentIndex(currentIndex - 1);
                }
            } else {
                // 向左滑动 - 切换到后一页
                int currentIndex = centralWidget->currentIndex();
                if (currentIndex < centralWidget->count() - 1) {
                    centralWidget->setCurrentIndex(currentIndex + 1);
                }
            }
        }

        dragging = false;
    }
    QMainWindow::mouseReleaseEvent(event);
}



