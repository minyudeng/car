#include "menu.h"

#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <QFile>

Menu::Menu(QWidget *parent) : QWidget(parent)
{
    initWidgets();
    setupUI();
    createConnections();
}

Menu::~Menu()
{
}

void Menu::onButtonClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());

    if (clickedButton == settingButton)
    {
        emit pageChangeRequested(2); // 发送信号到设置页面（索引2）
    }
    else if (clickedButton == dashboardButton)
    {
        emit pageChangeRequested(0); // 发送信号到仪表盘页面（索引0）
    }
}

void Menu::initWidgets()
{
    settingButton = new QPushButton("Settings");
    settingButton->setProperty("class", "menuButton");
    settingButton->setFixedSize(70, 70);

    dashboardButton = new QPushButton("Dashboard");
    dashboardButton->setProperty("class", "menuButton");
    dashboardButton->setFixedSize(70, 70);
}

void Menu::setupUI()
{
    // load style
    QFile styleFile(":/styles/menu.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text))
    {
        QString style = styleFile.readAll();
        this->setStyleSheet(style);
        qDebug() << "样式表menu加载成功!";
        styleFile.close();
    }
    else
    {
        qDebug() << "cannot open menu.qss:" << styleFile.errorString();
    }
    // set layout
    QGridLayout *layout = new QGridLayout(this);

    layout->setContentsMargins(40, 40, 60, 60);
    layout->setSpacing(0);

    QVBoxLayout *settingLayout = createButtonLayout(settingButton, "Settings");
    QVBoxLayout *dashboardLayout = createButtonLayout(dashboardButton, "Dashboard");

    layout->addLayout(settingLayout, 0, 0);
    layout->addLayout(dashboardLayout, 0, 1);
}

QVBoxLayout* Menu::createButtonLayout(QPushButton *button, const QString &labelText)
{
    QVBoxLayout *layout = new QVBoxLayout;
    // 设置布局的间距和边距
    layout->setSpacing(0); // 将垂直间距设为0
    layout->setContentsMargins(0, 0, 0, 0); // 移除布局的边距

    // 设置按钮大小策略
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->addWidget(button, 0, Qt::AlignCenter);

    QLabel *label = new QLabel(labelText);
    label->setProperty("class", "menuLabel");
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->addWidget(label, 0, Qt::AlignCenter);
    layout->addStretch(); // 添加弹性空间到底部

    return layout;
}

void Menu::createConnections()
{
    connect(settingButton, &QPushButton::clicked, this, &Menu::onButtonClicked);
    connect(dashboardButton, &QPushButton::clicked, this, &Menu::onButtonClicked);
}