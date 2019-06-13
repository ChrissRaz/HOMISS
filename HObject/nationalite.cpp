#include "nationalite.h"

NATIONALITE::NATIONALITE(int id,QString label): LABELED (id,label)
{

}

NATIONALITE::NATIONALITE(const NATIONALITE &other): LABELED (other)
{

}

NATIONALITE::NATIONALITE(): LABELED ()
{

}

NATIONALITE &NATIONALITE::operator=(NATIONALITE &other)
{
    if (this!=&other)
    {
        LABELED::operator=(other);
    }
    return *this;
}
