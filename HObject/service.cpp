#include "service.h"

SERVICE::SERVICE(int id, QString nom, QList<MEDECIN *> medecins, QList<INFIRMIER *> infirmiers, QString nomHopital, MEDECIN *chef, MEDECIN *major): LABELED (id,nom)
{
    this->setChef(new MEDECIN(*chef));
    this->setMajor(new MEDECIN(*major));
    this->setMedecins(medecins);
    this->setInfirmiers(infirmiers);
    this->setNomHopital(nomHopital);
}

SERVICE::SERVICE(int id, QString nom, QString nomHopital, MEDECIN *chef, MEDECIN *major): LABELED (id,nom)
{
    this->setChef(new MEDECIN(*chef));
    this->setMajor(new MEDECIN(*major));
    this->setMedecins(QList<MEDECIN *> ());
    this->setInfirmiers(QList<INFIRMIER *>());
    this->setNomHopital(nomHopital);
}

SERVICE::SERVICE(const SERVICE &other): LABELED (other)
{
    this->setChef(new MEDECIN(*other.chef()));
    this->setMajor(new MEDECIN(*other.major()));
    this->setMedecins(other.medecins());
    this->setInfirmiers(other.infirmiers());
    this->setNomHopital(other.nomHopital());
}

SERVICE::SERVICE(): LABELED ()
{
    this->setMajor(new MEDECIN());
    this->setChef(new MEDECIN());
    this->setMedecins(QList<MEDECIN *> ());
    this->setInfirmiers(QList<INFIRMIER *>());
    this->setNomHopital("");
}

QString SERVICE::nom()
{
    return LABELED::getLabel();
}

void SERVICE::setNom(QString &nom)
{
    LABELED::setLabel(nom);
}

SERVICE &SERVICE::operator=(SERVICE &other)
{
    if (this!=&other)
    {
        LABELED::operator=(other);
        delete m_major;
        delete m_chef;
        this->setChef(new MEDECIN(*other.chef()));
        this->setMajor(new MEDECIN(*other.major()));
        this->setMedecins(other.medecins());
        this->setInfirmiers(other.infirmiers());
        this->setNomHopital(other.nomHopital());

    }
    return  *this;
}

MEDECIN *SERVICE::major() const
{
    return m_major;
}

void SERVICE::setMajor(MEDECIN *major)
{
    m_major = major;
}

MEDECIN *SERVICE::chef() const
{
    return m_chef;
}

void SERVICE::setChef(MEDECIN *chef)
{
    m_chef = chef;
}

QList<MEDECIN *> SERVICE::medecins() const
{
    return m_medecins;
}

void SERVICE::setMedecins(const QList<MEDECIN *> &medecins)
{
    m_medecins = medecins;
}

QList<INFIRMIER *> SERVICE::infirmiers() const
{
    return m_infirmiers;
}

void SERVICE::setInfirmiers(const QList<INFIRMIER *> &infirmiers)
{
    m_infirmiers = infirmiers;
}

QString SERVICE::nomHopital() const
{
    return m_nomHopital;
}

void SERVICE::setNomHopital(const QString &nomHopital)
{
    m_nomHopital = nomHopital;
}



