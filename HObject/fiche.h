#ifndef FICHE_H
#define FICHE_H
#include "patient.h"
#include "examen.h"
#include "sortie.h"
#include "modeadmission.h"
#include "modesoin.h"


namespace H {
    class FICHE;
}

class FICHE: public HObject
{

public:
    FICHE();
    FICHE(QString id, PATIENT *proprietaire, QDateTime date_admission, MODEADMISSION *mode_admission, MODESOIN *mode_soin,MEDECIN *responsable, QList <EXAMEN *> examens= QList<EXAMEN *>(), SERVICE *provenance= new SERVICE(), SORTIE *sortie= new SORTIE());
    FICHE(FICHE const &other);

    FICHE &operator=(FICHE &other);

    ~FICHE();

    PATIENT *proprietire() const;
    void setProprietire(PATIENT *proprietire);

    MODESOIN *modesoin() const;
    void setModesoin(MODESOIN *modesoin);

    SORTIE *sortie() const;
    void setSortie(SORTIE *sortie);

    MODEADMISSION *modeadmission() const;
    void setModeadmission(MODEADMISSION *modeadmission);

    QDateTime getDate_admission() const;
    void setDate_admission(const QDateTime &value);

    QList<EXAMEN *> getExamens() const;
    void setExamens(const QList<EXAMEN *> &examens);

    SERVICE *getProvenance() const;
    void setProvenance(SERVICE *value);


    MEDECIN *getResponsble() const;
    void setResponsble(MEDECIN *value);

protected:

    PATIENT *m_proprietire;
    MODESOIN *m_modesoin;
    SORTIE *m_sortie;
    SERVICE *provenance;
    QList <EXAMEN *> m_examens;
    MODEADMISSION *m_modeadmission;
    QDateTime date_admission;
    MEDECIN *responsble;
};

#endif // FICHE_H
