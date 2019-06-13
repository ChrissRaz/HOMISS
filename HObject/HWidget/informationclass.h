#ifndef INFORMATIONCLASS_H
#define INFORMATIONCLASS_H

#include <QWidget>
#include "HObject/classe.h"
#include "QDebug"
#include "QRandomGenerator"

namespace Ui {
class INFORMATIONCLASS;
}

class INFORMATIONCLASS : public QWidget
{
    Q_OBJECT

public:
    INFORMATIONCLASS(CLASSE *classe, QWidget *parent = nullptr);
    ~INFORMATIONCLASS();

    QList<CLASSE *> classes() const;

    void setClasses(const QList<CLASSE *> &classes);

    void configure();

    CLASSE *classe() const;
    void setClasse(CLASSE *classe);

private:
    Ui::INFORMATIONCLASS *ui;
    CLASSE *m_classe;
};

#endif // INFORMATIONCLASS_H
