#include "hchartview.h"
#include "ui_hchartview.h"

HCHARTVIEW::HCHARTVIEW(QWidget *parent) : QWidget(parent), ui(new Ui::HCHARTVIEW)
{
    ui->setupUi(this);

    xmlManager= new HXmlManager(this);
    view = new QChartView();
    chart= new QChart();
    series= new QPieSeries;
    ui->containerChart->addWidget(view);

    QSqlDatabase *db= new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL","ADDCLASSDB"));


    db->setHostName(xmlManager->getDatabaseParameter(DBInfoType::HOSTNAME));
    db->setUserName(xmlManager->getDatabaseParameter(DBInfoType::USERNAME));
    db->setPassword(xmlManager->getDatabaseParameter(DBInfoType::PASSWORD));
    db->setDatabaseName(xmlManager->getDatabaseParameter(DBInfoType::DBNAME));
    db->setPort(xmlManager->getDatabaseParameter(DBInfoType::PORT).toInt());

    if (!db->open())
    {
        QMessageBox::warning(this,"Non connecté","Impossible d'acceder à la base de donnée.\nVeuillez réessayer!)");
        this->close();
    }

    dbm= new DBManager(*db,this);

    QObject::connect(dbm,&DBManager::error,this,&HCHARTVIEW::showError);
    dbm->seConnecter();

    ui->filtresVal->setVisible(false);
    nbCritere=0;

    soins= dbm->getAllModeSoin();
    sorties= dbm->getAllModeSortie();
    villes= dbm->getAllVille();
    natio= dbm->getAllNationalites();
    idMeres.append("");
    setCurrentNiveau(dbm->getClasses());
    setNiveauBase(getCurrentNiveau());
    values=dbm->getAllValues();


    int i=0;
    while (i < soins.length())
    {
        ui->modeSoin->addItem(soins.at(i)->getLabel());
        i++;
    }

    i=0;

    while (i < sorties.length())
    {
        ui->modeSortie->addItem(sorties.at(i)->getLabel());
        i++;
    }

    i=0;
    while (i < villes.length())
    {
        ui->ville->addItem(villes.at(i)->nom());
        i++;
    }
    i=0;
    while (i < natio.length())
    {
        ui->nationalite->addItem(natio.at(i)->getLabel());
        i++;
    }

    i=0;

    while (i < currentNiveau.length())
    {
        ui->queryBase->addItem(currentNiveau.at(i)->descriptif());
        i++;
    }

    i=0;
    while (i< values.length())
    {
        ui->values->addItem(values.at(i));
        i++;
    }


//    getListQueryCombo().append(ui->queryBase);
    QList<QComboBox *> s=getListQueryCombo();
    s.append(ui->queryBase);
    setListQueryCombo(s);
    setCurrentQueryCombo(ui->queryBase);

    connect(ui->queryBase,SIGNAL(currentIndexChanged(int)),this,SLOT(currentQueryIndexChanged(int)));




    ui->sexe->setCurrentIndex(1);
    ui->age->setValue(1);
    ui->age2->setValue(1);
    ui->ville->setCurrentIndex(0);
    ui->nationalite->setCurrentIndex(0);
    ui->dateAdmission->setDate(QDate::currentDate());
    ui->dateAdmission2->setDate(QDate::currentDate());
    ui->modeSoin->setCurrentIndex(0);
    ui->modeSoin->setCurrentIndex(0);
    ui->queryBase->setCurrentIndex(0);
    ui->values->setCurrentIndex(0);
}


HCHARTVIEW::~HCHARTVIEW()
{
    delete ui;
}

QChartView *HCHARTVIEW::getView() const
{
    return view;
}

void HCHARTVIEW::fomerReq()
{
    QString f="SELECT DISTINCT per.id_personne, COUNT(*) AS effectif ";
    QString last = " WHERE ";
    QString op= " AND  ";
    QString groupement= "GROUP BY ";
    bool first=true;
    bool hide=false;

    if(!sexe.isEmpty() && ui->sexe->isVisible())
    {
        if (!first)
        {
            last+=op;
        }
        hide=true;

        last+= "per.genre ="+sexe+" ";

        first=false;
    }

    if ((!age1.isEmpty() || !age2.isEmpty()) && (ui->age->isVisible() || ui->age2->isVisible()))
    {
        if(!first)
        {
            last+=op;
        }

        if (!age1.isEmpty() && !age2.isEmpty() & ui->age->isVisible() && ui->age2->isVisible())
        {
            hide=true;

            last+=" per.age BETWEEN "+age1+ " AND "+age2+" ";
        }
        else
        {
            hide=true;

            last+=" per.age="+age1+" ";
        }

        first=false;
    }

    if ((!ville.isEmpty() || !quatier.isEmpty()) && (ui->ville->isVisible() || ui->quartier->isVisible()))
    {
        if (!first)
        {
            last+=op;
        }

        if (!ville.isEmpty() && !quatier.isEmpty() &&ui->ville->isVisible() && ui->quartier->isVisible())
        {
            last+=" v.num_ville= "+ville +" AND q.id_quartier="+quatier+" ";
            hide=true;
        }
        else
        {
            last+=" v.num_ville= "+ville +" ";
            hide=true;

        }

        first=false;
    }

    if(!profession.isEmpty() && ui->profession->isVisible())
    {
        if (!first)
        {
            last+=op;
        }

        last+= "pat.proffession ="+profession+" ";

        first=false;
        hide=true;

    }
    if(!natinalite.isEmpty() && ui->nationalite->isVisible())
    {
        if (!first)
        {
            last+=op;
        }

        last+= "per.id_nationalite ="+natinalite+" ";

        first=false;
        hide=true;

    }

    if(!modeSoin.isEmpty() && ui->modeSoin->isVisible())
    {
        if (!first)
        {
            last+=op;
        }

        last+= "adm.id_mode_soin ="+modeSoin+" ";

        first=false;
        hide=true;

    }
    if(!modeSortie.isEmpty() && ui->modeSortie->isVisible())
    {
        if (!first)
        {
            last+=op;
        }

        last+= " sort.id_mode_sortie= "+modeSortie+" ";

        first=false;
        hide=true;
    }

    if((!dateAdmission1.isEmpty() || !dateAdmission2.isEmpty()) && (ui->dateAdmission->isVisible() || ui->dateAdmission2->isVisible()))
    {
        if (!first)
        {
            last+=op;
        }

        if (!dateAdmission1.isEmpty() && !dateAdmission2.isEmpty() && ui->dateAdmission->isVisible() && ui->dateAdmission2->isVisible())
        {
            last+= " adm.date_admission BEWEEN "+dateAdmission1+ " AND "+dateAdmission2+" ";
            hide=true;

        }
        else
        {
            last+= " adm.date_admission="+dateAdmission1+" ";
            hide=true;

        }

        first=false;
    }

    if(!classe.isEmpty() && !valeur.isEmpty()  && ui->queryBase->isVisible())
    {
        if (getCriterePrincipale()!="informationMedicalF")
        {
            if (!first)
            {
                last+=op;
            }


            last+= " cl.id_classe="+classe+ " AND val.contenu=\""+valeur+"\"";


            first=false;
            hide=true;
        }
        else
        {
            if (!first)
            {
                last+=op;
            }


            last+= " cl.id_classe="+classe+" ";


            first=false;
            hide=true;
        }
    }


    QString principal=getCriterePrincipale();

    if (principal=="sexeF")
    {

       groupement+=" per.genre ";
       f+=" ,per.genre AS indicatif ";
    }
    else if (principal=="ageF")
    {
        groupement+="per.age";
        f.append(" ,per.age AS indicatif ");
    }
    else if (principal=="intervalleAgeF")
    {
        groupement+="per.age";
        f.append(" ,per.age AS indicatif ");
    }
    else if (principal=="villeF")
    {
        groupement+="v.num_ville";
        f.append(" ,v.nom_ville AS indicatif ");
    }
    else if(principal=="villeQuartierF")
    {
        groupement+=" q.id_quartier";
        f.append(" ,GROUP_CONCAT(DISTINCT v.nom_ville,\" / \",q.nom_quartier) AS indicatif ");
    }
    else if (principal=="nationaliteF")
    {
        groupement+= " nat.id_nationalite ";
        f.append(" ,nat.label_nationalite AS indicatif ");
    }
    else if (principal=="professionF")
    {
        groupement+= " pat.proffession ";
        f.append(" ,pat.proffession AS indicatif ");
    }
    else if (principal=="dateAdmissionF")
    {
        groupement+=" adm.date_admission ";
        f.append(" ,DATE(adm.date_admission) AS indicatif ");
    }
    else if (principal=="IntervalleAdmissionF")
    {
        groupement+=" adm.date_admission ";
        f.append(" ,DATE(adm.date_admission) AS indicatif ");
    }
    else if (principal=="modeSoinF")
    {
       groupement+="adm.date_date_admission ";
       f.append(" ,ms.label AS indicatif ");
    }
    else if (principal=="modeSortieF")
    {
        groupement+=" sort.id_mode_sortie ";
        f.append(" ,msort.label AS indicatif ");
    }
    else if (principal=="informationMedicalF")
    {
        groupement+=" prop.id_valeur ";
        f.append(" ,val.contenu AS indicatif ");
    }


    QString swap= getCurrentReq();


    if (hide)
    {
        swap= f+" "+swap+last+" "+groupement+" ORDER BY effectif DESC";
    }
    else
    {
        swap= f+" "+swap+" "+groupement+" ORDER BY effectif DESC";
    }


    //formation des requêtes spéciaux

//    if (principal=="dateAdmissionF")
//    {
//        swap="SELECT indicatif , SUM(effectif) AS effectif FROM ("+swap+") AS efftable GROUP BY effectif";
//    }
    setReqString(swap);

    qDebug() << getReqString();

}

void HCHARTVIEW::interpretParam()
{
    QString req=" FROM personne AS per INNER JOIN patient AS pat ON per.id_personne=pat.id_personne INNER JOIN adresse AS adr ON adr.id_adresse=per.id_adresse INNER JOIN quartier AS q ON q.id_quartier=adr.id_quartier INNER JOIN ville AS v ON q.num_ville=v.num_ville INNER JOIN nationalite as nat ON nat.id_nationalite=per.id_nationalite ";
    QString req2=" INNER JOIN admission AS adm ON adm.id_patient=pat.id_personne INNER JOIN mode_soin AS ms ON ms.id_mode_soin=adm.id_mode_soin INNER JOIN sortie AS sort ON sort.id_admission=adm.id_admission INNER JOIN mode_sortie AS msort ON msort.id_mode_sortie=sort.id_mode_sortie ";
    QString req3= " INNER JOIN examen AS e ON e.id_admission=adm.id_admission INNER JOIN propos AS prop ON prop.id_examen=e.id_examen INNER JOIN classe AS cl ON cl.id_classe=prop.id_classe INNER JOIN valeur as val ON val.id_valeur=prop.id_valeur ";
    bool part2=false;
    bool part3=false;
    QList<QCheckBox *> box= ui->paramFiltre->findChildren<QCheckBox *> ();

    int i=0;
    while (i<box.length())
    {
        qDebug()<< box.at(i)->objectName();
        if (!box.at(i)->isChecked() || box.at(i)->objectName()==getCriterePrincipale())
        {
            if (box.at(i)->objectName()=="sexeF")
            {
                ui->sexeB->setVisible(false);
            }
            else if (box.at(i)->objectName()=="ageF")
            {
                if (!ui->intervalleAgeF->isChecked() || getCriterePrincipale()==ui->intervalleAgeF->objectName())
                {
                    ui->ageB->setVisible(false);
                }
                else
                {
                    ui->ageB->setVisible(true);
                    ui->age->setVisible(true);
                    ui->age2->setVisible(true);
                }
            }
            else if (box.at(i)->objectName()=="intervalleAgeF")
            {
                if (!ui->ageF->isChecked() || ui->ageF->objectName()==getCriterePrincipale())
                {
                    ui->ageB->setVisible(false);
                }
                else
                {
                    ui->ageB->setVisible(true);
                    ui->age2->setVisible(false);
                    ui->age->setVisible(true);
                }
            }
            else if (box.at(i)->objectName()=="villeF")
            {
                if (!ui->villeQuartierF->isChecked())
                {
                    ui->villeB->setVisible(false);
                }

            }
            else if(box.at(i)->objectName()=="villeQuartierF")
            {
                ui->quartierB->setVisible(false);
                ui->villeB->setVisible(false);
            }
            else if (box.at(i)->objectName()=="nationaliteF")
            {
                ui->nationaliteB->setVisible(false);
            }
            else if (box.at(i)->objectName()=="professionF")
            {
                ui->professionB->setVisible(false);
            }
            else if (box.at(i)->objectName()=="dateAdmissionF")
            {
                 if (box.at(i)->objectName()==getCriterePrincipale() && !part2)
                 {
                     req+=req2;
                     part2=true;
                 }
                if (!ui->IntervalleAdmissionF->isChecked() || ui->IntervalleAdmissionF->objectName()==getCriterePrincipale())
                {

                    ui->dateAdmissionB->setVisible(false);
                }
                else
                {

                    ui->dateAdmissionB->setVisible(true);
                    ui->dateAdmission->setVisible(true);
                    ui->dateAdmission2->setVisible(true);

                }
            }
            else if (box.at(i)->objectName()=="intervalleAdmissionF")
            {
                if (box.at(i)->objectName()==getCriterePrincipale() && !part2)
                {
                    req+=req2;
                    part2=true;
                }

                if (!ui->dateAdmissionF->isChecked() || ui->dateAdmissionF->objectName()==getCriterePrincipale())
                {
                    ui->dateAdmissionB->setVisible(false);
                }
                else
                {
                    ui->dateAdmissionB->setVisible(true);
                    ui->dateAdmission->setVisible(true);
                    ui->dateAdmission2->setVisible(false);
                }

            }
            else if (box.at(i)->objectName()=="modeSoinF")
            {
                ui->modeSoinB->setVisible(false);
                if (box.at(i)->objectName()==getCriterePrincipale() && !part2)
                {
                    req+=req2;
                    part2=true;
                }
            }
            else if (box.at(i)->objectName()=="modeSortieF")
            {
                ui->modeSortieB->setVisible(false);
                if (box.at(i)->objectName()==getCriterePrincipale() && !part2)
                {
                    req+=req2;
                    part2=true;
                }
            }
            else if (box.at(i)->objectName()=="informationMedicalF")
            {
                if (box.at(i)->objectName()!=getCriterePrincipale())
                {
                    ui->infoMedB->setVisible(false);
                }
                else
                {
                    ui->label_11->setVisible(false);
                    ui->values->setVisible(false);
                }

                if (box.at(i)->objectName()==getCriterePrincipale() && !part3)
                {

                    if (!part2)
                    {
                        req+=req2;
                        part2=true;
                    }
                    req+=req3; 
                    part3=true;
                }
            }

        }
        else
        {
            if ((box.at(i)->objectName()=="dateAdmissionF" ||box.at(i)->objectName()=="modeSoinF" || box.at(i)->objectName()=="intervalleAdmissionF" || box.at(i)->objectName()=="modeSortieF") && !part2)
            {
                req+=req2;
                part2=true;
            }
            else if ((box.at(i)->objectName()=="informationMedicalF") && !part3)
            {

                if (!part2)
                {
                    req+=req2;
                    part2=true;
                }
                req+=req3;

                part3=true;
            }

        }
        i++;
    }

    setCurrentReq(req);
}

void HCHARTVIEW::launch()
{

//    delete chart;

    series=new QPieSeries(view);

    series=dbm->getStat(getReqString());


    if (getCriterePrincipale()=="sexeF")// changement des simboles de sexe en lettre
    {
        int i=0;
        QString lab,sex;

        while (i<series->slices().length())
        {
            lab=series->slices().at(i)->label();
            sex=lab.split(" ").first();
            if (sex=="0")
            {
                sex="Femme";
            }
            else
            {
                sex="Homme";
            }

            lab.replace(0,1,sex);

            series->slices().at(i)->setLabel(lab);
            i++;
        }

    }

    QLineSeries tes;

    setChart(new QChart());
    getChart()->addSeries(series);
//    getChart()->setAnimationOptions(QChart::AnimationOption::SeriesAnimations);
    getChart()->setAnimationOptions(QChart::AnimationOption::AllAnimations);
    getChart()->setAnimationDuration(1000);
    getChart()->legend()->setAlignment(Qt::AlignRight);

//    QValueAxis *axisX = new QValueAxis;
//     axisX->setRange(10, 20.5);
//     axisX->setTickCount(10);
//     axisX->setLabelFormat("%.2f");
//     getChart()->setAxisX(axisX, series);


    if (series->isEmpty())
    {
         getChart()->setTitle("Aucune correspondance");
    }
    else
    {
        QString prop;
        if (getCriterePrincipale()=="sexeF")
        {
            prop="Sexe";
        }
        else if (getCriterePrincipale()=="ageF" || getCriterePrincipale()=="intervalleAgeF")
        {
            prop="Age";

        }
        else if (getCriterePrincipale()=="villeF")
        {
            prop="Ville";
        }
        else if(getCriterePrincipale()=="villeQuartierF")
        {
            prop=" Ville et Quartier";
        }
        else if (getCriterePrincipale()=="nationaliteF")
        {
            prop=" Nationalite ";
        }
        else if (getCriterePrincipale()=="professionF")
        {
            prop="Profession";
        }
        else if (getCriterePrincipale()=="dateAdmissionF")
        {
            prop="date d'amission";
        }
        else if (getCriterePrincipale()=="intervalleAdmissionF")
        {

            prop= "date admission entre "+dateAdmission1+" et "+dateAdmission2;
        }
        else if (getCriterePrincipale()=="modeSoinF")
        {
            prop="Mode de soin";
        }
        else if (getCriterePrincipale()=="modeSortieF")
        {
           prop="Mode de sortie";
        }
        else if (getCriterePrincipale()=="informationMedicalF")
        {
            prop="Infromation medicale "+classe;
        }

         getChart()->setTitle("Resultat Statistique par rapport à "+prop);

    }

    getChart()->legend()->show();

    history.append("Requêtre du "+QDateTime::currentDateTime().toString()+" : "+getReqString()+"\n");

    setReqString("");
//    view=new QChartView(getChart(),this);
    view->setChart(getChart());
//    view->setRubberBand(QChartView::RubberBand::VerticalRubberBand);
    //    ui->containerChart->addWidget(view);
}

void HCHARTVIEW::appendQueryCombo()
{
    QComboBox *n = new QComboBox(ui->queries);

    getCurrentQueryCombo()->setEnabled(false);
//    setCurrentNiveau(getCurrentNiveau().at(getListQueryCombo().last()->currentIndex())->subClasses());
    setCurrentNiveau(getCurrentNiveau().at(getListQueryCombo().last()->currentIndex())->subClasses());

    QList<QComboBox *> s=getListQueryCombo();
    s.append(n);
    setListQueryCombo(s);

    setCurrentQueryCombo(n);
    ui->queriesContainerLayout->addWidget(n);

    int i=0;

    while (i<getCurrentNiveau().length())
    {
        n->addItem(getCurrentNiveau().at(i)->descriptif());
        i++;
    }

    connect(n,SIGNAL(currentIndexChanged(int)),this,SLOT(currentQueryIndexChanged(int)));
}

void HCHARTVIEW::deleteLastQueryCombo()
{
    if (getListQueryCombo().length()>1)
    {
        qDebug() << "un";
        setCurrentQueryCombo(getListQueryCombo().at(getListQueryCombo().length()-2));
        qDebug() << "deux";
        getListQueryCombo().at(getListQueryCombo().length()-2)->setEnabled(true);
        qDebug() << "3";
        QList<CLASSE *> current=getNiveauBase();
        qDebug() << "4";

        QComboBox *toDel=getListQueryCombo().last();
        qDebug() << "5";

        QList<QComboBox *> sw= getListQueryCombo();
        sw.removeLast();
        setListQueryCombo(sw);

        qDebug() << "6";
        delete toDel;

        qDebug() << "7";
        int i=0;

        while (i<getListQueryCombo().length()-1)
        {
            current=current.at(getListQueryCombo().at(i)->currentIndex())->subClasses();
            i++;
        }
        qDebug() << "8";

        setCurrentNiveau(current);
//         delete ui->queriesContainerLayout->findChildren<QComboBox *>().last();
    }
    else
    {
        getListQueryCombo().first()->setEnabled(true);
    }
}

QPieSeries *HCHARTVIEW::getSeries() const
{
    return series;
}

void HCHARTVIEW::setSeries(QPieSeries *value)
{
    series = value;
}

void HCHARTVIEW::eventCheck(bool buttonstate, QCheckBox *box)
{
    if (buttonstate)
    {
        nbCritere++;
    }
    else
    {
        nbCritere--;
    }

    if (!getPrincipaleEstChoisi() && buttonstate)
    {
        this->criterePrincipale=box->objectName();
        this->setPrincipaleEstChoisi(true);
        box->setEnabled(false);
    }
}

void HCHARTVIEW::showError(QString text)
{
    QMessageBox::warning(this,"Error",text);
}

QList<QPieSlice *> HCHARTVIEW::getResults() const
{
    return results;
}

void HCHARTVIEW::setResults(const QList<QPieSlice *> &value)
{
    results = value;
}

QString HCHARTVIEW::getReqString() const
{
    return reqString;
}

void HCHARTVIEW::setReqString(const QString &value)
{
    reqString = value;
}

bool HCHARTVIEW::getPrincipaleEstChoisi() const
{
    return principaleEstChoisi;
}

void HCHARTVIEW::setPrincipaleEstChoisi(bool value)
{
    principaleEstChoisi = value;
}

QString HCHARTVIEW::getCriterePrincipale() const
{
    return criterePrincipale;
}

int HCHARTVIEW::getNbCritere() const
{
    return nbCritere;
}

void HCHARTVIEW::setNbCritere(int value)
{
    nbCritere = value;
}

QChart *HCHARTVIEW::getChart() const
{
    return chart;
}

void HCHARTVIEW::setChart(QChart *value)
{
    chart = value;
}

void HCHARTVIEW::on_sexeF_toggled(bool checked)
{
    eventCheck(checked,ui->sexeF);
}

void HCHARTVIEW::on_villeF_toggled(bool checked)
{
    eventCheck(checked,ui->villeF);
    ui->villeQuartierF->setEnabled(!checked);
}

void HCHARTVIEW::on_dateAdmissionF_toggled(bool checked)
{
    eventCheck(checked,ui->dateAdmissionF);
    ui->IntervalleAdmissionF->setEnabled(!checked);
}


void HCHARTVIEW::on_ageF_toggled(bool checked)
{
    eventCheck(checked,ui->ageF);

    ui->intervalleAgeF->setEnabled(!checked);

}



void HCHARTVIEW::on_modeSoinF_toggled(bool checked)
{
    eventCheck(checked,ui->modeSoinF);

}

void HCHARTVIEW::on_intervalleAgeF_toggled(bool checked)
{
    eventCheck(checked,ui->intervalleAgeF);
    ui->ageF->setEnabled(!checked);
}

void HCHARTVIEW::on_nationalieF_toggled(bool checked)
{
    eventCheck(checked,ui->nationaliteF);
    ui->ageF->setEnabled(!checked);
}

void HCHARTVIEW::on_professionF_toggled(bool checked)
{

    eventCheck(checked,ui->professionF);
     ui->ageF->setEnabled(!checked);
}

void HCHARTVIEW::on_modeSortieF_toggled(bool checked)
{
    eventCheck(checked,ui->modeSortieF);

}

void HCHARTVIEW::on_informationMedicalF_toggled(bool checked)
{
    eventCheck(checked,ui->informationMedicalF);
}

void HCHARTVIEW::on_nationaliteF_toggled(bool checked)
{
    eventCheck(checked,ui->nationaliteF);
}


//void HCHARTVIEW::on_villeQuartierF_toggled(bool checked)
//{
//    eventCheck(checked,ui->villeQuartierF);
//    ui->villeF->setEnabled(!checked);
//}

void HCHARTVIEW::on_IntervalleAdmissionF_toggled(bool checked)
{
    eventCheck(checked,ui->IntervalleAdmissionF);
    ui->dateAdmissionF->setEnabled(!checked);
}


void HCHARTVIEW::on_rechoisir_clicked()
{

    QList<QCheckBox *> box= ui->paramFiltre->findChildren<QCheckBox *> ();
    int i=0;
    while(i<box.length())
    {
        if(box.at(i)->objectName()==criterePrincipale)
        {
            box.at(i)->setChecked(false);
            box.at(i)->setEnabled(true);
            this->setPrincipaleEstChoisi(false);
            break;
        }
        i++;
    }

}

void HCHARTVIEW::on_refresh_clicked()
{

    QList<QCheckBox *> box= ui->paramFiltre->findChildren<QCheckBox *> ();
    int i=0;
    while(i<box.length())
    {

        box.at(i)->setChecked(false);
        box.at(i)->setEnabled(true);
        i++;
    }

    setPrincipaleEstChoisi(false);

}

void HCHARTVIEW::on_pushButton_clicked()
{
    QList<QWidget *> box= ui->filtresVal->findChildren<QWidget *>();
    int i=0;

    while (i<box.length())
    {
        box.at(i)->setVisible(true);
        i++;
    }

    ui->filtresVal->setVisible(false);
    ui->paramFiltre->setVisible(true);

}

void HCHARTVIEW::on_villeQuartierF_toggled(bool checked)
{
    eventCheck(checked,ui->villeQuartierF);
    ui->villeF->setEnabled(!checked);
}

void HCHARTVIEW::on_lauch_clicked()
{
//    interpretParam();
    this->fomerReq();
    this->launch();
}

void HCHARTVIEW::on_sexe_currentIndexChanged(int index)
{
    sexe=QString::number(index);
}

void HCHARTVIEW::on_age_valueChanged(int arg1)
{
    age1=QString::number(arg1);
}

void HCHARTVIEW::on_age2_valueChanged(const QString &arg1)
{
    age2=arg1;
}

void HCHARTVIEW::on_ville_currentIndexChanged(int index)
{
    ville= QString::number(villes.at(index)->getId());
    int i=0;
    quartiers= dbm->getAllQuatierOfVille(QString::number(villes.at(index)->getId()));

    while (i<quartiers.length())
    {
        ui->quartier->addItem(quartiers.at(i)->nom());
        i++;
    }
}

void HCHARTVIEW::on_quartier_currentIndexChanged(int index)
{
    quatier= QString::number(quartiers.at(index)->getId());
}

void HCHARTVIEW::on_nationalite_currentIndexChanged(int index)
{
    natinalite= QString::number(natio.at(index)->getId());
}

void HCHARTVIEW::on_dateAdmission_dateChanged(const QDate &date)
{
    dateAdmission1= date.toString("yyyy-MM-dd");
}

void HCHARTVIEW::on_dateAdmission2_dateChanged(const QDate &date)
{
    dateAdmission2= date.toString("yyyy-MM-dd");
}

void HCHARTVIEW::on_modeSoin_currentIndexChanged(int index)
{
    modeSoin= QString::number(soins.at(index)->getId());
}

void HCHARTVIEW::on_modeSortie_currentIndexChanged(int index)
{
    modeSortie= QString::number(sorties.at(index)->getId());
}

//void HCHARTVIEW::on_queryBase_currentIndexChanged(int index)
//{
//    classe= QString::number(currentNiveau.at(index)->getId());

//    if (currentNiveau.at(index)->subClasses().isEmpty())
//    {
//        ui->addQuery->setEnabled(false);
//    }
//    else
//    {
//        ui->addQuery->setEnabled(true);
//    }
//}

void HCHARTVIEW::on_values_currentTextChanged(const QString &arg1)
{
    valeur=arg1;
}

void HCHARTVIEW::on_values_currentIndexChanged(const QString &arg1)
{
    valeur=arg1;
}

void HCHARTVIEW::on_addQuery_clicked()
{
    appendQueryCombo();
    if (!currentNiveau.at(getCurrentQueryCombo()->currentIndex())->subClasses().isEmpty())
    {
        ui->addQuery->setEnabled(true);
    }
    else
    {
        ui->addQuery->setEnabled(false);
    }

    if (getListQueryCombo().length()>1)
    {
        ui->delQuery->setEnabled(true);
    }
    else
    {
        ui->delQuery->setEnabled(false);
    }

}

void HCHARTVIEW::on_age_valueChanged(const QString &arg1)
{
    age1=arg1;
}

QString HCHARTVIEW::getCurrentReq() const
{
    return currentReq;
}

void HCHARTVIEW::setCurrentReq(const QString &value)
{
    currentReq = value;
}

void HCHARTVIEW::on_endPram_clicked()
{
    QList<QCheckBox *> box= ui->paramFiltre->findChildren<QCheckBox *> ();

    if(!box.isEmpty())
    {
        if (getPrincipaleEstChoisi())
        {
            ui->paramFiltre->setVisible(false);
            ui->filtresVal->setVisible(true);
            interpretParam();
        }
        else
        {
            QMessageBox::warning(this,"Invalide","Vous n'avez pas encore séléctionné le critère principale");
        }
    }
    else
    {
        QMessageBox::warning(this,"Invalide","Veuillez au moins sélétionner un critère!");
    }
}

void HCHARTVIEW::currentQueryIndexChanged(int index)
{
//        qDebug()<< "index "+QString::number(index);
//    int curi= getCurrentQueryCombo()->currentIndex();
    classe= QString::number(getCurrentNiveau().at(index)->getId());

    if (currentNiveau.at(index)->subClasses().isEmpty())
    {
        ui->addQuery->setEnabled(false);
    }
    else
    {
        ui->addQuery->setEnabled(true);
    }
}

QList<CLASSE *> HCHARTVIEW::getNiveauBase() const
{
    return niveauBase;
}

void HCHARTVIEW::setNiveauBase(const QList<CLASSE *> &value)
{
    niveauBase = value;
}

QList<CLASSE *> HCHARTVIEW::getCurrentNiveau() const
{
    return currentNiveau;
}

void HCHARTVIEW::setCurrentNiveau(const QList<CLASSE *> &value)
{
    currentNiveau = value;
}

QComboBox *HCHARTVIEW::getCurrentQueryCombo() const
{
    return currentQueryCombo;
}

void HCHARTVIEW::setCurrentQueryCombo(QComboBox *value)
{
    currentQueryCombo = value;
}

QList<QComboBox *> HCHARTVIEW::getListQueryCombo() const
{
    return listQueryCombo;
}

void HCHARTVIEW::setListQueryCombo(const QList<QComboBox *> &value)
{
    listQueryCombo = value;
}

void HCHARTVIEW::on_delQuery_clicked()
{
    deleteLastQueryCombo();

    if (!currentNiveau.at(getCurrentQueryCombo()->currentIndex())->subClasses().isEmpty())
    {
        ui->addQuery->setEnabled(true);
    }
    else
    {
        ui->addQuery->setEnabled(false);
    }


    if (getListQueryCombo().length()>1)
    {
        ui->delQuery->setEnabled(true);
    }
    else
    {
        ui->delQuery->setEnabled(false);
    }
}


