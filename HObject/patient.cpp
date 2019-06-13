#include "patient.h"


PATIENT::PATIENT(int id, QString nom, QString prenom, QString profession, sexe genre, NATIONALITE *nationalite ,STATUT* statut, ADRESSE *adresse, QList<CONTACT *> contacts, QDate naissance, QString age):
    PERSONNE(id,nom,prenom,genre,nationalite,adresse,contacts,naissance,age)
{
    this->setProffession(profession);
    this->setStatut(statut);
}

PATIENT::PATIENT (PATIENT const &other):PERSONNE (other)
{
    this->setProffession(other.getProffession());
    this->setStatut(other.getStatut());
}

PATIENT::PATIENT(): PERSONNE ()
{
    this->setProffession("");
    this->setStatut(new STATUT());
}

PATIENT::~PATIENT()
{

}


QString PATIENT::getProffession() const
{
    return proffession;
}

void PATIENT::setProffession(const QString &value)
{
    proffession = value;
}


PATIENT &PATIENT::operator=(const PATIENT &other)
{
    if (this!=&other)
    {
        PERSONNE::operator=(other);
        this->setProffession(other.getProffession());
        delete m_statut;
        this->setStatut(new STATUT(*other.getStatut()));
    }
    return  *this;

}


STATUT *PATIENT::getStatut() const
{
    return m_statut;
}

void PATIENT::setStatut(STATUT *statut)
{
    m_statut = statut;
}

