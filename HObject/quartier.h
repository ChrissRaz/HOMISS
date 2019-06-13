#ifndef QUARTIER_H
#define QUARTIER_H

#include <HObject/ville.h>


namespace H
{
    class QUARTIER;
}

class QUARTIER: public HObject
{
public:
    QUARTIER(int id,QString nom,VILLE* ville);
    QUARTIER(QUARTIER const &other);
    QUARTIER();

    ~QUARTIER();

    QString nom() const;
    void setNom(const QString &nom);


    QUARTIER & operator=(QUARTIER const&other);

    VILLE *ville() const;
    void setVille(VILLE *ville);

protected:
    QString m_nom;
    VILLE *m_ville;
};

#endif // QUARTIER
