#include "setting.h"

#include <QPushButton>

Setting::Setting(QWidget *parent) : QWidget(parent)
{
    QPushButton* btn = new QPushButton("Setting Button", this);
}

Setting::~Setting()
{

}