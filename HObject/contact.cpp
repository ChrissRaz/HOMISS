#include "contact.h"


CONTACT::CONTACT(int id, QString label): LABELED (id,label)
{

}

CONTACT::CONTACT(const CONTACT &other):LABELED(other)
{

}

CONTACT::CONTACT(): LABELED ()
{

}

CONTACT &CONTACT::operator=(CONTACT &other)
{
    if (this!=&other)
    {
        LABELED::operator=(other);
    }

    return *this;
}
