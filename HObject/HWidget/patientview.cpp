#include "patientview.h"
#include "ui_patientview.h"


PATIENTVIEW::PATIENTVIEW(FICHE *fiche, QWidget *parent):QWidget(parent),ui(new Ui::PATIENTVIEW)
{
    ui->setupUi(this);
    this->setFiche(fiche);
}

PATIENTVIEW::~PATIENTVIEW()
{
    delete ui;
    delete fiche;
}

void PATIENTVIEW::configure()
{
    if (fiche->isNull())
    {
        QMessageBox::critical(this,"Fiche invalide","La fiche du patient "+fiche->proprietire()->nom()+" est invalide!");
        this->hide();
    }

    ui->parcours->setVisible(false);

    ui->nationalite->setText(fiche->proprietire()->nationalite()->getLabel());

    ui->nom->setText(fiche->proprietire()->nom());
    ui->prenom->setText(fiche->proprietire()->prenom());
    ui->age->setText(fiche->proprietire()->age());
    ui->sexe->setText(fiche->proprietire()->genre()==sexe::HOMME? "Masculin": "Feminin");
    ui->profession->setText(fiche->proprietire()->getProffession());
    ui->status->setText(fiche->proprietire()->getStatut()->getLabel());
    ui->quartier->setText(fiche->proprietire()->adresse()->quartier()->nom());
    ui->ville->setText(fiche->proprietire()->adresse()->quartier()->ville()->nom());
    ui->lot->setText(fiche->proprietire()->adresse()->lot());

    ui->modeAdmission->setText(fiche->modeadmission()->getLabel());
    ui->dateAdmission->setText(fiche->getDate_admission().toString("dddd dd/MMMM/yyyy à HH:mm:ss"));
    ui->serviceProvenance->setText(fiche->getProvenance()->nom());
    ui->hopitalProvenance->setText(fiche->getProvenance()->nomHopital());


    ui->motifHospitalisation->setText("MALADIE");//à corriger


    ui->dateSortie->setText(fiche->sortie()->getDate().toString("dddd dd/MMMM/yyyy à HH:mm:ss"));
    ui->modeSortie->setText(fiche->sortie()->mode()->getLabel());
    ui->serviceSestinaire->setText(fiche->sortie()->getDestination()->nom());

    ui->serviceProvenance->setText(fiche->getProvenance()->nom());
    ui->hopitalProvenance->setText(fiche->getProvenance()->nomHopital());

    ui->responsablePrincipal->setText(fiche->getResponsble()->nom()+" "+fiche->getResponsble()->prenom());
    if (ui->serviceProvenance->text().isEmpty())
    {
        ui->provenance->setVisible(false);
    }
    if (ui->serviceSestinaire->text().isEmpty() && ui->dateSortie->text().isEmpty())
    {
        ui->informationTransfert->setVisible(false);

        setIsExited(false);
    }
    else
    {
        ui->pushButton->setVisible(false);
        setIsExited(true);
    }

    if (fiche->getResponsble()->nom().isEmpty())
    {
        ui->responsablePrincipal->setText("Non spécifié");
        ui->responsablePrincipal->setEnabled(false);
    }
    else
    {
        ui->responsablePrincipal->setEnabled(true);
    }


    int i=0;


    while (i<ui->infoMedLayout->children().length())
    {
        delete ui->infoMedLayout->children().at(i);
        i++;
    }



    //paremétrage dela vue des examens


    QList<EXAMEN *> examen= fiche->getExamens();

    i=1;//ne pas affiches l'exament 0 qui est l'examen par défaut contenant les classes mères sans valeur

    INFORMATIONEXAMEN *infex;

    while (i < examen.length())
    {

        infex= new INFORMATIONEXAMEN(examen.at(i),this);
        ui->infoMedLayout->addWidget(infex);


        i++;
    }

}


class FICHE *PATIENTVIEW::getFiche() const
{
    return fiche;
}

void PATIENTVIEW::setFiche(class FICHE *value)
{
    fiche = value;
    configure();
}


void PATIENTVIEW::on_pushButton_clicked()
{
    emit addExam(getFiche());
}

bool PATIENTVIEW::getIsExited() const
{
    return m_isExited;
}

void PATIENTVIEW::setIsExited(bool isExited)
{
    m_isExited = isExited;
}

void PATIENTVIEW::clicAdd()
{
    ui->pushButton->click();
}
