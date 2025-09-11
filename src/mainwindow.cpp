#include "mainwindow.h"

#include <QVBoxLayout>
#include <QScreen>
#include <QRect>
#include <QDebug>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Car System");
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    this->setObjectName("mainWindow");
    setStyleSheet("#mainWindow { border-image: url(:/images/th.jpg); }");

    swipeThreshold = screenGeometry.width() / 4;

    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        startPosX = event->globalPosition().x();
        dragging = true;
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {

    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (dragging && event->button() == Qt::LeftButton) {
        endPosX = event->globalPosition().x();
        int deltaX = endPosX - startPosX;
        if (qAbs(deltaX) > swipeThreshold) {
            if (deltaX > 0) {
                // 向右滑动
                int newIndex = centralWidget->currentIndex() - 1;
                slideToIndex(newIndex);
            } else {
                // 向左滑动
                int newIndex = centralWidget->currentIndex() + 1;
                slideToIndex(newIndex);
            }
        }
        dragging = false;
    }
    QMainWindow::mouseReleaseEvent(event);
}

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

void MainWindow::slideToIndex(int newIndex) {
    if (newIndex < 0 || newIndex >= centralWidget->count() || newIndex == centralWidget->currentIndex())
        return;

    int direction = (newIndex > centralWidget->currentIndex()) ? 1 : -1;
    int width = centralWidget->width();

    QWidget *currentWidget = centralWidget->currentWidget();
    QWidget *nextWidget = centralWidget->widget(newIndex);
    nextWidget->setGeometry(direction * width, 0, width, centralWidget->height());
    nextWidget->show();

    // 动画效果
    QPropertyAnimation *currentAnim = new QPropertyAnimation(currentWidget, "geometry");
    currentAnim->setDuration(300);
    currentAnim->setStartValue(currentWidget->geometry());
    currentAnim->setEndValue(QRect(-direction * width, 0, width, centralWidget->height()));

    QPropertyAnimation *nextAnim = new QPropertyAnimation(nextWidget, "geometry");
    nextAnim->setDuration(300);
    nextAnim->setStartValue(nextWidget->geometry());
    nextAnim->setEndValue(QRect(0, 0, width, centralWidget->height()));

    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    group->addAnimation(currentAnim);
    group->addAnimation(nextAnim);

    connect(group, &QParallelAnimationGroup::finished, [this, nextWidget]() {
        centralWidget->setCurrentWidget(nextWidget);
        nextWidget->setGeometry(0, 0, centralWidget->width(), centralWidget->height());
        // 清理动画对象
        QParallelAnimationGroup *group = qobject_cast<QParallelAnimationGroup *>(sender());
        if (group) {
            group->deleteLater();
        }
    });

    group->start();
}



