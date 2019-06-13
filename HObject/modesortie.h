#ifndef MODESORTIE_H
#define MODESORTIE_H
#include "labeled.h"


class MODESORTIE: public LABELED
{
public:
    MODESORTIE(QString id,QString label);
    MODESORTIE (MODESORTIE &other);
    MODESORTIE();
};

#endif // MODESORTIE_H
