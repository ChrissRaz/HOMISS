#include "labeled.h"

LABELED::LABELED(int id, QString &label):HObject (id)
{
    this->setLabel(label);
}

LABELED::LABELED(const LABELED &other):HObject (other.getId())
{
    this->setLabel(other.Label);
}

LABELED::LABELED(): HObject ()
{
    this->setLabel("");
}

LABELED &LABELED::operator=(LABELED &other)
{
    if (this!=&other)
    {
        HObject::operator=(other);
        this->setLabel(other.getLabel());
    }
    return  *this;
}


QString LABELED::getLabel() const
{
    return Label;
}

void LABELED::setLabel(const QString &value)
{
    Label = value;
}
