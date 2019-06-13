#ifndef PERSONNE_H
#define PERSONNE_H

#include "HObject/adresse.h"
#include "HObject/contact.h"
#include "HObject/nationalite.h"

#include <QDate>

enum sexe{HOMME,FEMME};

namespace H
{
    class PERSONNE;
}

class PERSONNE: public HObject
{
public:
    PERSONNE(int id,QString nom,QString prenom,sexe genre,NATIONALITE *nationalite,ADRESSE *adresse,QList<CONTACT*> contacts,QDate naissance,QString age=QString());
    PERSONNE(PERSONNE const& other);
    PERSONNE();
    ~PERSONNE();

    QString nom() const;
    void setNom(const QString &nom);

    QString prenom() const;
    void setPrenom(const QString &prenom);

    QString age() const;
    void setAge(const QString &age);


    sexe genre() const;
    void setGenre(const sexe &genre);

    QDate naissance() const;
    void setNaissance(QDate naissance);


    PERSONNE &operator=(PERSONNE const &other);

    ADRESSE *adresse() const;
    void setAdresse(ADRESSE *adresse);


    NATIONALITE *nationalite() const;
    void setNationalite(NATIONALITE *nationalite);

    QList<CONTACT *> contacts() const;
    void setContacts(const QList<CONTACT *> &contacts);

private:
    bool ageCalculed;
protected:
    QString calculAge();
    QString m_nom,m_prenom,m_age;
    NATIONALITE *m_nationalite;
    sexe m_genre;
    QDate m_naissance;
    QList<CONTACT*> m_contacts;
    ADRESSE *m_adresse;
};


#endif // PERSONNE_H
