#ifndef STATUT_H
#define STATUT_H

#include "HObject/labeled.h"

namespace H
{
class STATUT;
}

class STATUT: public LABELED
{
public:
    STATUT(int id,QString label);
    STATUT(STATUT const& other);
    STATUT();
};

#endif // STATUT_H
