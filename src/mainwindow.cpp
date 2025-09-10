#include "mainwindow.h"

#include <QVBoxLayout>
#include <QScreen>
#include <QRect>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Car System");
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    setMinimumSize(screenGeometry.width() * 0.8, screenGeometry.height() * 0.8);

    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    centralWidget = new QStackedWidget(this);
    setCentralWidget(centralWidget);

    dashboard = new Dashboard(this);
    centralWidget->addWidget(dashboard);


    centralWidget->setCurrentWidget(dashboard);
}


