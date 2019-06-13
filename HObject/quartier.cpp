#include "quartier.h"


QUARTIER::QUARTIER(int id, QString nom, VILLE *ville) : HObject (id)
{
    this->setNom(nom);
    this->setVille(ville);
}

QUARTIER::QUARTIER(const QUARTIER &other):HObject (other.getId())
{
    this->setNom(other.nom());
    this->setVille(new VILLE(*other.ville()));
}

QUARTIER::QUARTIER(): HObject ()
{
    this->setNom("");
    this->setVille(new VILLE());
}

QUARTIER::~QUARTIER()
{
    delete m_ville;
}


QString QUARTIER::nom() const
{
    return m_nom;
}

void QUARTIER::setNom(const QString &nom)
{
    m_nom = nom;
}


QUARTIER &QUARTIER::operator=(const QUARTIER &other)
{
    if (this!=&other)
    {
        this->setId(other.getId());
        this->setNom(other.nom());
        delete m_ville;
        this->setVille(new  VILLE(*other.ville()));
    }
    return *this;
}

VILLE *QUARTIER::ville() const
{
    return m_ville;
}

void QUARTIER::setVille(VILLE *ville)
{
    m_ville = ville;
}
