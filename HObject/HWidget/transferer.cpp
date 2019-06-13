#include "transferer.h"
#include "ui_transferer.h"

TRANSFERER::TRANSFERER(class FICHE *f, QWidget *parent) :QDialog (parent),ui(new Ui::TRANSFERER)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setF(f);
    sortie= new SORTIE();
    xmlManager= new HXmlManager(this);
    newExamen= new EXAMEN("-1",ui->dateSortie->dateTime(),f->getResponsble(),QList<CLASSE *>());
    ui->label_5->setText("Sortie du patient "+f->proprietire()->nom()+" "+f->proprietire()->prenom()+" du service");

    setId_admission(QString::number(f->getId()));
    QSqlDatabase *db;
    db= new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL","TRANSFERTDB"));
    db->setHostName(xmlManager->getDatabaseParameter(DBInfoType::HOSTNAME));
    db->setUserName(xmlManager->getDatabaseParameter(DBInfoType::USERNAME));
    db->setPassword(xmlManager->getDatabaseParameter(DBInfoType::PASSWORD));
    db->setDatabaseName(xmlManager->getDatabaseParameter(DBInfoType::DBNAME));
    db->setPort(xmlManager->getDatabaseParameter(DBInfoType::PORT).toInt());

    dbm= new DBManager(*db,this);


    QObject::connect(dbm,&DBManager::connected,this, &TRANSFERER::activate);
    QObject::connect(dbm,&DBManager::error,this, &TRANSFERER::showDBError);

    dbm->seConnecter();


    QList<QDomElement > vals= xmlManager->getValuesOfMenu("9");
    QStringList v;
    int k=0;
    QStringList s;
    while (k<vals.length())
    {
        v.append(vals.at(k).attribute("content"));
        k++;
    }

    k=0;
    vals= xmlManager->getMenuChilds("9");
    while (k<vals.length())
    {
        s.append(vals.at(k).attribute("id"));
        k++;
    }

    QDomElement paramTrans= xmlManager->getMenu("9",xmlManager->findElement(xmlManager->getConfigurationDom()->documentElement(),"model"));
    parametre = new EXAMLIGNE(paramTrans.attribute("id"),paramTrans.attribute("descriptif"), paramTrans.attribute("nbVal").toInt(),v, paramTrans.attribute("Alias"), paramTrans.attribute("remplissage"),s,paramTrans.attribute("valueModifiable")=="true"?true:false, paramTrans.attribute("valueInverted")=="true"? true:false,QString(),this);

     ui->containerPramSortie->addWidget(parametre);
     parametre->derouler();
    connect(parametre,&EXAMLIGNE::set,this,&TRANSFERER::recept);
    connect(parametre,&EXAMLIGNE::clear,this,&TRANSFERER::remove);
}

TRANSFERER::~TRANSFERER()
{
//    delete m_f;
    delete ui;
//    delete dbm;
//    delete xmlManager;
    //à résoudre plus tard
}

void TRANSFERER::showDBError(QString message)
{
    QMessageBox::critical(this,"Connewion  error",message);
}

void TRANSFERER::activate()
{
    ui->dateSortie->setEnabled(true);
    ui->modeSortie->setEnabled(true);
    ui->serviceDestionaire->setEnabled(true);
    ui->hopitalDestinaire->setEnabled(true);
    ui->buttonBox->setEnabled(false);
    ui->Local->setEnabled(true);

    setHopitals(dbm->getAllHopital());
    setServices(getHopitals().at(0)->services());
    updateCompleterHopital(dbm->getAllHopital());
    updateCompleterService(getServices());
    setSorties(dbm->getAllModeSortie());

    ui->dateSortie->setDateTime(QDateTime::currentDateTime());
    int i=0;

    while (i < getSorties().length())
    {
        ui->modeSortie->addItem(getSorties().at(i)->getLabel());
        i++;
    }

//    ui->hopitalDestinaire->setCurrentText("CENHOSOA");
}


void TRANSFERER::on_Local_toggled(bool checked)
{
    if (checked)
    {
        ui->hopitalDestinaire->setCurrentText("CENHOSOA");

        ui->serviceDestionaire->setEditable(false);
    }
    else
    {
        if (ui->hopitalDestinaire->currentText()!="CENHOSOA")
        {
            ui->serviceDestionaire->setEditable(true);
        }

    }
}

void TRANSFERER::updateCompleterService(QList<SERVICE *> se)
{
    ui->serviceDestionaire->clear();
    int i=0;
    QStringList contents;
    while (i<se.length())
    {
        contents.append(se.at(i)->nom());
        i++;
    }
    contents.sort(Qt::CaseInsensitive);
    ui->serviceDestionaire->addItems(contents);

}

void TRANSFERER::updateCompleterHopital(QList<HOPITAL *> ho)
{
    int i=0;
    QStringList contents;
    while (i<ho.length())
    {
        contents.append(ho.at(i)->getNom());
        i++;
    }

    contents.sort(Qt::CaseInsensitive);
    ui->hopitalDestinaire->addItems(contents);

//    ui->serviceDestionaire->setCompleter(new QCompleter(contents));
}

class FICHE *TRANSFERER::getF() const
{
    return m_f;
}

void TRANSFERER::setF(class FICHE *f)
{
    m_f = f;
}

QList<SERVICE *> TRANSFERER::getServices() const
{
    return services;
}

void TRANSFERER::setServices(const QList<SERVICE *> &value)
{
    services = value;
}

QList<HOPITAL *> TRANSFERER::getHopitals() const
{
    return hopitals;
}

void TRANSFERER::setHopitals(const QList<HOPITAL *> &value)
{
    hopitals = value;
}

QList<MODESORTIE *> TRANSFERER::getSorties() const
{
    return sorties;
}

void TRANSFERER::setSorties(const QList<MODESORTIE *> &value)
{
    sorties = value;
}

QString TRANSFERER::getId_admission() const
{
    return id_admission;
}

void TRANSFERER::setId_admission(const QString &value)
{
    id_admission = value;
}

void TRANSFERER::on_hopitalDestinaire_currentIndexChanged(const QString &arg1)
{
//    setServices(dbm->getServices(QString::number(getHopitals().at(ui->hopitalDestinaire->currentIndex())->getId())));

    setServices(getHopitals().at(ui->hopitalDestinaire->currentIndex())->services());
    updateCompleterService(getServices());

    if (arg1.toUpper()=="CENHOSOA")
    {
        ui->serviceDestionaire->setEditable(false);
        if (!ui->Local->isChecked())
        {
            ui->Local->setChecked(true);
        }
    }
    else
    {
        ui->serviceDestionaire->setEditable(true);
        if (ui->Local->isChecked())
        {
            ui->Local->setChecked(false);
        }
    }

    if (!ui->serviceDestionaire->currentText().isEmpty() && !ui->hopitalDestinaire->currentText().isEmpty())
    {
        ui->buttonBox->setEnabled(true);
    }
    else
    {
        ui->buttonBox->setEnabled(false);
    }

    sortie->getDestination()->setNomHopital(arg1);
}

void TRANSFERER::on_hopitalDestinaire_currentTextChanged(const QString &arg1)
{
    setServices(getHopitals().at(ui->hopitalDestinaire->currentIndex())->services());
    updateCompleterService(getServices());

    if (!ui->serviceDestionaire->currentText().isEmpty() && !ui->hopitalDestinaire->currentText().isEmpty())
    {
        ui->buttonBox->setEnabled(true);
    }
    else
    {
        ui->buttonBox->setEnabled(false);
    }

    if (arg1.toUpper()=="CENHOSOA")
    {
        ui->serviceDestionaire->setEditable(false);
    }
    else
    {
        ui->serviceDestionaire->setEditable(true);
    }

    sortie->getDestination()->setNomHopital(arg1);
}

void TRANSFERER::on_modeSortie_currentIndexChanged(int index)
{
    if (ui->modeSortie->currentText().toUpper()=="TRASFERT DANS UN AUTRE SERVICE")
    {
        ui->hopitalDestinaire->setEnabled(true);
        ui->serviceDestionaire->setEnabled(true);
        ui->Local->setEnabled(true);
        ui->groupBox->setEnabled(true);
    }
    else
    {
        ui->hopitalDestinaire->setEnabled(false);
        ui->serviceDestionaire->setEnabled(false);
        ui->Local->setEnabled(false);
        ui->groupBox->setEnabled(false);

    }

    sortie->setMode(getSorties().at(index));

    if (!ui->serviceDestionaire->isEnabled())
    {
        ui->buttonBox->setEnabled(true);
    }
    else
    {
        if (!ui->serviceDestionaire->currentText().isEmpty() && !ui->hopitalDestinaire->currentText().isEmpty())
        {
            ui->buttonBox->setEnabled(true);
        }
        else
        {
            ui->buttonBox->setEnabled(false);
        }
    }
}

void TRANSFERER::on_serviceDestionaire_currentTextChanged(const QString &arg1)
{
    if (ui->serviceDestionaire->currentIndex()!=-1)
    {
        sortie->setDestination(getServices().at(ui->serviceDestionaire->currentIndex()));
    }
    else
    {
        sortie->setDestination(new SERVICE(-1,arg1,QList<MEDECIN *>(),QList<INFIRMIER *>(),ui->hopitalDestinaire->currentText()));
    }

    if (!arg1.isEmpty() && !ui->hopitalDestinaire->currentText().isEmpty())
    {
        ui->buttonBox->setEnabled(true);
    }
    else
    {
        ui->buttonBox->setEnabled(false);
    }

}

void TRANSFERER::on_serviceDestionaire_currentIndexChanged(int index)
{
    if (index!=-1)
    {
        sortie->setDestination(getServices().at(ui->serviceDestionaire->currentIndex()));
    }
    else
    {
        sortie->setDestination(new SERVICE(-1,ui->serviceDestionaire->currentText(),QList<MEDECIN *>(),QList<INFIRMIER *>(),ui->hopitalDestinaire->currentText()));
    }

    if (!ui->serviceDestionaire->currentText().isEmpty() && !ui->hopitalDestinaire->currentText().isEmpty())
    {
        ui->buttonBox->setEnabled(true);
    }
    else
    {
        ui->buttonBox->setEnabled(false);
    }
}

void TRANSFERER::on_dateSortie_dateTimeChanged(const QDateTime &dateTime)
{
    sortie->setDate(dateTime);
    newExamen->setDate(dateTime);
    if (!ui->serviceDestionaire->isEnabled())
    {
        ui->buttonBox->setEnabled(true);
    }
    else
    {
        if (!ui->serviceDestionaire->currentText().isEmpty() && !ui->hopitalDestinaire->currentText().isEmpty())
        {
            ui->buttonBox->setEnabled(true);
        }
        else
        {
            ui->buttonBox->setEnabled(false);
        }
    }
}

void TRANSFERER::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->buttonRole(button)==QDialogButtonBox::AcceptRole)
    {

        if (!ui->serviceDestionaire->isEnabled())
        {
            sortie->setDestination(new SERVICE());
        }
        else
        {
            if (newExamen->propos().isEmpty())
            {
                if (QMessageBox::No==QMessageBox::question(this,"Incomlet","Aucun paramètre de sortie n'est spécifié.\n Voulez vous continuer ainsi?"))
                {
                    return;
                }
            }
        }
        getF()->setSortie(sortie);

        QList<EXAMEN *> paramsSortie;
        paramsSortie.append(newExamen);
        if (dbm->addSortie(getId_admission(),sortie) && dbm->addExam(getId_admission(),paramsSortie))
        {
            if (ui->serviceDestionaire->isVisible())
            {
                QString emplacement=QFileDialog::getSaveFileName(this,"Emplacement du dossier médicale",QString(),"PDF (*.dmp)");

                emit save(emplacement);//signale pour enregistrer le dossier médicale

                QMessageBox::information(this,"Succes","La surtie est fait avec succe.\nvous pouvesz récupérer le dossier médicale dans"+emplacement);
            }

        }
        qDebug() << "Sortie de "+ getF()->proprietire()->nom()+" : "+getF()->sortie()->getDate().toString();

        if (!ui->serviceDestionaire->isEnabled())
        {
            qDebug() << "destionation "+ getF()->sortie()->getDestination()->nom() + " hopital "+getF()->sortie()->getDestination()->nomHopital();
        }

    }
    else
    {
        QApplication::beep();
        if(QMessageBox::Yes==QMessageBox::question(this,"Abandonner","Abandonner la sortie?"))
        {
            this->close();
        }
    }
}

void TRANSFERER::recept(QString id, QString id_mere, QString descri, QString value)
{

    qDebug() << value +" descri " +descri+ " " +id + "child of "+id_mere;
    int i=0;

    QStringList valu;
    QList<CLASSE *> propos;

    if (id_mere.isEmpty())//probablement inutile
    {
        valu.append(value);
        QList<CLASSE *> c= newExamen->propos();
        c.append(new CLASSE(id,descri,valu,"",QList<CLASSE *>()));
        newExamen->setPropos(c);
        qDebug() << value +" added to parent "+descri +" n "+id;
    }
    else
    {
        while(i < newExamen->propos().length())
        {
            if (newExamen->propos().at(i)->getId()==id.toInt() && newExamen->propos().at(i)->id_mere()==id_mere)
            {
                valu= newExamen->propos().at(i)->values();
                valu.append(value);


                newExamen->propos().at(i)->setValues(valu);
                 qDebug() << value +" added to "+descri +" n "+id+" child of "+id_mere;
                return;
            }
            i++;
        }

        propos= newExamen->propos();
        valu.append(value);
        propos.append(new CLASSE(id,descri,valu,id_mere,QList<CLASSE *>()));
        newExamen->setPropos(propos);
        qDebug("New ajouté");
    }

}

void TRANSFERER::remove(QString id, QString id_mere, QString desc)
{
    int i=0;

    while(i < newExamen->propos().length())
    {
        if (newExamen->propos().at(i)->getId()==id.toInt() && newExamen->propos().at(i)->id_mere()==id_mere)
        {
            newExamen->propos().at(i)->setValues(QStringList());
            qDebug() << "values of n"+id +" "+desc+" child of"+id_mere+ " removed";
            return;
        }
        i++;
    }
}




