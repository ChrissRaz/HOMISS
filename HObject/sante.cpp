#include "sante.h"


SANTE::SANTE(int id, QString nom, QString prenom, sexe genre, NATIONALITE *nationalite, SM *statut, ADRESSE *adresse, QList<CONTACT*> contacts, QDate naissance, QString age):
    PERSONEL (id,nom,prenom,genre,nationalite,statut,adresse,contacts,naissance,age)
{

}

SANTE::SANTE(const SANTE &other): PERSONEL (other)
{

}

SANTE::SANTE(): PERSONEL ()
{

}
