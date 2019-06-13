#ifndef MODESOIN_H
#define MODESOIN_H
#include "labeled.h"

class MODESOIN: public LABELED
{
public:
    MODESOIN(QString id,QString label);
    MODESOIN (MODESOIN &other);
    MODESOIN();
};

#endif // MODESOIN_H
