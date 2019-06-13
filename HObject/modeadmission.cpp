#include "modeadmission.h"

MODEADMISSION::MODEADMISSION(QString id, QString label):  LABELED (id.toInt(),label)
{

}

MODEADMISSION::MODEADMISSION(MODEADMISSION &other): LABELED (other)
{

}

MODEADMISSION::MODEADMISSION(): LABELED ()
{

}
