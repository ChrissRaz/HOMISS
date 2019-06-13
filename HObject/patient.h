#ifndef PATIENT_H
#define PATIENT_H
#include "HObject/personne.h"
#include "HObject/medecin.h"
#include "HObject/statut.h"


namespace H {
class PATIENT;

}
class PATIENT: public PERSONNE
{
public:
    PATIENT(int id, QString nom, QString prenom, QString profession, sexe genre, NATIONALITE *nationalite ,STATUT* statut, ADRESSE *adresse, QList<CONTACT *> contacts, QDate naissance, QString age=QString());
    PATIENT(PATIENT const &other);
    PATIENT();
    ~PATIENT();

    QString getProffession() const;
    void setProffession(const QString &value);
    PATIENT &operator=(PATIENT const &other);


    STATUT *getStatut() const;
    void setStatut(STATUT *statut);

protected:
    QString proffession;
    STATUT *m_statut;
};

#endif // PATIENT_H
