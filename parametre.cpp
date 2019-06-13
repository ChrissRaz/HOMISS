#include "parametre.h"
#include "ui_parametre.h"


PARAMETRE::PARAMETRE(QWidget *parent, int index) : QDialog(parent),ui(new Ui::PARAMETRE)
{
    ui->setupUi(this);
    this->setWindowTitle("Paramètres");

    this->setAttribute(Qt::WA_DeleteOnClose);


    ui->message->setVisible(false);
    applique = false;
    egal=false;
    setValidity(true);


    xmlManager= new HXmlManager(this);
    host=xmlManager->getDatabaseParameter(DBInfoType::HOSTNAME);
    username= xmlManager->getDatabaseParameter(DBInfoType::USERNAME);
    pass=xmlManager->getDatabaseParameter(DBInfoType::PASSWORD);
    dbname=xmlManager->getDatabaseParameter(DBInfoType::DBNAME);
    port=xmlManager->getDatabaseParameter(DBInfoType::PORT);

    ui->stackedWidget->setCurrentIndex(index);
    ui->host->setText(host);
    ui->un->setText(username);
    ui->pass->setText(pass);
    ui->dbname->setText(dbname);
    ui->port->setText(port);


    QObject::connect(ui->host,SIGNAL(textChanged(QString)),this,SLOT(verifChamp(QString)));
    QObject::connect(ui->un,SIGNAL(textChanged(QString)),this,SLOT(verifChamp(QString)));
    QObject::connect(ui->pass,SIGNAL(textChanged(QString)),this,SLOT(verifChamp(QString)));
    QObject::connect(ui->dbname,SIGNAL(textChanged(QString)),this,SLOT(verifChamp(QString)));
    QObject::connect(ui->port,SIGNAL(textChanged(QString)),this,SLOT(verifChamp(QString)));

    QObject::connect(ui->confirmationBtn,&QDialogButtonBox::clicked,this,&PARAMETRE::actionClick);

    instantiate();

    if (index==1)
    {
        this->setViewfiche();
    }

}

PARAMETRE::~PARAMETRE()
{
    delete ui;
    delete xmlManager;
}
QString PARAMETRE::getPort() const
{
    return port;
}

void PARAMETRE::setPort(const QString &value)
{
    port = value;
}

QString PARAMETRE::getPass() const
{
    return pass;
}

void PARAMETRE::setPass(const QString &value)
{
    pass = value;
}

QString PARAMETRE::getDbname() const
{
    return dbname;
}

void PARAMETRE::setDbname(const QString &value)
{
    dbname = value;
}

QString PARAMETRE::getHost() const
{
    return host;
}

void PARAMETRE::setHost(const QString &value)
{
    host = value;
}

bool PARAMETRE::getEgal() const
{
    return egal;
}

void PARAMETRE::setEgal(bool value)
{
    egal = value;
}

void PARAMETRE::setDatabaseParams()
{
    this->setUsername(ui->un->text());
    this->setHost(ui->host->text());
    this->setPass(ui->pass->text());
    this->setPort(ui->port->text());
    this->setDbname(ui->dbname->text());
}

void PARAMETRE::saveDBParams()
{
    //mettre ici tout les paramètres à enregistrer

    xmlManager->setDatabaseParameters(this->getHost(),this->getUsername(),this->getPass(),this->getDbname(),this->getPort());
}

void PARAMETRE::save()
{
    setDatabaseParams();
    saveDBParams();
//    if (xmlManager->getStartParameter(startParamaterType::GLOBAL))
//    {
//        if ( xmlManager->getStartParameter(startParamaterType::FICHE))
//        {
//            xmlManager->setStartParameters(startParamaterType::GLOBAL,false);
//        }
//    }

}

void PARAMETRE::actionClick(QAbstractButton *clickedButton)
{
    QPushButton  *btn= qobject_cast<QPushButton *>(clickedButton);
    if (egal)
    {
        showMessage("Aucune changement n'a été faite");
    }

    if (btn!=nullptr && ui->confirmationBtn->buttonRole(btn)==QDialogButtonBox::ApplyRole)
    {
        if (isValid())
        {
            if (xmlManager->getStartParameter(startParamaterType::FICHE))//pour ne plus afficher le paramètre à chaque connexion
            {
                xmlManager->setStartParameters(startParamaterType::FICHE,false);
            }
            applique=true;
            save();
            emit applied();
        }
        else
        {
            showMessage("Vous devez remplir tout les champs");
        }

    }
    else if (btn!=nullptr && ui->confirmationBtn->buttonRole(btn)==QDialogButtonBox::AcceptRole)
    {
        if (isValid())
        {

            if (xmlManager->getStartParameter(startParamaterType::FICHE))//pour ne plus afficher le paramètre à chaque connexion
            {
                xmlManager->setStartParameters(startParamaterType::FICHE,false);
            }

            this->save();

            emit accepted();
        }
        else
        {
            showMessage("Vous devez remplir tout les champs");
        }

    }
    else if(btn!=nullptr && ui->confirmationBtn->buttonRole(btn)==QDialogButtonBox::RejectRole)
    {

        if (!applique && isValid())
        {
            QString ms="";
            if (m_first)
            {
                ms="Il se pourait que la connexion ne sera pas établie si vous quittez sans enregistrer maintenant.\n";
            }

            ms+="Voulez vous ignorer les changements?";
            int res= QMessageBox::question(this,"Confirmation",ms);

            if(res==QMessageBox::No)
            {
                this->save();
                emit accepted();
                if (!m_first)
                {
                    QMessageBox::information(this,"Information","Les paramètres sont pris en charges après redémarage de l'application!");
                }
            }
        }

    }
}

void PARAMETRE::exec(bool first)
{
    m_first=first;

    if (first)
    {
        ui->ModeleBtn->setEnabled(false);

    }
    else
    {
        ui->ModeleBtn->setEnabled(true);

    }

    QDialog::exec();


}

void PARAMETRE::verifChamp(QString text)
{
    applique=false;
    if (!ui->dbname->text().isEmpty() && !ui->host->text().isEmpty() && !ui->un->text().isEmpty() && !ui->pass->text().isEmpty() && !ui->port->text().isEmpty())
    {
        if (ui->host->text()==host && ui->un->text()==username && ui->pass->text()==pass && ui->dbname->text()==dbname && ui->port->text()==port)
        {
            setEgal(true);
        }
        else
        {
            setEgal(false);
        }

        setValidity(true);
    }
    else
    {
        setValidity(false);
    }
}

void PARAMETRE::setValidity(bool value)
{
    validity = value;
}

bool PARAMETRE::getViewFicheSet() const
{
    return viewFicheSet;
}

void PARAMETRE::setViewFicheSet(bool value)
{
    viewFicheSet = value;
}

bool PARAMETRE::isValid() const
{
    return validity;
}

void PARAMETRE::instantiate()
{
    if (ui->stackedWidget->currentIndex()==2)
    {
        setViewfiche();
    }

}

QString PARAMETRE::getUsername() const
{
    return username;
}

void PARAMETRE::setUsername(const QString &value)
{
    username = value;
}

void PARAMETRE::on_DBButton_clicked()
{
    if (ui->stackedWidget->currentIndex()!=0)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void PARAMETRE::on_ModeleBtn_clicked()
{

    if (ui->stackedWidget->currentIndex()!=1)
    {
        setViewfiche();
        ui->stackedWidget->setCurrentIndex(1);
    }
}


void PARAMETRE::hideMessage()
{
    ui->message->setText("");
}

void PARAMETRE::showMessage(QString msg)
{
    ui->message->setText(msg);
    QTimer::singleShot(5000,this,SLOT(hideMessage()));
}

void PARAMETRE::setViewfiche()
{
    if (!getViewFicheSet())
    {
        QList<QDomElement> super=xmlManager->getSuperMenus();

        int i=0;

        i=0;
        HLine *l;


        while(i < super.length())//récuération des Super menus
        {
    //        qDebug() << super.at(i).attribute("descriptif");

            QStringList values;
            QStringList ids;

            QList<QDomElement > valE= xmlManager->getValuesOfMenu(super.at(i).attribute("id"));
            QList<QDomElement > idsE= xmlManager->getMenuChilds(super.at(i).attribute("id"));

            int k=0;

            while (k < valE.length())//récupération des id des values
            {
                values.append(valE.at(k).attribute("content"));

                k++;
            }

            k=0;

            while (k < idsE.length())// récupération des id des sous menu
            {
                ids.append(idsE.at(k).attribute("id"));
    //            qDebug() << idsE.at(k).attribute("id");
                k++;
            }

            //Détermination du remplissage

            if (super.at(i).attribute("remplissage")!="0" && super.at(i).attribute("remplissage")!="1" && super.at(i).attribute("remplissage")!="2")
            {
                qWarning() << "remplissage invalide dans l'instantiation des lignes super dan PARAMETRE::instantiate pour "+super.at(i).attribute("descriptif");
            }

            bool usage = false;

            if (super.at(i).attribute("usage")=="true")
            {
                usage=true;
            }
            else if(super.at(i).attribute("usage")=="false")
            {
                usage=false;
            }
            else
            {
                qWarning("usage invalide dans l'instantiation des lignes super dan PARAMETRE::instantiate");
            }

            l= new HLine(super.at(i).attribute("id"),super.at(i).attribute("descriptif"),values,super.at(i).attribute("alias"),super.at(i).attribute("remplissage"),usage,ids,super.at(i).attribute("parametrable")=="true"?true:false,super.at(i).attribute("valueInverted")=="true"?true:false,this);

           ui->modeleMenuLayout->addWidget(l);//ajout des Super menus dans parametre

            i++;
        }

        setViewFicheSet(true);
    }

}
