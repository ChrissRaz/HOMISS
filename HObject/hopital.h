#ifndef HOPITAL_H
#define HOPITAL_H

#include "service.h"
#include "labeled.h"

class HOPITAL: public LABELED
{
public:
    HOPITAL();
    HOPITAL(QString id,QString nom,QList<SERVICE *> services);

    QList<SERVICE *> services() const;
    void setServices(const QList<SERVICE *> &services);

    QString getNom();

protected:
    QList<SERVICE *> m_services;
};

#endif // HOPITAL_H
