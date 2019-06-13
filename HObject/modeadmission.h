#ifndef MODEADMISSION_H
#define MODEADMISSION_H

#include  "labeled.h"

class MODEADMISSION: public LABELED
{
public:
    MODEADMISSION(QString id,QString label);
    MODEADMISSION (MODEADMISSION &other);
    MODEADMISSION();

};

#endif // MODEADMISSION_H
