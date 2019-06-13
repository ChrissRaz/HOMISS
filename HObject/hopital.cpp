#include "hopital.h"

HOPITAL::HOPITAL(): LABELED ()
{
    this->setServices(QList<SERVICE *>());
}

HOPITAL::HOPITAL(QString id, QString nom, QList<SERVICE *> services): LABELED (id.toInt(),nom)
{
    this->setServices(services);
}

QList<SERVICE *> HOPITAL::services() const
{
    return m_services;
}

void HOPITAL::setServices(const QList<SERVICE *> &services)
{
    m_services = services;
}

QString HOPITAL::getNom()
{
    return  LABELED::getLabel();
}
