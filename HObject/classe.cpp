#include "classe.h"

CLASSE::CLASSE(): HObject ()
{
    this->setValues(QStringList());
    this->setSubClasses(QList<CLASSE *>());
    this->setDescriptif("");
    this->setId_mere("");
}

CLASSE::CLASSE(QString id, QString descriptif, QStringList values, QString id_mere, QList<CLASSE *> subClasses):HObject(id.toInt())
{
    this->setValues(values);
    this->setSubClasses(subClasses);
    this->setDescriptif(descriptif);
    this->setId_mere(id_mere);
}

CLASSE::CLASSE(CLASSE &other): HObject (other.getId())
{
    this->setValues(other.values());
    this->setSubClasses(other.subClasses());
    this->setDescriptif(other.descriptif());
    this->setId_mere(other.id_mere());
}

QStringList CLASSE::values() const
{
    return m_values;
}

void CLASSE::setValues(const QStringList &values)
{
    m_values = values;
}

QList<CLASSE *> CLASSE::subClasses() const
{
    return m_subClasses;
}

void CLASSE::setSubClasses(const QList<CLASSE *> &subClasses)
{
    m_subClasses = subClasses;
}

QString CLASSE::descriptif() const
{
    return m_descriptif;
}

void CLASSE::setDescriptif(const QString &descriptif)
{
    m_descriptif = descriptif;
}

QString CLASSE::id_mere() const
{
    return m_id_mere;
}

void CLASSE::setId_mere(const QString &id_mere)
{
    m_id_mere = id_mere;
}
