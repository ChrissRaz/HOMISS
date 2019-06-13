#include "fiche.h"

FICHE::FICHE(QString id, PATIENT *proprietaire, QDateTime date_admission, MODEADMISSION *mode_admission, MODESOIN *mode_soin, MEDECIN *responsable, QList<EXAMEN *> examens, SERVICE *provenance, SORTIE *sortie):HObject (id.toInt())
{
    this->setProprietire(proprietaire);
    this->setDate_admission(date_admission);
    this->setModeadmission(mode_admission);
    this->setModesoin(mode_soin);
    this->setSortie(sortie);
    this->setExamens(examens);
    this->setProvenance(provenance);

    this->setResponsble(responsable);
}

FICHE::FICHE(FICHE const&other): HObject (other.getId())
{

    this->setProprietire(new PATIENT(*other.proprietire()));
    this->setDate_admission(other.getDate_admission());
    this->setModeadmission(new MODEADMISSION(*other.modeadmission()));
    this->setModesoin(new MODESOIN(*other.modesoin()));
    this->setSortie(new SORTIE(*other.sortie()));
    this->setProvenance(new SERVICE(*other.getProvenance()));
    this->setExamens(other.getExamens());

    this->setResponsble(new MEDECIN(*other.responsble));
}

FICHE::FICHE(): HObject ()
{
    this->setProprietire(new PATIENT());
    this->setDate_admission(QDateTime());
    this->setModeadmission(new MODEADMISSION());
    this->setModesoin(new MODESOIN);
    this->setSortie(new SORTIE());
    this->setProvenance(new SERVICE());
    this->setExamens(QList <EXAMEN *>());

    this->setResponsble(new MEDECIN());
}

FICHE &FICHE::operator=(FICHE &other)
{
    if (this!=&other)
    {
        delete m_proprietire;
        delete m_modeadmission;
        delete m_modesoin;
        delete m_sortie;
        delete provenance;


        this->setProprietire(new PATIENT(*other.proprietire()));
        this->setDate_admission(other.getDate_admission());
        this->setModeadmission(new MODEADMISSION(*other.modeadmission()));
        this->setModesoin(new MODESOIN(*other.modesoin()));
        this->setSortie(new SORTIE(*other.sortie()));
        this->setProvenance(new SERVICE(*other.getProvenance()));
        this->setExamens(other.getExamens());

        this->setResponsble(new MEDECIN(*other.getResponsble()));
    }

    return  *this;
}

FICHE::~FICHE()
{
    delete m_proprietire;
    delete m_modeadmission;
    delete m_modesoin;
    delete m_sortie;

}

PATIENT *FICHE::proprietire() const
{
    return m_proprietire;
}

void FICHE::setProprietire(PATIENT *proprietire)
{
    m_proprietire = proprietire;
}

MODESOIN *FICHE::modesoin() const
{
    return m_modesoin;
}

void FICHE::setModesoin(MODESOIN *modesoin)
{
    m_modesoin = modesoin;
}

SORTIE *FICHE::sortie() const
{
    return m_sortie;
}

void FICHE::setSortie(SORTIE *sortie)
{
    m_sortie = sortie;
}

MODEADMISSION *FICHE::modeadmission() const
{
    return m_modeadmission;
}

void FICHE::setModeadmission(MODEADMISSION *modeadmission)
{
    m_modeadmission = modeadmission;
}

QDateTime FICHE::getDate_admission() const
{
    return date_admission;
}

void FICHE::setDate_admission(const QDateTime &value)
{
    date_admission = value;
}

QList<EXAMEN *> FICHE::getExamens() const
{
    return m_examens;
}

void FICHE::setExamens(const QList<EXAMEN *> &examens)
{
    m_examens = examens;
}

SERVICE *FICHE::getProvenance() const
{
    return provenance;
}

void FICHE::setProvenance(SERVICE *value)
{
    provenance = value;
}


MEDECIN *FICHE::getResponsble() const
{
    return responsble;
}

void FICHE::setResponsble(MEDECIN *value)
{
    responsble = value;
}
