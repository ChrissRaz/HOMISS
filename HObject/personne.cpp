#include "personne.h"


PERSONNE::PERSONNE(int id, QString nom, QString prenom, sexe genre, NATIONALITE *nationalite, ADRESSE *adresse, QList<CONTACT *> contacts, QDate naissance, QString age): HObject (id)
{
    ageCalculed=false;
    this->setNom(nom);
    this->setPrenom(prenom);
    this->setGenre(genre);
    this->setNationalite(nationalite);
    this->setAdresse(new ADRESSE(*adresse));
    this->setContacts(contacts);
    this->setNaissance(naissance);
    this->setAge(age);


}

PERSONNE::PERSONNE(const PERSONNE &other): HObject (other.getId())
{
    this->setNom(other.nom());
    this->setPrenom(other.prenom());
    this->setGenre(other.genre());
    this->setNationalite(new NATIONALITE(*other.nationalite()));
    this->setContacts(other.contacts());
    this->setNaissance(other.naissance());
    this->setAge(other.age());
    this->setAdresse(new ADRESSE(*other.adresse()));
}

PERSONNE::PERSONNE(): HObject ()
{
    ageCalculed=false;
    this->setNom("");
    this->setPrenom("");
    this->setGenre(sexe::HOMME);
    this->setNationalite(new NATIONALITE());
    this->setAdresse(new ADRESSE());
    this->setContacts(QList<CONTACT*>());
    this->setNaissance(QDate());
    this->setAge("");
}

PERSONNE::~PERSONNE()
{
    delete m_adresse;
    delete m_nationalite;
}


QString PERSONNE::nom() const
{
    return m_nom;
}

void PERSONNE::setNom(const QString &nom)
{
    m_nom = nom;
}

QString PERSONNE::prenom() const
{
    return m_prenom;
}

void PERSONNE::setPrenom(const QString &prenom)
{
    m_prenom = prenom;
}

QString PERSONNE::age() const
{
    return m_age;
}

void PERSONNE::setAge(const QString &age)
{
    if (this->naissance().isValid()&& !ageCalculed)
    {
        m_age=calculAge();
        ageCalculed=true;
    }
    else
    {
        m_age = age;
    }

}


sexe PERSONNE::genre() const
{
    return m_genre;
}

void PERSONNE::setGenre(const sexe &genre)
{
    m_genre = genre;
}

QDate PERSONNE::naissance() const
{
    return m_naissance;
}

void PERSONNE::setNaissance(QDate naissance)
{
    m_naissance = naissance;
}


PERSONNE &PERSONNE::operator=(const PERSONNE &other)
{
    if (this!=&other)
    {
        ageCalculed=false;
        this->setId(other.getId());
        this->setNom(other.nom());
        this->setPrenom(other.prenom());
        this->setGenre(other.genre());
        this->setNaissance(QDate(other.naissance()));
        delete  m_adresse;
        this->setAdresse(new ADRESSE(*other.adresse()));
        this->setAge(other.age());
        this->setContacts(QList<CONTACT*>(other.contacts()));
    }
    return *this;
}

QString PERSONNE::calculAge()
{
    ageCalculed=true;
    int res;
    if (this->naissance().isValid())
    {
        res=QDate::currentDate().year()-naissance().year();
        int m= QDate::currentDate().month()-naissance().month();

        if (m==0)
        {
            int j= QDate::currentDate().day()-naissance().day();

            if (j<0)
            {
                res--;
            }
        }
        else if(m<0)
        {
            res--;
        }
        return QString::number(res);
    }
    else
    {
        return QString();
    }
}

QList<CONTACT *> PERSONNE::contacts() const
{
    return m_contacts;
}

void PERSONNE::setContacts(const QList<CONTACT *> &contacts)
{
    m_contacts = contacts;
}

NATIONALITE *PERSONNE::nationalite() const
{
    return m_nationalite;
}

void PERSONNE::setNationalite(NATIONALITE *nationalite)
{
    m_nationalite = nationalite;
}


ADRESSE *PERSONNE::adresse() const
{
    return m_adresse;
}

void PERSONNE::setAdresse(ADRESSE *adresse)
{
    m_adresse = adresse;
}
