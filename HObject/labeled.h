#ifndef LABELED_H
#define LABELED_H

#include "HObject/hobject.h"

namespace H {
class LABELED;
}

class LABELED: public HObject
{
public:
    LABELED(int id,QString &label);
    LABELED(LABELED const& other);
    LABELED();

    LABELED &operator=(LABELED &other);

    QString getLabel() const;
    void setLabel(const QString &value);


protected:
    QString Label;
};

#endif // LABELED_H
