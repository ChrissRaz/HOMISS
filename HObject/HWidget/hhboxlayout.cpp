#include "hhboxlayout.h"

HHBoxLayout::HHBoxLayout(QString id, QWidget *parent): QHBoxLayout (parent)
{
    this->setId(id);
}

QString HHBoxLayout::getId() const
{
    return id;
}

void HHBoxLayout::setId(const QString &value)
{
    id = value;
}
