#ifndef SM_H
#define SM_H

#include "HObject/labeled.h"

class SM: public LABELED
{
public:
    SM(int id,QString label);
    SM(SM const &other);
    SM();
};

#endif // SM_H
