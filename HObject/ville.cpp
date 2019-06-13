#include "ville.h"

VILLE::VILLE(int id, QString nom):HObject (id)
{
    this->setNom(nom);
}

VILLE::VILLE(const VILLE &other):HObject (other.getId())
{
    this->setNom(other.nom());
}

VILLE::VILLE(): HObject ()
{
    this->setNom("");
}

VILLE::~VILLE()
{

}

QString VILLE::nom() const
{
    return m_nom;
}

void VILLE::setNom(const QString &nom)
{
    m_nom = nom;
}


VILLE &VILLE::operator=(const VILLE &other)
{
    if(this!=&other)
    {
        this->setId(other.getId());
        this->setNom(other.nom());
    }
    return *this;
}

