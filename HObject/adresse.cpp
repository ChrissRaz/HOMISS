#include "adresse.h"


ADRESSE::ADRESSE(int id, QString lot, QUARTIER *quartier): HObject(id)
{
    this->setLot(lot);
    this->setQuartier( new QUARTIER(*quartier));
}

ADRESSE::ADRESSE(const ADRESSE &other):HObject (other.getId())
{
    this->setLot(other.lot());
    this->setQuartier(new QUARTIER(*other.quartier()));

}

ADRESSE::ADRESSE(): HObject ()
{
    this->setLot("");
    this->setQuartier( new QUARTIER());
}

ADRESSE::~ADRESSE()
{
    delete m_quartier;
}



QString ADRESSE::lot() const
{
    return m_lot;
}

void ADRESSE::setLot(const QString &lot)
{
    m_lot = lot;
}



ADRESSE& ADRESSE::operator=(const ADRESSE &other)
{
    if (this!=&other)
    {
        this->setId(other.getId());
        this->setLot(other.lot());
        delete m_quartier;
        this->setQuartier(new QUARTIER(*other.quartier()));
    }
    return *this;
}

QUARTIER *ADRESSE::quartier() const
{
    return m_quartier;
}

void ADRESSE::setQuartier(QUARTIER *quartier)
{
    m_quartier = quartier;
}


