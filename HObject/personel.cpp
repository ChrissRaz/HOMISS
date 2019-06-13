#include "personel.h"


PERSONEL::PERSONEL(int id, QString nom, QString prenom, sexe genre, NATIONALITE *nationalite, SM *statut, ADRESSE *adresse, QList<CONTACT *> contacts, QDate naissance, QString age):
    PERSONNE(id,nom,prenom,genre,nationalite,adresse,contacts,naissance,age)
{
    this->setStatut(new SM(*statut));
}

PERSONEL::PERSONEL(const PERSONEL &other): PERSONNE (other)
{
    this->setStatut(new SM(*other.getStatut()));
}

PERSONEL::PERSONEL(): PERSONNE ()
{
    this->setStatut(new SM());
}

SM* PERSONEL::getStatut() const
{
    return statut;
}

void PERSONEL::setStatut(SM *value)
{
    statut = value;
}

PERSONEL &PERSONEL::operator=(const PERSONEL &other)
{
    if (&other!=this)
    {
        PERSONNE::operator=(other);
        delete statut;
        this->setStatut(new SM(*other.getStatut()));
    }
    return *this;
}

