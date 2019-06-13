#include "statut.h"

STATUT::STATUT(int id, QString label): LABELED (id,label)
{

}

STATUT::STATUT(const STATUT &other): LABELED (other)
{

}

STATUT::STATUT(): LABELED ()
{

}
