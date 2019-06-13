#include "hlineedit.h"

HLineEdit::HLineEdit(QString id, QWidget *parent):QLineEdit (parent)
{
    this->setId(id);
}

QString HLineEdit::getId() const
{
    return id;
}

void HLineEdit::setId(const QString &value)
{
    id = value;
}
