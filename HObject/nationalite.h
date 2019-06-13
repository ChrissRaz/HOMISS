#ifndef NATIONALITE_H
#define NATIONALITE_H
#include "labeled.h"

namespace H
{
class NATIONALITE;
}
class NATIONALITE: public LABELED
{
public:
    NATIONALITE(int id,QString label);
    NATIONALITE(NATIONALITE const &other);
    NATIONALITE();

    NATIONALITE & operator=(NATIONALITE &other);
};

#endif // NATIONALITE_H
