#ifndef PERSONEL_H
#define PERSONEL_H

#include "HObject/personne.h"
#include "HObject/sm.h"

namespace H {
class PERSONEL;
}


class PERSONEL: public PERSONNE
{
public:
    PERSONEL(int id, QString nom, QString prenom, sexe genre, NATIONALITE *nationalite,SM *statut, ADRESSE *adresse, QList<CONTACT*> contacts, QDate naissance, QString age);
    PERSONEL(PERSONEL const &other);
    PERSONEL();

    SM *getStatut() const;

    void setStatut(SM *value);

    PERSONEL & operator=(PERSONEL const &other);

protected:
    SM *statut;
};

#endif // PERSONEL_H
