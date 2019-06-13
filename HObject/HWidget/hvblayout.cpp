#include "hvblayout.h"

HVBLayout::HVBLayout(QString id, QWidget *parent):QVBoxLayout (parent)
{
    this->setId(id);
}

QString HVBLayout::getId() const
{
    return id;
}

void HVBLayout::setId(const QString &value)
{
    id = value;
}
