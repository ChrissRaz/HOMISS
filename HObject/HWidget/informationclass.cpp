#include "informationclass.h"
#include "ui_informationclass.h"

INFORMATIONCLASS::INFORMATIONCLASS(CLASSE *classe, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::INFORMATIONCLASS)
{
    ui->setupUi(this);
    this->setClasse(classe);
    QString col;
    QRandomGenerator *gen= new QRandomGenerator();

//    col=  "rgb("+QString::number((int) gen->bounded(0,255))+","+QString::number((int) gen->bounded(0,255))+","+QString::number((int) gen->bounded(0,255))+")";
    this->setStyleSheet("background: rgba(225,225,225,0.2);");
//    delete gen;
}

INFORMATIONCLASS::~INFORMATIONCLASS()
{
    delete ui;
}

void INFORMATIONCLASS::configure()
{
    qDebug() << "Instantiation de la vue de la classe "+ classe()->descriptif();
    this->ui->descriptif->setText(classe()->descriptif());

    int i=0;

    if (classe()->subClasses().isEmpty())
    {
        QString val="";
        i=0;
        while(i<classe()->values().length())
        {
            if (i==0)
            {
                val+=classe()->values().at(i);
            }
            else
            {
                val+=", "+classe()->values().at(i);
            }

            i++;
        }
        this->ui->value->setText(val);
    }


    i=0;
    INFORMATIONCLASS *infc;

    while (i<classe()->subClasses().length())
    {
        infc= new INFORMATIONCLASS(classe()->subClasses().at(i),ui->subClasse);
        ui->subClasseLayout->addWidget(infc);
        i++;
    }
}

CLASSE *INFORMATIONCLASS::classe() const
{
    return m_classe;

}

void INFORMATIONCLASS::setClasse(CLASSE *classe)
{
    m_classe = classe;
    configure();
}
