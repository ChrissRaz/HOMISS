#ifndef SERVICE_H
#define SERVICE_H

#include "HObject/labeled.h"
#include "HObject/medecin.h"
#include "HObject/infirmier.h"

namespace H
{
class SERVICE;
}

class SERVICE: public LABELED
{
public:
    SERVICE(int id, QString nom,QList< MEDECIN *> medecins,QList<INFIRMIER*> infirmiers,QString nomHopital,MEDECIN *chef= new MEDECIN(), MEDECIN *major=new MEDECIN());
    SERVICE(int id, QString nom,QString nomHopital,MEDECIN *chef= new MEDECIN(), MEDECIN *major=new MEDECIN());
    SERVICE(SERVICE const& other);
    SERVICE();
    QString nom();
    void setNom(QString &nom);
    SERVICE &operator=(SERVICE &other);
    MEDECIN *major() const;
    void setMajor(MEDECIN *major);

    MEDECIN *chef() const;
    void setChef(MEDECIN *chef);



    QList<MEDECIN *> medecins() const;
    void setMedecins(const QList<MEDECIN *> &medecins);

    QList<INFIRMIER *> infirmiers() const;
    void setInfirmiers(const QList<INFIRMIER *> &infirmiers);

    QString nomHopital() const;
    void setNomHopital(const QString &nomHopital);

protected:
    MEDECIN *m_major,*m_chef;
    QList< MEDECIN *> m_medecins;
    QList <INFIRMIER *> m_infirmiers;
    QString m_nomHopital;
};

#endif // SERVICE_H
