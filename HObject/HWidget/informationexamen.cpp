#include "informationexamen.h"
#include "ui_informationexamen.h"

INFORMATIONEXAMEN::INFORMATIONEXAMEN(EXAMEN *examen, QWidget *parent) : QWidget(parent), ui(new Ui::INFORMATIONEXAMEN)
{
    ui->setupUi(this);
    this->setExamen(examen);
}

INFORMATIONEXAMEN::~INFORMATIONEXAMEN()
{
    delete ui;
    delete m_examen;
}


void INFORMATIONEXAMEN::configure()
{
    this->ui->date->setText(examen()->date().toString());
    this->ui->examinateur->setText(examen()->examinateur()->nom()+" "+examen()->examinateur()->prenom());

    //mettre le service dans

    //instantiation des information classe

    int i=0;
    QList<CLASSE *> clas= examen()->propos();//récupération des classes de l'examen

    qDebug () << "La taille des propos de l'exame du "+examen()->date().toString()+" est "+QString::number(clas.length());
    INFORMATIONCLASS *infC;
    while (i < clas.length())
    {
        infC= new INFORMATIONCLASS(clas.at(i),this);
        ui->containerClasses->addWidget(infC);
        infC->show();

//        ui->groupBox->layout()->addWidget(infC);
//        infC->show();

        i++;
    }
}

EXAMEN *INFORMATIONEXAMEN::examen() const
{
    return m_examen;
}

void INFORMATIONEXAMEN::setExamen(EXAMEN *examen)
{
    m_examen = examen;
    this->configure();
}
