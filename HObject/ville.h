#ifndef VILLE_H
#define VILLE_H

#include "HObject/hobject.h"

namespace H
{
    class VILLE;
}

class VILLE: public HObject
{
public:
    VILLE(int id,QString nom);
    VILLE(VILLE const &other);
    VILLE();
    ~VILLE();

    QString nom() const;
    void setNom(const QString &nom);

    VILLE& operator=(VILLE const &other);

protected:
    QString m_nom;
};

#endif // VILLE_H
