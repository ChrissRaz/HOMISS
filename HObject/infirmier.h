#ifndef INFIRMIER_H
#define INFIRMIER_H

#include "HObject/sante.h"


class INFIRMIER: public SANTE
{
public:
    INFIRMIER(int id, QString nom, QString prenom, sexe genre, NATIONALITE *nationalite,SM *statut, ADRESSE *adresse, QList<CONTACT*> contacts, QDate naissance, QString age);
    INFIRMIER(INFIRMIER const &other);
    INFIRMIER();
};

#endif // INFIRMIER_H
