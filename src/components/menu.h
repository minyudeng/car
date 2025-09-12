#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include "dashboard.h"
#include "setting.h"

class Menu : public QWidget
{
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

signals:
    void pageChangeRequested(int index);

private slots:
    void onButtonClicked();

private:
    void initWidgets();
    void setupUI();
    QVBoxLayout* createButtonLayout(QPushButton* button, const QString& labelText);
    void createConnections();

private:
    QPushButton* settingButton;
    QPushButton* dashboardButton;

private:
    Dashboard* dashboard;
    Setting* setting;
};






#endif // MENU_H