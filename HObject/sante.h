#ifndef SANTE_H
#define SANTE_H

#include "HObject/personel.h"


class SANTE: public PERSONEL
{
public:
    SANTE(int id, QString nom, QString prenom, sexe genre, NATIONALITE *nationalite,SM *statut, ADRESSE *adresse, QList<CONTACT*> contacts, QDate naissance, QString age);
    SANTE(SANTE const &other);
    SANTE();


};

#endif // SANTE_H
