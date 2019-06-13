#ifndef CLASSE_H
#define CLASSE_H

#include "HObject/hobject.h"


class CLASSE: public HObject
{
public:
    CLASSE();
    CLASSE(QString id,QString descriptif ,QStringList values,QString id_mere,QList<CLASSE *> subClasses);
    CLASSE(CLASSE &other);

    QStringList values() const;
    void setValues(const QStringList &values);

    QList<CLASSE *> subClasses() const;
    void setSubClasses(const QList<CLASSE *> &subClasses);

    QString descriptif() const;
    void setDescriptif(const QString &descriptif);

    QString id_mere() const;
    void setId_mere(const QString &id_mere);

protected:
    QStringList m_values;
    QString m_descriptif;
    QString m_id_mere;
    QList<CLASSE *> m_subClasses;
};

#endif // CLASSE_H
