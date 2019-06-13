#include "infirmier.h"

INFIRMIER::INFIRMIER(int id, QString nom, QString prenom, sexe genre, NATIONALITE *nationalite, SM *statut, ADRESSE *adresse, QList<CONTACT*> contacts, QDate naissance, QString age):
    SANTE (id,nom,prenom,genre,nationalite,statut,adresse,contacts,naissance,age)
{

}

INFIRMIER::INFIRMIER(const INFIRMIER &other): SANTE(other)
{

}

INFIRMIER::INFIRMIER():SANTE ()
{

}
