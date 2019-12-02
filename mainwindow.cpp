#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QInputDialog"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{

    int p=0;

    while (p<qApp->libraryPaths().length())
    {
        qDebug() << qApp->libraryPaths().at(p);
        p++;
    }
    ui->setupUi(this);
    this->setWindowTitle("HOMISS");
    timerIndicator= new QTimer(this);
    timerIndicator->setInterval(1000);
    canShowError=true;



    writer=new QPdfWriter("");

    xmlManager= new HXmlManager(this);
    awesome = new QtAwesome(this);
    ui->serviceName->setText(xmlManager->getService()->nom());


    this->start();


    setToolbar();


    idicatorChageState(dbm->isConnected());

    //connection de l'utilisateur
    connect(ui->un,SIGNAL(textChanged(QString)),this,SLOT(verifyChamp(QString)));
    connect(ui->pass,SIGNAL(textChanged(QString)),this,SLOT(verifyChamp(QString)));

    //options
    connect(ui->menuFichier,SIGNAL(triggered(QAction*)),this,SLOT(menuAction(QAction*)));
    connect(ui->menuOutils,SIGNAL(triggered(QAction*)),this,SLOT(menuAction(QAction*)));
    connect(ui->mainToolBar,&QToolBar::actionTriggered,this,&MainWindow::menuAction);


    //apllication
    connect(this,SIGNAL(quitter()),qApp,SLOT(quit()));
    connect(timerIndicator,SIGNAL(timeout()),this,SLOT(clignoterIndicator()));

    //parametre
}

MainWindow::~MainWindow()
{
    delete ui;
    delete xmlManager;
    delete timerIndicator;
}

void MainWindow::instantiateDB()
{

    dbm= new DBManager(xmlManager->getDatabaseParameter(DBInfoType::HOSTNAME),xmlManager->getDatabaseParameter(DBInfoType::USERNAME),xmlManager->getDatabaseParameter(DBInfoType::PASSWORD),xmlManager->getDatabaseParameter(DBInfoType::DBNAME),xmlManager->getDatabaseParameter(DBInfoType::PORT).toInt(),this);
    connect(dbm,SIGNAL(userConnection(bool,int)),this,SLOT(connectionResult(bool,int)));//utilisateur connecté
    connect(dbm,SIGNAL(error(QString)),this,SLOT(showDatabaseError(QString)));//erreur sql
    connect(dbm,&DBManager::connectionStateChanged,this,&MainWindow::changeIndicatorConnection);

    dbm->seConnecter();
    if (dbm->isConnected())
    {
        this->changeIndicatorConnection(true);
    }
    else
    {
        this->changeIndicatorConnection(false);
    }
}




void MainWindow::start()
{
    setConnectedUser(new PERSONEL());

    instantiateDB();

    if (dbm->isConnected())
    {
        this->updateListePatient(dbm->getAllPatients());


        ui->statusBar->showMessage("connecté au serveur",2000);
        if (xmlManager->isUserConnected())
        {
            ui->statusBar->showMessage("Récupération des inforamtions du dernier utilisateur");
            setConnectedUser(dbm->getPersonel(xmlManager->getIdConnectedUser()));

            if (!getConnectedUser()->isNull())
            {
                xmlManager->deconnectUser(QString::number(getConnectedUser()->getId()));
                xmlManager->addUserConnected(QString::number(getConnectedUser()->getId()));
                QApplication::beep();
                QMessageBox::information(this,"Reconnection",getConnectedUser()->nom()+" Votre connection du "+xmlManager->getUserConnectedConnexionDate()+" est récupérée connection est récupéré.\n Veuillez déconnecter l'utilisateur et vous reconnecter si ce n'est pas vous.");
                ui->statusBar->showMessage("utilisateur "+getConnectedUser()->nom() +" connecté",5000);
                ui->connectedUser->setText(getConnectedUser()->nom()+  " "+ getConnectedUser()->prenom());
                ui->pages->setCurrentIndex(1);
                this->setButtonState(true);
            }
        }
    }
    else
    {
        ui->statusBar->showMessage("non connecté au serveur");
    }
      this->showMaximized();
}

bool MainWindow::verifId()
{
    if (xmlManager->getAppId().isEmpty())//assigner un identifiant si y en a pas encore
    {
        QString id=generateId();
        xmlManager->setAppId(id);
    }

   bool id=xmlManager->getStartParameter(startParamaterType::ID);

    if (!id)
    {
        if (dbm->setAppId(xmlManager->getAppId()))
        {
            xmlManager->setStartParameters(startParamaterType::ID,false);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return true;
    }


}

class FICHE *MainWindow::getCurrentPatient() const
{
    return m_currentPatient;
}

void MainWindow::setCurrentPatient(class FICHE *currentPatient)
{
    m_currentPatient = currentPatient;
}

bool MainWindow::getAddMode() const
{
    return addMode;
}

void MainWindow::setAddMode(bool value)
{
    addMode = value;
}

EXAMEN *MainWindow::getNewExamen() const
{
    return newExamen;
}

void MainWindow::setNewExamen(EXAMEN *value)
{
    newExamen = value;
}

class FICHE *MainWindow::getNewPatient() const
{
    return newPatient;
}

void MainWindow::setNewPatient(class FICHE *value)
{
    newPatient = value;
}

QList<PATIENT *> *MainWindow::getPatients() const
{
    return m_patients;
}

void MainWindow::setPatients(QList<PATIENT *> *patients)
{
    m_patients = patients;
}

QList<HOPITAL *> *MainWindow::getHopitals() const
{
    return m_hopitals;
}

void MainWindow::setHopitals(QList<HOPITAL *> *hopitals)
{
    m_hopitals = hopitals;
}

QList<STATUT *> *MainWindow::getStatuts() const
{
    return m_statuts;
}

void MainWindow::setStatuts(QList<STATUT *> *statuts)
{
    m_statuts = statuts;
}

QList<ADRESSE *> *MainWindow::getAdresses() const
{
    return m_adresses;
}

void MainWindow::setAdresses(QList<ADRESSE *> *adresses)
{
    m_adresses = adresses;
}

QList<NATIONALITE *> *MainWindow::getNationalites() const
{
    return m_nationalites;
}

void MainWindow::setNationalites(QList<NATIONALITE *> *nationalites)
{
    m_nationalites = nationalites;
}

QList<QUARTIER *> *MainWindow::getQuartier() const
{
    return m_quartier;
}

void MainWindow::setQuartier(QList<QUARTIER *> *quartier)
{
    m_quartier = quartier;
}

QList<VILLE *> *MainWindow::getVilles() const
{
    return m_villes;
}

void MainWindow::setVilles(QList<VILLE *> *villes)
{
    m_villes = villes;
}

QList<MEDECIN *> *MainWindow::getMedecins() const
{
    return m_medecins;
}

void MainWindow::setMedecins(QList<MEDECIN *> *medecins)
{
    m_medecins = medecins;
}

QList<MODESOIN *> *MainWindow::getMode_soins() const
{
    return m_mode_soins;
}

void MainWindow::setMode_soins(QList<MODESOIN *> *mode_soins)
{
    m_mode_soins = mode_soins;
}

QList<MODESORTIE *> *MainWindow::getMode_sortie() const
{
    return m_mode_sortie;
}

void MainWindow::setMode_sortie(QList<MODESORTIE *> *mode_sortie)
{
    m_mode_sortie = mode_sortie;
}

QList<MODEADMISSION *> *MainWindow::getMode_admissions() const
{
    return m_mode_admissions;
}

void MainWindow::setMode_admissions(QList<MODEADMISSION *> *mode_admissions)
{
    m_mode_admissions = mode_admissions;
}


//informatoins d'affichage de détail et ajout et modification

PERSONEL *MainWindow::getConnectedUser() const
{
    return connectedUser;
}

void MainWindow::setConnectedUser(PERSONEL *value)
{
    connectedUser = value;
}

void MainWindow::updateListePatient(QList<PATIENT *> patients)
{

    if (patients.isEmpty())
    {
        ui->actionExporterEnPdf->setEnabled(false);
        ui->actionSortir_un_patient->setEnabled(false);
        ui->actionAjouter_une_Information_Medicale->setEnabled(false);
    }
    else
    {
        ui->actionExporterEnPdf->setEnabled(true);
        ui->actionSortir_un_patient->setEnabled(true);
        ui->actionAjouter_une_Information_Medicale->setEnabled(true);
    }

    QStandardItemModel *model=new QStandardItemModel(patients.length(),NB_COLONNES_PATIENT_LIST,this);
    sortFilter= new QSortFilterProxyModel(this);

    int i=0;

    QStandardItem *item= new QStandardItem();
    while (i<patients.length())
    {

        int j=0;
        while (j<NB_COLONNES_PATIENT_LIST)
        {
            QString value;

            switch (j) {
            case 0: value=QString::number(patients.at(i)->getId());
                break;
            case  1: value=patients.at(i)->nom();
                break;
            case 2: value= patients.at(i)->prenom();
                break;
            case 3: value= patients.at(i)->age();
                break;
            case 4: value= QString::number(dbm->getHospisalisationNumber(patients.at(i)));
                break;
            case 5: value=dbm->getSituation(patients.at(i));
                break;

            }
            item= new QStandardItem(value);

            model->setItem(i,j,item);
            j++;
        }
        i++;
    }

    sortFilter->setSourceModel(model);
    sortFilter->setHeaderData(0,Qt::Orientation::Horizontal,"N°");
    sortFilter->setHeaderData(1,Qt::Orientation::Horizontal,"NOM");
    sortFilter->setHeaderData(2,Qt::Orientation::Horizontal,"PRENOM");
    sortFilter->setHeaderData(3,Qt::Orientation::Horizontal,"AGE");
    sortFilter->setHeaderData(4,Qt::Orientation::Horizontal,"HOSPIPALISATION");
    sortFilter->setHeaderData(5,Qt::Orientation::Horizontal,"SITUATION");



    ui->listePatientView->setModel(model);
    ui->listePatientView->setSortingEnabled(true);
    ui->listePatientView->setColumnWidth(0,30);
    ui->listePatientView->setColumnWidth(3,35);
    ui->listePatientView->setColumnWidth(4,120);

    if (ui->listePatientView->model()->rowCount()!=0)
    {
        ui->listePatientView->selectRow(0);
        updateView(dbm->getFicheOf(ui->listePatientView->indexAt(QPoint(0,0)).data().toString()));
    }
}

void MainWindow::showParametre(int index)
{
    parametreWindow=  new PARAMETRE(this,index);
    connect(parametreWindow,SIGNAL(accepted()),dbm,SLOT(seConnecter()));
//    connect(parametreWindow,&QDialog::destroyed,this,&MainWindow::deconnectParam);


//    connect(parametreWindow,SIGNAL(accepted()),this,SLOT(instantiateDB()));
//    connect(parametreWindow,SIGNAL(applied()),this,SLOT(instantiateDB()));

    parametreWindow->exec(first);
}

void MainWindow::setButtonState(bool state)
{
    ui->actionDeconnecter->setEnabled(state);
    ui->actionEtudeStat->setEnabled(state);
    ui->actionFiche->setEnabled(state);
    ui->actionExporterEnPdf->setEnabled(state);
    ui->actionFiche->setEnabled(state);
    ui->actionImprimer->setEnabled(state);
    ui->actionRecevoirPatient->setEnabled(state);
    ui->actionServer->setEnabled(state);
    ui->actionTransfererPatient->setEnabled(state);

    ui->actionDeconnecter->setEnabled(state);
    ui->actionAjouter_une_Information_Medicale->setEnabled(state);
    ui->actionSortir_un_patient->setEnabled(state);
    ui->actionExporterEnPdf->setEnabled(state);
    ui->lineEdit->setEnabled(state);
    ui->filtre->setEnabled(state);
}

void MainWindow::updateView(QList<class FICHE*> newFiche)//update du vue de la vue des détails de patient
{
    int i=0;
    current= newFiche;
    ui->tabWidget->clear();


    PATIENTVIEW *ne;
    QScrollArea *scroll;
    QVBoxLayout *lay;
    QVBoxLayout *scrolledLayout;

    if (newFiche.length()>0)
    {
        setCurrentPatient(newFiche.last());
    }
    while(i <newFiche.length())
    {
        ne=new PATIENTVIEW(newFiche.at(i),ui->tabWidget);
        connect(ne,&PATIENTVIEW::addExam,this,&MainWindow::addExamFor);

        ui->actionSortir_un_patient->setEnabled(!ne->getIsExited());
        ui->actionAjouter_une_Information_Medicale->setEnabled(!ne->getIsExited());

        scroll = new QScrollArea(this);
        lay= new QVBoxLayout();
        scrolledLayout = new QVBoxLayout();

//        lay->addWidget(scroll);
//        scroledLayout->addWidget(ne);
//        ui->tabWidget->setLayout(scroledLayout);
//        lay->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);

//        scroll->setLayout(scroledLayout);

        scrolledLayout->addWidget(ne);
        scroll->setWidgetResizable(true);
        scroll->setWidget(ne);
//        scroll->setLayout(lay);
        scroll->setBackgroundRole(QPalette::Light);

//
//        ui->tabWidget->setLayout(lay);


        ui->tabWidget->addTab(scroll,newFiche.at(i)->getDate_admission().date().toString("dd/MMM/yyyy"));




        ui->tabWidget->widget(i)->setBackgroundRole(QPalette::Light);
        i++;
    }

//    ui->tabWidget->setLayout(new QVBoxLayout());

}

void MainWindow::menuAction(QAction *act)//slot qui reçois tout les actions des menus et agir par rapport
{
    if (act->objectName()=="actionQuitter")
    {
        if(QMessageBox::question(this,"Quitter","Voulez vous vraiment quitter?")==QMessageBox::Yes)
        {
            qApp->exit();
        }
    }
    else if(act->objectName()=="actionServer")
    {
        showParametre();
    }
    else if(act->objectName()=="actionFiche")
    {
        showParametre(1);
    }
    else if(act->objectName()=="actionPreference")
    {
        showParametre(2);
    }
    else if (act->objectName()=="actionDeconnecter" || act->objectName()=="actionSeDeconnecter")
    {
        if (QMessageBox::question(this,"Quitter","Voulez vous vraiment vous déconnecter?")==QMessageBox::Yes)
        {
            closeAdd();
            xmlManager->deconnectUser(QString::number(getConnectedUser()->getId()));
            ui->connectedUser->setText("");
            ui->pages->setCurrentIndex(0);
            ui->mixte->setCurrentIndex(0);
            ui->listePatientView->setVisible(true);
            this->setButtonState(false);
            delete connectedUser;
            this->setConnectedUser(new PERSONEL());
            ui->un->clear();
            ui->pass->clear();
            ui->statusBar->showMessage("Utilisateur Déconnecté",5000);

        }
        ui->statusBar->showMessage("Déconnection annulé",5000);
    }
    else if (act->objectName()=="actionExporterEnPdf")
    {
        QString emplacement= QFileDialog::getSaveFileName(this,"Choisir un emplacement",QString(),"PDF (*.pdf)");
        exportToPdf(ui->tabWidget->currentWidget(),emplacement);
    }

    else if(act->objectName()=="actionSortir_un_patient")
    {
        transfertView= new TRANSFERER(getCurrentPatient(),this);
        transfertView->show();
    }
    else if (act->objectName()=="actionEtudeStat")
    {
        startStat();
    }
    else if (act->objectName()=="actionAjouter_une_Information_Medicale")
    {

    }
}

void MainWindow::showDatabaseError(QString err)
{
    QMessageBox::critical(this,"Erreur",err);

//    if (canShowError)
//    {
//        canShowError=false;
//        QMessageBox::critical(this,"Erreur",err);
//        QTimer::singleShot(3000,this,SLOT(enableShowError()));
//    }
}

void MainWindow::changeIndicatorConnection(bool state)
{
    //swicher entre deux images et désactiver ou activer certaine boutons
    idicatorChageState(state);

    if(!state)
    {
        ui->connexionbtn->setEnabled(false);
    }
    else
    {
        this->verifyChamp("");
    }
}

PARAMETRE *MainWindow::getParametreWindow() const
{
    return parametreWindow;
}

void MainWindow::on_connexionbtn_clicked()
{
    if (ui->un->text().isEmpty() || ui->pass->text().isEmpty())
    {
        QMessageBox::information(this,"Information","Veuillez remplir tout les champs");
    }
    else
    {
        dbm->userConnectionApp(ui->un->text(),ui->pass->text());
    }
}

void MainWindow::verifyChamp(QString champValue)
{
    if (ui->un->text().isEmpty() || ui->pass->text().isEmpty())
    {
        ui->connexionbtn->setEnabled(false);
    }
    else if (!ui->un->text().isEmpty() && !ui->pass->text().isEmpty())
    {
        ui->connexionbtn->setEnabled(true);
    }
}

void MainWindow::connectionResult(bool isConnected, int idUser)
{
    if (isConnected)
    {

        setButtonState(true);
        this->setConnectedUser(dbm->getPersonel(QString::number(idUser)));
        ui->connectedUser->setText(getConnectedUser()->nom()+" "+ getConnectedUser()->prenom());


        if (!xmlManager->getStartParameter(startParamaterType::GLOBAL))
        {
            if (xmlManager->getStartParameter(startParamaterType::ID))
            {
                if (dbm->setAppId(xmlManager->getAppId()))
                {

                    xmlManager->setStartParameters(startParamaterType::ID,false);
                    qDebug("done");
                }
                else
                {
                    qDebug("non conf");
                }
            }
            if ( xmlManager->getStartParameter(startParamaterType::SER))
            {
                s=new SERVICE_UI(this);
                s->setList(dbm->getServices("1"));
                s->exec();
            }

            if (xmlManager->getStartParameter(startParamaterType::FICHE))
            {
                qDebug("start to get classes from database");
                dbm->setModelFromDB();//on instantie venant de la base de donné une seule fois
                showParametre(1);
            }


        }

        QApplication::beep();
        ui->pages->setCurrentIndex(1);
        QMessageBox::information(this,"Connecté",getConnectedUser()->nom() +",Vous êtes désoramis connecté et toutes les actions serons à votres nom");
    }
    else
    {
        QMessageBox::warning(this,"Login error","Login innexistant, veuillez bien verifier");
    }
}

void MainWindow::idicatorChageState(bool state)
{
    if (state==true)
    {
        timerIndicator->stop();
        ui->indicator->setPixmap(QPixmap(":/images/connected"));
        ui->indicator->setVisible(true);
    }
    else
    {
        ui->indicator->setPixmap(QPixmap(":/images/notConnected"));
        timerIndicator->start();
    }
}

void MainWindow::clignoter(QWidget &wid)
{
    if (wid.isVisible())
    {
        wid.setVisible(false);
    }
    else
    {
        wid.setVisible(true);
    }
}

void MainWindow::enableShowError()
{
    canShowError=true;
}

void MainWindow::clignoterIndicator()
{
    clignoter(*ui->indicator);
}

void MainWindow::fatalErrorAction(QString error)
{
    QMessageBox::critical(this,"Fatal Error","une erreur s'est produite, l'application va se fermer.\n Détails: "+error);
    emit fatalError();
}

void MainWindow::setToolbar()
{
//
}

QString MainWindow::generateId()
{
    QRandomGenerator *gen= new QRandomGenerator();
    int i=0;
    QString res="";
    while (i<100)
    {
        int rand=0;

        while (rand==0 || rand==34 || rand==39)
        {
            rand= (int) gen->bounded(33,126);
        }


        char r(rand);
        res+=r;
        i++;
    }

    return res;
}


//gestion de l'ajout d'un nouveau patient

void MainWindow::on_ConfirmationPatientBtn_clicked(QAbstractButton *button)
{
    if (ui->ConfirmationPatientBtn->buttonRole(button)==QDialogButtonBox::AcceptRole)
    {
        if (getAddMode())
        {
            if (isSavable())
            {
                ui->statusBar->showMessage("Enregistrement du patient");

                if (ui->modeAdmission->currentText()!="TRANSFERT")
                {
                    getNewPatient()->getProvenance()->setId(-1);
                }

                if (dbm->addFiche(getNewPatient()))
                {
                    ui->statusBar->showMessage("success",5000);
                    QMessageBox::information(this,"Succeès","Le patient a été bien ajouté");

                    updateListePatient(dbm->getAllPatients());
                    ui->mixte->setCurrentIndex(0);
                    deleteInfoMedicaleViewOnAdd();
                    ui->listePatientView->setVisible(true);
                    setButtonState(true);
                    closeAdd();
                    deleteAllDataForAdd();

                }
                else
                {
                     ui->statusBar->showMessage("échec",5000);
                    QMessageBox::critical(this,"Erreur","Le patient n'est pas ajouté!");
                }
            }
            else
            {
                if (getAddMode())
                {
                    QMessageBox::warning(this,"Invalide","Vous devez remplir les minimales d'information!\n Nom,Prenom,nationlaite,...");
                }
            }

        }
        else
        {
            bool cl=true;
            if (isExamEdited)
            {
               cl=QMessageBox::Yes==QMessageBox::question(this,"Abandonner","Voulez vous abbandonner Les modifications?");
            }
            if (cl)
            {
                ui->listePatientView->setVisible(true);
                closeAdd();
                deleteInfoMedicaleViewOnAdd();
                deleteAllDataForAdd();
                ui->mixte->setCurrentIndex(0);
            }
        }

    }
    else if (ui->ConfirmationPatientBtn->buttonRole(button)==QDialogButtonBox::RejectRole)
    {
        //Vérifier les saisies déjà faites et de mander si l'utilisateur veut abandonner les saisies

        if (getAddMode())
        {
            if (QMessageBox::Yes==QMessageBox::question(this,"Abandonner","Voulez vous abbandonner l'ajout?"))
            {
                ui->mixte->setCurrentIndex(0);
                ui->listePatientView->setVisible(true);
                deleteInfoMedicaleViewOnAdd();
                closeAdd();
                deleteAllDataForAdd();
            }
        }
        else if (isExamEdited)
        {
            if (QMessageBox::Yes==QMessageBox::question(this,"Abandonner","Voulez vous abbandonner Les modifications?"))
            {
                ui->mixte->setCurrentIndex(0);
                ui->listePatientView->setVisible(true);
                deleteInfoMedicaleViewOnAdd();
                closeAdd();
                deleteAllDataForAdd();

            }
        }
    }
}

void MainWindow::on_newPatientBtn_clicked()
{
    setAddMode(true);
    configureAddPatientView(new class FICHE());
    ui->mixte->setCurrentIndex(1);
    ui->listePatientView->setVisible(false);
}

void MainWindow::on_listePatientView_clicked(const QModelIndex &index)
{
    this->setCurrentPatient(dbm->getFicheOf(index.siblingAtColumn(0).data().toString()).last());
    updateView(dbm->getFicheOf(index.siblingAtColumn(0).data().toString()));
}

void MainWindow::updateInfoMedicaleViewOnAdd(class FICHE *f)//mise à jour des formulaires des informations médicales pendant une modif
{
    deleteInfoMedicaleViewOnAdd();//supprime tout les formulaire si il la fenêtre a été déjà ouverte avant
    int m=0;
    QList<QDomElement> super= xmlManager->getSuperMenus();//récupération des super classe dans le ficher de config
    EXAMLIGNE *ne;
    QList<CLASSE *> defaultClasses;

    while (m<super.length())
    {
       QList<QDomElement > vals= xmlManager->getValuesOfMenu(super.at(m).attribute("id"));
       QStringList v;
       QStringList s;
       int k=0;
       while (k<vals.length())
       {
           v.append(vals.at(k).attribute("content"));
           k++;
       }
       k=0;
       vals= xmlManager->getMenuChilds(super.at(m).attribute("id"));
       while (k<vals.length())
       {
           s.append(vals.at(k).attribute("id"));
           k++;
       }
       //Instantiation des sous classes
       ne=new EXAMLIGNE(super.at(m).attribute("id"),super.at(m).attribute("descriptif"), super.at(m).attribute("nbVal").toInt(),v, super.at(m).attribute("Alias"), super.at(m).attribute("remplissage"),s,super.at(m).attribute("valueModifiable")=="true"?true:false, super.at(m).attribute("valueInverted")=="true"? true:false,QString(),ui->groupBox_2);
       connect(ne,&EXAMLIGNE::set,this,&MainWindow::recept);
       connect(ne,&EXAMLIGNE::clear,this,&MainWindow::remove);
//       ne->getDefault();
       QStringList defVal;//le type est liste car les valeurs d'une classe est une liste même que on dois faire oui seulement pour touss les base
       defVal.append("oui");
       defaultClasses.append(new CLASSE(super.at(m).attribute("id"),super.at(m).attribute("descriptif"),defVal,"",QList<CLASSE *>()));
       ui->containerInfoMed->addWidget(ne);




       //mettre ici tout les contraites
       if (super.at(m).attribute("descriptif").toUpper()=="TRANSFERT")
       {
           ne->setEnabled(false);
       }
       m++;
    }

    //récupérationde des examens de base si on est en mode ajout
    if (getAddMode())
    {
        EXAMEN *n= new EXAMEN();
        n->setPropos(defaultClasses);
        QList<EXAMEN *> swchExam=getNewPatient()->getExamens();
        swchExam.append(n);
        getNewPatient()->setExamens(swchExam);
    }
    else
    {
       defaultClasses.clear();
    }


}

void MainWindow::configureAddPatientView(class FICHE *f)
{

    changeBtnState(false);
    instantiateAllDataForAdd();

    ui->dateAdmission->setMaximumDateTime(QDateTime::currentDateTime());
    ui->dateAdmission->setDateTime(QDateTime::currentDateTime());
    QDateTime max(QDateTime::currentDateTime());
    QDate d;
    d.setDate(1,1,max.date().year()-200);
    ui->dateAdmission->setMinimumDate(d);

    updateInfoMedicaleViewOnAdd(f);//rechargement des vues et formulaires
    ui->groupBox_2->setVisible(false);
    ui->groupBox_3->setVisible(false);


    if (!getAddMode())
    {
        closeAdd();
    }


    if (getQuartier()->length()>0)
    {
        ui->quartier->setCurrentIndex(0);
    }

    int i=0;

    while (i<getMedecins()->length())
    {
        ui->medecinResponsable->addItem(getMedecins()->at(i)->nom()+ " "+getMedecins()->at(i)->prenom());

        i++;
    }

    i=0;

    while (i < getMode_admissions()->length())
    {
        ui->modeAdmission->addItem(getMode_admissions()->at(i)->getLabel());
        i++;
    }

    i=0;

    while(i < getMode_soins()->length())
    {
        ui->modeSoin->addItem(getMode_soins()->at(i)->getLabel());
        i++;
    }

    i=0;

    while(i <getNationalites()->length())
    {
        ui->nationalite->addItem(getNationalites()->at(i)->getLabel());
        i++;
    }

    i=0;

    while(i < getVilles()->length())
    {
        ui->ville->addItem(getVilles()->at(i)->nom());
        i++;
    }

    i=0;

    while(i < getStatuts()->length())
    {
        ui->statut->addItem(getStatuts()->at(i)->getLabel());
        i++;
    }

    i=0;

    while(i< getHopitals()->length())
    {
        ui->hopitalCombo->addItem(getHopitals()->at(i)->getNom());

        i++;
    }

    if (!getAddMode())
    {

        this->setNewPatient(new class FICHE(*f));
        qDebug() << "okzu";
        this->rewritePatient(getNewPatient());
        qDebug("entree");
        ui->addExamenBtn->click();
        qDebug("sortie");
    }
    else
    {
        ui->ville->setCurrentText("ANTANANARIVO");
        ui->hopitalCombo->setCurrentIndex(0);
        ui->service->setCurrentIndex(0);
        ui->nationalite->setCurrentText("MALAGASY");
        ui->sexe_2->setCurrentIndex(0);
        ui->nom->setText("");
        ui->prenom->setText("");
        ui->statut->setCurrentIndex(0);
        ui->medecinResponsable->setCurrentIndex(0);
        ui->modeSoin->setCurrentIndex(0);
        ui->modeAdmission->setCurrentIndex(0);

    }


}

void MainWindow::addExamFor(class FICHE *f)
{
    setAddMode(false);
    qDebug("configration view");
    configureAddPatientView(f);
    qDebug("configration view done");

    ui->mixte->setCurrentIndex(1);
    qDebug("a");
    ui->listePatientView->setVisible(false);
    qDebug("a");
}


void MainWindow::updateCompleterLot(QUARTIER *quartier)
{
    this->setAdresses(new QList<ADRESSE*> (dbm->getAllAdresses(QString::number(quartier->getId()))));
    QStringList listA;

    int i=0;

    while (i < getAdresses()->length())
    {
        listA.append(getAdresses()->at(i)->lot());
        i++;
    }
    ui->lot->clear();
    ui->lot->setCompleter(new QCompleter(listA));
    ui->lot->completer()->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
}

void MainWindow::updateCompleterQuartier(VILLE *ville)
{
    ui->quartier->clear();
    this->setQuartier(new QList<QUARTIER*>(dbm->getAllQuatierOfVille(QString::number(ville->getId()))));
    int i=0;

    while(i < getQuartier()->length())
    {
        ui->quartier->addItem(getQuartier()->at(i)->nom());
        i++;
    }

}

void MainWindow::updateCompleterService(HOPITAL *hopital)
{
    int j=0;
    ui->service->clear();

    while(j<hopital->services().length())
    {
        ui->service->addItem(hopital->services().at(j)->nom());
        j++;
    }

}

void MainWindow::deleteInfoMedicaleViewOnAdd()
{
    int m=0;
    QList<EXAMLIGNE *> wid = ui->groupBox_2->findChildren<EXAMLIGNE *>(QString(),Qt::FindDirectChildrenOnly);
    int len=wid.length();
    while(m<len)
    {
        delete wid.at(m);
        m++;
    }
}

void MainWindow::instantiateAllDataForAdd()
{
    isExamEdited=false;
    newPatient= new class FICHE();//fiche pour stoquer les nouveaux infos
    newExamen= new EXAMEN();


    this->setMedecins(new QList<MEDECIN*> (dbm->getAllMedecinsFromService(QString::number(xmlManager->getService()->getId()))));
    this->setMode_admissions(new QList<MODEADMISSION*> (dbm->getAllModeAdmission()));
    this->setMode_soins(new QList<MODESOIN*> (dbm->getAllModeSoin()));
    this->setMode_sortie( new QList<MODESORTIE*>(dbm->getAllModeSortie()));
    this->setNationalites(new QList<NATIONALITE*> (dbm->getAllNationalites()));
    this->setStatuts(new QList<STATUT*> (dbm->getAllStatut()));
    this->setVilles(new QList<VILLE*> (dbm->getAllVille()));
    this->setHopitals(new QList<HOPITAL*> (dbm->getAllHopital()));
    this->setPatients(new QList<PATIENT*> (dbm->getAllPatients()));
    this->setQuartier(new QList<QUARTIER *>());
    this->setAdresses(new QList<ADRESSE *>());//non utilisé
}

void MainWindow::deleteAllDataForAdd()
{
    qDebug("Début de suppression des données d'Ajout");
    delete m_mode_admissions;
    qDebug("2");
    delete m_mode_sortie;
    qDebug("3");
    delete m_mode_soins;
    qDebug("4");
    delete m_medecins;
    qDebug("5");
    delete m_villes;
    qDebug("6");
    delete m_quartier;
    qDebug("7");
    delete m_nationalites;
    qDebug("8");
    delete m_adresses;
    qDebug("9");
    delete m_statuts;
    qDebug("10");
    delete m_hopitals;
    qDebug("11");
    delete newPatient;
    qDebug("12");
    delete newExamen;
    qDebug("Fin de suppression de données d'enregistrement");
}

bool MainWindow::isSavable()
{
    if (ui->nom->text().isEmpty() || ui->prenom->text().isEmpty() || ui->nationalite->currentText().isEmpty() )
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool MainWindow::exportToPdf(QWidget *w, QString dir)
{
    QPixmap m(w->size());
    QPainter p(&m);
    w->render(&p);
    p.end();
   writer=new QPdfWriter(dir);
   writer->setPageSize(QPrinter::A4);
   p.begin(writer);

//   this->update();

}

void MainWindow::paintEvent(QPaintEvent *)
{

//    QPainter painter(writer);
//    painter.setPen(QColor::Rgb);
//    painter.begin(ui->listePatientView);

}

void MainWindow::on_quartier_currentIndexChanged(int index)
{
    if (index!=-1 && index<getQuartier()->length())
    {
        updateCompleterLot(getQuartier()->at(index));
        getNewPatient()->proprietire()->adresse()->setQuartier(getQuartier()->at(index));
    }
    else
    {
        getNewPatient()->proprietire()->adresse()->setQuartier(new QUARTIER(-1,ui->quartier->currentText(),new VILLE(-1,ui->ville->currentText())));
        ui->lot->setCompleter(new QCompleter());
    }

    qDebug() << "Patient quaritier: " +QString::number(getNewPatient()->proprietire()->adresse()->quartier()->getId())+" "+ getNewPatient()->proprietire()->adresse()->quartier()->nom();

}


void MainWindow::on_quartier_currentTextChanged(const QString &arg1)
{
    if (ui->quartier->currentIndex()!=-1 && ui->quartier->currentIndex()<getQuartier()->length())
    {
        updateCompleterLot(getQuartier()->at(ui->quartier->currentIndex()));
        getNewPatient()->proprietire()->adresse()->setQuartier(getQuartier()->at(ui->quartier->currentIndex()));
    }
    else
    {
        getNewPatient()->proprietire()->adresse()->setQuartier(new QUARTIER(-1,arg1,new VILLE(-1,ui->ville->currentText())));
        ui->lot->setCompleter(new QCompleter());
    }
}

void MainWindow::on_ville_currentIndexChanged(int index)
{
    if (index!=-1)
    {
        updateCompleterQuartier(getVilles()->at(index));
        getNewPatient()->proprietire()->adresse()->quartier()->setVille(getVilles()->at(index));
    }
    else
    {
        updateCompleterQuartier(new VILLE());
        getNewPatient()->proprietire()->adresse()->quartier()->setVille(new VILLE(-1,ui->ville->currentText()));
    }

}

void MainWindow::on_ville_currentTextChanged(const QString &arg1)
{
    qDebug() << "current ville text "+arg1;
    qDebug() << "current ville index "+QString::number(ui->ville->currentIndex());

    if (ui->ville->currentIndex()!=-1)
    {
        updateCompleterQuartier(getVilles()->at(ui->ville->currentIndex()));
        getNewPatient()->proprietire()->adresse()->quartier()->setVille(getVilles()->at(ui->ville->currentIndex()));
    }
    else
    {
        updateCompleterQuartier(new VILLE());
        getNewPatient()->proprietire()->adresse()->quartier()->setVille(new VILLE(-1,ui->ville->currentText()));
    }
}

void MainWindow::on_hopitalCombo_currentIndexChanged(int index)
{
    qDebug() << "index hopital "+QString::number(index);
    if (index!=-1 && index < getHopitals()->length())
    {
        if (ui->hopitalCombo->currentText()=="CENHOSOA")
        {
            ui->service->setEditable(false);
            ui->medecinResponsable->setEnabled(true);
        }
        else
        {
            ui->medecinResponsable->setEnabled(false);
            ui->service->setEditable(true);
        }
        updateCompleterService(getHopitals()->at(index));
    }
    else
    {
        updateCompleterService(new HOPITAL());
    }
}

void MainWindow::on_hopitalCombo_currentTextChanged(const QString &arg1)
{
    if (ui->hopitalCombo->currentIndex()!=-1 && ui->hopitalCombo->currentIndex() < getHopitals()->length())
    {
        if (ui->hopitalCombo->currentText()=="CENHOSOA")
        {
            ui->service->setEditable(false);
        }
        else
        {
            ui->service->setEditable(true);
        }
        updateCompleterService(getHopitals()->at(ui->hopitalCombo->currentIndex()));
    }
    else
    {
       updateCompleterService(new HOPITAL());
    }
}

void MainWindow::on_service_currentTextChanged(const QString &arg1)
{
    qDebug() <<"Index of serrvice "+QString::number(ui->service->currentIndex());
    if (ui->service->currentIndex()!=-1 && ui->hopitalCombo->currentIndex()!=-1)
    {
//        if (ui->service->currentIndex()!=-1/*&&ui->service->currentIndex()<getHopitals()->at(ui->hopitalCombo->currentIndex())->services().length()*/)
//        {
//            getNewPatient()->setProvenance(getHopitals()->at(ui->hopitalCombo->currentIndex())->services().at(ui->service->currentIndex()));
//        }
//        else
//        {
//            getNewPatient()->setProvenance(new SERVICE(-1,arg1,ui->hopitalCombo->currentText()));
//        }

        getNewPatient()->setProvenance(getHopitals()->at(ui->hopitalCombo->currentIndex())->services().at(ui->service->currentIndex()));


    }
    else
    {
        getNewPatient()->setProvenance(new SERVICE(-1,arg1,ui->service->currentText()));
    }
}

void MainWindow::on_service_currentIndexChanged(int index)
{
    qDebug() <<"Index of serrvice "+QString::number(index);
    if (index!=-1 && ui->hopitalCombo->currentIndex()!=-1)
    {
//        if (index!=-1&&ui->service->currentIndex()<getHopitals()->at(ui->hopitalCombo->currentIndex())->services().length())
//        {
//            getNewPatient()->setProvenance(getHopitals()->at(index)->services().at(ui->service->currentIndex()));
//        }
//        else
//        {
//            getNewPatient()->setProvenance(new SERVICE(-1,ui->service->currentText(),ui->hopitalCombo->currentText()));
//        }

        getNewPatient()->setProvenance(getHopitals()->at(ui->hopitalCombo->currentIndex())->services().at(index));

    }
    else
    {
        getNewPatient()->setProvenance(new SERVICE(-1,ui->service->currentText(),ui->hopitalCombo->currentText()));

    }
}

void MainWindow::on_nom_editingFinished()
{
    this->getNewPatient()->proprietire()->setNom(ui->nom->text());
    PATIENT *p= isPatientLastHospitalised(getNewPatient()->proprietire());
    if (!p->isNull())
    {
        if (rewritePatient(getNewPatient()))
        {
            getNewPatient()->setProprietire(p);
            ui->age->setFocus();
        }
    }
}

void MainWindow::on_prenom_editingFinished()
{
    this->getNewPatient()->proprietire()->setPrenom(ui->prenom->text());
    PATIENT *p= isPatientLastHospitalised(getNewPatient()->proprietire());
    if (!p->isNull())
    {
       if (rewritePatient(getNewPatient()))
       {
           getNewPatient()->setProprietire(p);
           ui->age->setFocus();
       }
    }
}

PATIENT *MainWindow::isPatientLastHospitalised(PATIENT *pat)
{
    int i=0;

    while(i < getPatients()->length())
    {
        if (pat->nom().toUpper()==getPatients()->at(i)->nom().toUpper() && pat->prenom().toUpper()==getPatients()->at(i)->prenom().toUpper())
        {
            return getPatients()->at(i);
        }
        i++;
    }
    return  new PATIENT();
}

bool MainWindow::rewritePatient(class FICHE *f)
{

    qDebug("Start rewrite");
    f->proprietire();
    qDebug("step 1");
    QString sexe=f->proprietire()->genre()==HOMME? "Homme":"Femme";
    qDebug("step 2");
//    ui->age->setValue(f->proprietire()->age().toInt());
    qDebug("step 3");
    int ok;

    if (getAddMode())
    {
        ok=QMessageBox::question(this,"Nom Existant","Le patient "+ui->nom->text()+ " "+ui->prenom->text()+" figure déjà dans la base de donnée.\n Informations: age avant "+ f->proprietire()->age() +" ,sexe "+sexe+".\n Est ce que c'est la même personne?");
    }
    else
    {
        ok=QMessageBox::Yes;
    }


    if (ok==QMessageBox::Yes)
    {
        qDebug("a");
        getNewPatient()->setProprietire(f->proprietire());
        qDebug("b");
        ui->age->setFocus();
        qDebug("c");
        if(sexe=="Homme")
        {
            ui->sexe_2->setCurrentIndex(0);
        }
        else
        {
            ui->sexe_2->setCurrentIndex(1);
        }
        qDebug("d");
        ui->ville->setCurrentText(f->proprietire()->adresse()->quartier()->ville()->nom());
        qDebug("e");
        ui->quartier->setCurrentText(f->proprietire()->adresse()->quartier()->nom());
        qDebug("f");
        ui->lot->setText(f->proprietire()->adresse()->lot());
        qDebug("g");
        if (!f->proprietire()->contacts().isEmpty())
        {
            ui->contact->setText(f->proprietire()->contacts().at(0)->getLabel());
        }

        qDebug("h");
        ui->statut->setCurrentText(f->proprietire()->getStatut()->getLabel());
        ui->nationalite->setCurrentText(f->proprietire()->nationalite()->getLabel());
        ui->proffession->setCurrentText(f->proprietire()->getProffession());
        ui->age->setValue(f->proprietire()->age().toInt());


        if (!getAddMode())
        {
            ui->nom->setText(f->proprietire()->nom());
            ui->prenom->setText(f->proprietire()->prenom());
            ui->quartier->setCurrentText(f->proprietire()->adresse()->quartier()->nom());
            ui->dateAdmission->setDateTime(f->getDate_admission());
            ui->modeAdmission->setCurrentText(f->modeadmission()->getLabel());
            ui->modeSoin->setCurrentText(f->modesoin()->getLabel());
            ui->medecinResponsable->setCurrentText(f->getResponsble()->nom()+" "+f->getResponsble()->prenom());
            ui->hopitalCombo->setCurrentText(f->getProvenance()->nomHopital());
            ui->service->setCurrentText(f->getProvenance()->nom());
            ui->civiliteGB->setEnabled(false);
            ui->groupBox->setEnabled(false);
            ui->ProvenanceGB->setEnabled(false);
        }
        return true;
    }
    return false;
}

void MainWindow::closeAdd()//ne pas mettre des donnée pointers de la classe inci
{

    ui->groupBox->setEnabled(true);
    ui->ProvenanceGB->setEnabled(true);
    ui->civiliteGB->setEnabled(true);

    ui->nom->clear();
    ui->prenom->clear();
    ui->contact->clear();
    ui->proffession->clear();
    ui->lot->clear();
    ui->nationalite->clear();
    ui->quartier->clear();
    ui->ville->clear();
    ui->medecinResponsable->clear();
    ui->modeAdmission->clear();
    ui->modeSoin->clear();
    ui->statut->clear();
    ui->dateTimeEdit->clear();
    ui->service->clear();
    ui->hopitalCombo->clear();
    ui->responsablePrincipalCheck->setChecked(false);
    ui->examinateur->clear();
    changeBtnState(true);
}

void MainWindow::changeBtnState(bool state)
{
    ui->lineEdit->setEnabled(state);
    ui->filtre->setEnabled(state);
    ui->actionAjouter_une_Information_Medicale->setEnabled(state);
    ui->actionSortir_un_patient->setEnabled(state);
    ui->actionRecevoirPatient->setEnabled(state);
    ui->actionTransfererPatient_2->setEnabled(state);
}

void MainWindow::on_age_editingFinished()
{
    getNewPatient()->proprietire()->setAge(QString::number(ui->age->value()));
}

void MainWindow::on_sexe_2_currentIndexChanged(int index)
{
    getNewPatient()->proprietire()->setGenre(index==0? HOMME:FEMME);
}

void MainWindow::on_nationalite_currentTextChanged(const QString &arg1)
{
    if (ui->nationalite->currentIndex()!=-1)
    {
        getNewPatient()->proprietire()->setNationalite(getNationalites()->at(ui->nationalite->currentIndex()));
    }
    else
    {
        getNewPatient()->proprietire()->setNationalite(new NATIONALITE(-1,arg1));
    }

}
void MainWindow::on_nationalite_currentIndexChanged(int index)
{
    if (index!=-1)
    {
        getNewPatient()->proprietire()->setNationalite(getNationalites()->at(index));
    }
    else
    {
        getNewPatient()->proprietire()->setNationalite(new NATIONALITE(-1,ui->nationalite->currentText()));
    }
}


void MainWindow::on_proffession_currentTextChanged(const QString &arg1)
{
    getNewPatient()->proprietire()->setProffession(arg1);
}

void MainWindow::on_statut_currentIndexChanged(int index)
{
    if (index!=-1)
    {
        getNewPatient()->proprietire()->setStatut(getStatuts()->at(index));
    }
}


void MainWindow::on_contact_editingFinished()
{//un patien ne possède qu'un contacte mais comme il hérite de personne, ils est possible passer plusieurs contacts.
    QList<CONTACT *> c;
    c.append(new CONTACT(-1,ui->contact->text()));
    getNewPatient()->proprietire()->setContacts(c);
}

void MainWindow::on_lot_editingFinished()
{
    getNewPatient()->proprietire()->adresse()->setLot(ui->lot->text());
}

void MainWindow::on_dateAdmission_dateTimeChanged(const QDateTime &dateTime)
{
    getNewPatient()->setDate_admission(ui->dateAdmission->dateTime());
}

void MainWindow::on_modeAdmission_currentIndexChanged(int index)
{
    if (index!=-1)
    {
        getNewPatient()->setModeadmission(getMode_admissions()->at(index));
        if (index==1)
        {
            ui->hopitalCombo->setEnabled(true);
            ui->service->setEnabled(true);
        }
        else
        {
            ui->hopitalCombo->setEnabled(false);
            ui->service->setEnabled(false);
        }
    }
}

void MainWindow::on_modeSoin_currentIndexChanged(int index)
{
    if (index!=-1)
    {
        getNewPatient()->setModesoin(getMode_soins()->at(index));
    }

}

void MainWindow::on_medecinResponsable_currentIndexChanged(int index)
{
    if (index!=-1)
    {
        getNewPatient()->setResponsble(getMedecins()->at(index));
    }

}

void MainWindow::on_addExamenBtn_clicked()
{
    if (!ui->groupBox_3->isVisible())
    {
        isExamEdited=false;
        ui->groupBox_3->setVisible(true);

        ui->groupBox_2->setVisible(true);

        ui->examinateur->clear();


        newExamen= new EXAMEN();//

        ui->examinateur->setCurrentIndex(0);

        newExamen->setDate(ui->dateTimeEdit->dateTime());

        int i=0;
        while(i < getMedecins()->length())
        {
            ui->examinateur->addItem(getMedecins()->at(i)->nom()+ " "+getMedecins()->at(i)->prenom());
            i++;
        }

        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
        ui->dateTimeEdit->setMaximumDateTime(QDateTime::currentDateTime());
        ui->dateTimeEdit->setMaximumDateTime(getNewPatient()->getDate_admission());

        ui->statusBar->showMessage("Prêt à enregistrer les Examens faits par "+ui->examinateur->currentText(),6000);

    }

    else
    {
        if (isExamEdited)
        {

            ui->groupBox_3->setVisible(false);

            ui->groupBox_2->setVisible(false);
            qDebug() << QString::number(getNewPatient()->getId()) +" Nb new Classe "+QString::number(getNewExamen()->propos().length());

            if (!getNewExamen()->propos().isEmpty() && !getAddMode())
            {
                qDebug("update");

                QList<EXAMEN *> l;
                l.append(getNewExamen());
                if (!dbm->addExam(QString::number(getNewPatient()->getId()),l))
                {
                    QMessageBox::warning(this,"Erreur","non enregistré");
                    ui->groupBox_3->setVisible(true);
                    return;//ne rien faire d'autre si la fiche est pas enrigistré au complet
                }
            }
            this->addExamToFiche(newExamen);//sans utilité pour la mise à jour

            ui->statusBar->showMessage("Examen Enregistré",6000);

            isExamEdited=false;

            ui->groupBox_2->setVisible(false);
            ui->groupBox_3->setVisible(false);
            ui->examinateur->setCurrentIndex(0);
            ui->addExamenBtn->setText("Ajouter un examen");
            qDebug() << "test "+ QString::number(newPatient->getExamens().length());

        }
        else
        {
             ui->statusBar->showMessage("Prêt à enregistrer les Examens faits par "+ui->examinateur->currentText(),6000);
        }

    }

}

void MainWindow::addExamToFiche(EXAMEN *exam)
{
    if (!exam->propos().isEmpty())
    {
        QList<EXAMEN *> examens= newPatient->getExamens();
        examens.append(exam);
        newPatient->setExamens(examens);
    }
    else
    {
        ui->statusBar->showMessage("Aucun Examen ajouté",6000);
    }
}

void MainWindow::recept(QString id, QString id_mere, QString descri, QString value)
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

void MainWindow::remove(QString id, QString id_mere, QString desc)
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

void MainWindow::on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    if (!isExamEdited)
    {
        isExamEdited=true;
        ui->addExamenBtn->setText("Finir");
    }
    getNewExamen()->setDate(dateTime);
}

void MainWindow::on_responsablePrincipalCheck_toggled(bool checked)
{
    if (!isExamEdited)
    {
        isExamEdited=true;
        ui->addExamenBtn->setText("Finir");
    }


    if (checked && ui->medecinResponsable->currentIndex()==-1)
    {
        ui->examinateur->setCurrentIndex(ui->medecinResponsable->currentIndex());
    }

    newExamen->setExaminateur(getMedecins()->at(ui->medecinResponsable->currentIndex()));
}

void MainWindow::on_examinateur_currentIndexChanged(int index)
{
    if (!isExamEdited)
    {
        isExamEdited=true;
        ui->addExamenBtn->setText("Finir");
    }

    if (index!=-1)
    {
        newExamen->setExaminateur(getMedecins()->at(index));
    }

}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    qDebug() << arg1;
    QRegExp regExp(arg1,Qt::CaseInsensitive);
    sortFilter->setFilterRegExp(regExp);
    ui->listePatientView->setModel(sortFilter);

}

void MainWindow::on_filtre_currentIndexChanged(int index)
{
    sortFilter->setFilterKeyColumn(index);
}


void MainWindow::on_pushButton_2_clicked(bool checked)
{
    ui->pages->setCurrentIndex(0);
}



void MainWindow::on_endStat_clicked()
{
    finishStat();
}

void MainWindow::startStat()
{
    cv= new HCHARTVIEW(ui->pages->widget(2));
    ui->containerStat->addWidget(cv);
    ui->pages->setCurrentIndex(2);
    ui->pages->widget(2)->setBackgroundRole(QPalette::HighlightedText);
}

void MainWindow::finishStat()
{
    delete cv;
    ui->pages->setCurrentIndex(1);
//    delete cv;
}
