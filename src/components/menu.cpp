#include "menu.h"

#include <QPushButton>

Menu::Menu(QWidget *parent) : QWidget(parent)
{
    QPushButton* button = new QPushButton("Menu Button", this);
}

Menu::~Menu()
{

}