#include "examen.h"

EXAMEN::EXAMEN(): HObject ()
{
    this->setDate(QDateTime());
    this->setExaminateur(new MEDECIN);
    this->setPropos(QList<CLASSE *>());
}

EXAMEN::EXAMEN(QString id, QDateTime date, MEDECIN *examinateur, QList<CLASSE *> propos): HObject (id.toInt())
{
    this->setDate(date);
    this->setExaminateur(examinateur);
    this->setPropos(propos);
}

EXAMEN::~EXAMEN()
{
    delete examinateur();
}

EXAMEN &EXAMEN::operator=(EXAMEN &other)
{
    if(this!=&other)
    {
        HObject::operator=(other);
        this->setDate(other.date());
        delete m_examinateur;
        this->setPropos(other.propos());
    }
    return *this;
}

QList<CLASSE *> EXAMEN::propos() const
{
    return m_propos;
}

void EXAMEN::setPropos(const QList<CLASSE *> &propos)
{
    m_propos = propos;
}

QDateTime EXAMEN::date() const
{
    return m_date;
}

void EXAMEN::setDate(const QDateTime &date)
{
    m_date = date;
}

MEDECIN *EXAMEN::examinateur() const
{
    return m_examinateur;
}

void EXAMEN::setExaminateur(MEDECIN *examinateur)
{
    m_examinateur = examinateur;
}
