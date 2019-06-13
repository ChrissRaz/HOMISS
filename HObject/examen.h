#ifndef EXAMEN_H
#define EXAMEN_H
#include "classe.h"
#include "medecin.h"

class EXAMEN : public HObject
{
public:
    EXAMEN();
    EXAMEN(QString id,QDateTime date,MEDECIN *exminateur,QList< CLASSE *> propos);
    ~EXAMEN();
    EXAMEN &operator=(EXAMEN &other);


    QList<CLASSE *> propos() const;
    void setPropos(const QList<CLASSE *> &propos);

    QDateTime date() const;
    void setDate(const QDateTime &date);

    MEDECIN *examinateur() const;
    void setExaminateur(MEDECIN *examinateur);

protected:
    QList<CLASSE *> m_propos;
    QDateTime m_date;
    MEDECIN * m_examinateur;

};

#endif // EXAMEN_H
