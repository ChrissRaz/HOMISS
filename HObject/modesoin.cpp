#include "modesoin.h"

MODESOIN::MODESOIN(QString id, QString label):  LABELED (id.toInt(),label)
{

}

MODESOIN::MODESOIN(MODESOIN &other): LABELED (other)
{

}

MODESOIN::MODESOIN(): LABELED ()
{

}
