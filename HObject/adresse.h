#ifndef ADRESSE_H
#define ADRESSE_H
#include "HObject/quartier.h"
namespace H
{
class ADRESSE;
}

class ADRESSE: public HObject
{
public:
    ADRESSE(int id, QString lot, QUARTIER *quartier);
    ADRESSE(ADRESSE const &other);
    ADRESSE();
    ~ADRESSE();

    QString lot() const;
    void setLot(const QString &lot);



    ADRESSE& operator=(ADRESSE const &other);

    QUARTIER *quartier() const;
    void setQuartier(QUARTIER *quartier);

protected:
    QString m_lot;
    QUARTIER *m_quartier;


};



#endif // ADRESSE_H
