#include "modesortie.h"

MODESORTIE::MODESORTIE(QString id, QString label):  LABELED (id.toInt(),label)
{

}

MODESORTIE::MODESORTIE(MODESORTIE &other): LABELED (other)
{

}

MODESORTIE::MODESORTIE(): LABELED ()
{

}
