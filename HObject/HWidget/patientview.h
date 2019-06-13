#ifndef PATIENTVIEW_H
#define PATIENTVIEW_H

#include <QWidget>
#include "HObject/hobjects.h"
#include "informationexamen.h"

#include <QMessageBox>
namespace Ui {
class PATIENTVIEW;
}

class PATIENTVIEW : public QWidget
{
    Q_OBJECT

public:
    PATIENTVIEW (class FICHE *fiche,QWidget *parent);

    ~PATIENTVIEW();



    class FICHE *getFiche() const;
    void setFiche(class FICHE *value);

    bool getIsExited() const;

signals:
    void addExam(class FICHE *f);
//    void sortieState(bool state);//pas d'utilité
protected:
    void configure();
    void setIsExited(bool isExited);
    void clicAdd();
private slots:
    void on_pushButton_clicked();

private:
    bool m_isExited;
    Ui::PATIENTVIEW *ui;
    class FICHE *fiche;
};

#endif // PATIENTVIEW_H
