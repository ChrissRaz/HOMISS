#ifndef CONTACT_H
#define CONTACT_H

#include <HObject/labeled.h>

namespace H
{
    class CONTACT;
}

class CONTACT: public LABELED
{
public:
    CONTACT(int id,QString label);
    CONTACT(CONTACT const &other);
    CONTACT();

    CONTACT &operator=(CONTACT &other);

};

#endif // CONTACT_H
