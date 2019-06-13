#include "hobject.h"

HObject::HObject(int id)
{
    setNull(false);
    m_id=id;
}

HObject::HObject()
{
    setNull(true);
    m_id=-1;
}

HObject::~HObject()
{

}

int HObject::getId() const
{
    return m_id;
}

void HObject::setId(int value)
{
    m_id = value;
}

HObject &HObject::operator=(HObject &other)
{
    if (this!=&other)
    {
        this->setId(other.getId());
    }
    return *this;
}

bool HObject::isNull() const
{
    return null;
}

void HObject::setNull(bool value)
{
    null = value;
}

