#ifndef INFORMATIONEXAMEN_H
#define INFORMATIONEXAMEN_H

#include <QWidget>
#include "informationclass.h"
#include "HObject/examen.h"

namespace Ui {
class INFORMATIONEXAMEN;
}

class INFORMATIONEXAMEN : public QWidget
{
    Q_OBJECT

public:
    INFORMATIONEXAMEN(EXAMEN *examen, QWidget *parent = nullptr);
    ~INFORMATIONEXAMEN();

    EXAMEN *examen() const;
    void setExamen(EXAMEN *examen);


    void configure();


private:
    Ui::INFORMATIONEXAMEN *ui;
    EXAMEN *m_examen;

};

#endif // INFORMATIONEXAMEN_H
