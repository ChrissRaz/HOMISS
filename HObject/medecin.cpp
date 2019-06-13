#include "medecin.h"
MEDECIN::MEDECIN(int id, QString nom, QString prenom, sexe genre, NATIONALITE *nationalite, SM *statut, ADRESSE *adresse, QList<CONTACT*> contacts, QDate naissance, QString age):
    SANTE (id,nom,prenom,genre,nationalite,statut,adresse,contacts,naissance,age)
{

}

MEDECIN::MEDECIN(const MEDECIN &other): SANTE(other)
{

}

MEDECIN::MEDECIN(): SANTE ()
{

}
