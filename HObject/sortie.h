#ifndef SORTIE_H
#define SORTIE_H
#include "hobject.h"
#include "modesortie.h"
#include "service.h"

class SORTIE : public HObject
{
public:
    SORTIE(QString id,QDateTime date,MODESORTIE *mode,SERVICE *destination);
    SORTIE(SORTIE &other);
    SORTIE();

    MODESORTIE *mode() const;
    void setMode(MODESORTIE *mode);


    SERVICE *getDestination() const;
    void setDestination(SERVICE *value);

    QDateTime getDate() const;
    void setDate(const QDateTime &date);

    SORTIE &operator=(SORTIE &other);

protected:
    MODESORTIE *m_mode;
    QDateTime m_date;
    SERVICE *m_destination;
};


#endif // SORTIE_H
