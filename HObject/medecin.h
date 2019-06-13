#ifndef MEDECIN_H
#define MEDECIN_H

#include "HObject/sante.h"

class MEDECIN: public SANTE
{
public:
    MEDECIN(int id, QString nom, QString prenom, sexe genre, NATIONALITE *nationalite,SM *statut, ADRESSE *adresse, QList<CONTACT*> contacts, QDate naissance, QString age);
    MEDECIN(MEDECIN const &other);
    MEDECIN();
};

#endif // MEDECIN_H
