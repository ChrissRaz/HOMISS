#include "sortie.h"

SORTIE::SORTIE(QString id, QDateTime date, MODESORTIE *mode, SERVICE *destination): HObject (id.toInt())
{
    this->setDate(date);
    this->setDestination(destination);
    this->setMode(mode);
}

SORTIE::SORTIE(SORTIE &other): HObject (other.getId())
{
    this->setDate(other.getDate());
    this->setDestination(other.getDestination());
    this->setMode(other.mode());
}

SORTIE::SORTIE():HObject ()
{
    this->setDate(QDateTime());
    this->setDestination(new SERVICE());
    this->setMode(new MODESORTIE());
}

MODESORTIE *SORTIE::mode() const
{
    return m_mode;
}

void SORTIE::setMode(MODESORTIE *mode)
{
    m_mode = mode;
}


SERVICE *SORTIE::getDestination() const
{
    return m_destination;
}

void SORTIE::setDestination(SERVICE *value)
{
    m_destination = value;
}

QDateTime SORTIE::getDate() const
{
    return m_date;
}

void SORTIE::setDate(const QDateTime &date)
{
    m_date = date;
}

SORTIE &SORTIE::operator=(SORTIE &other)
{
    if (this!=&other)
    {
        HObject::operator=(other);

        delete m_destination;
        delete m_mode;
        this->setDate(other.getDate());
        this->setDestination(other.getDestination());
        this->setMode(other.mode());
    }
    return *this;
}
