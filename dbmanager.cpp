#include "DBManager.h"

DBManager::DBManager(QString hote, QString userName, QString password, QString DBName, int port, QObject *parent): QObject(parent)
{

    this->setHote(new QString(hote));
    this->setUserName(new QString(userName));
    this->setPassword(new QString(password));
    this->setDBName(new QString(DBName));
    this->setPort(&port);
    this->setWithMainDb(true);

    this->instantiate();
     qDebug ()  <<  autoFormat("L'homme");
}

DBManager::DBManager(QSqlDatabase &database, QObject *parent): QObject(parent)
{
    this->setWithMainDb(false);
    this->setHote(new QString(database.hostName()));
    this->setUserName(new QString (database.userName()));
    this->setPassword(new QString(database.password()));
    this->setDBName(new QString(database.databaseName()));
    this->setPort(new int(database.port()));
    DB=new QSqlDatabase(database);
    req=new QSqlQuery(*DB);
    this->instantiate();
}

DBManager::~DBManager()
{
    stopThread();
    th->quit();
    th->wait();
//    qDebug() <<"Thread state before: "<< th->isFinished();
//    stopThread();
//    qDebug() <<"Thread state after: "<< th->isFinished();

    delete  DB;
    delete  req;
    delete  m_hote;
    delete  m_userName;
    delete m_password;
    delete  m_DBName;
    delete  m_port;
}

void DBManager::instantiate()
{

    xmlManager= new HXmlManager(this);

    if (getWithMainDb())
    {
        DB= new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));//default database
        req= new QSqlQuery();//query for default database
    }


    connect(this,SIGNAL(errBrute(QSqlError::ErrorType)),this,SLOT(showError(QSqlError::ErrorType)));
    connect(xmlManager,SIGNAL(fileNotOpened(QString)),this,SLOT(xmlError(QString)));

    th= new QThread(this);

//    instatiateConnectionThread();
}

void DBManager::setHote(QString *hote)
{
    m_hote = hote;
}

void DBManager::setUserName(QString *userName)
{
    m_userName = userName;
}

void DBManager::setPassword(QString *password)
{
    m_password = password;
}
void DBManager::setDBName(QString *DBName)
{
    m_DBName = DBName;
}

QSqlDatabase *DBManager::getDB() const
{
    return DB;
}

void DBManager::setDB(QSqlDatabase *value)
{
    DB = value;
}

void DBManager::setConnection(bool value)
{
    connection = value;
}

void DBManager::setPort(int *value)
{
    m_port = value;
}

void DBManager::configureDB(QString hote, QString userName, QString password, QString DBName,int port)
{
    bool o=DB->isOpen();
    if (o) DB->close();
    DB->setHostName(hote);
    DB->setUserName(userName);
    DB->setPassword(password);
    DB->setDatabaseName(DBName);
    DB->setPort(port);
    if (o) DB->open();
}

void DBManager::reload()
{
    bool ope=DB->isOpen();
    if (ope) DB->close();
    this->setHote(new QString(xmlManager->getDatabaseParameter(DBInfoType::HOSTNAME)));
    this->setUserName(new QString(xmlManager->getDatabaseParameter(DBInfoType::USERNAME)));
    this->setPassword(new QString(xmlManager->getDatabaseParameter(DBInfoType::PASSWORD)));
    this->setDBName(new QString(xmlManager->getDatabaseParameter(DBInfoType::DBNAME)));
    this->setPort(new int(xmlManager->getDatabaseParameter(DBInfoType::PORT).toInt()));
    configureDB(*this->m_hote,*this->m_userName,*this->m_password,*this->m_DBName,*this->m_port);
    if (ope) DB->open();
    qDebug() << "Database manage Database reloaded: "<< xmlManager->getDatabaseParameter(DBInfoType::HOSTNAME);
}

QString DBManager::autoFormat(QString entre)
{
    QString e=entre;
    QString res;
    int i=0;
    while (i<e.length())
    {
        if (e.at(i)=="'")
        {
           res.append("\\'");
        }
        else
        {
            res.append(e.at(i));
        }

        i++;
    }
    return res;
}

QSqlQuery *DBManager::getReq() const
{
    return req;
}

QPieSeries *DBManager::getStat(QString reqString,TYPESTAT type,int interv)
{
    QPieSeries *res= new QPieSeries();

    if (type==NORMALE)
    {
//        if (!req->exec("CREATE VIEW V_tmp AS "+reqString))
//        {
//            req->finish();
//            abort("Erreur lors de getstat création de vue");
//            return res;
//        }

//        reqString= "SELECT effectif, MAX(effectif) as effmax FROM V_tmp";
        int i=0;
        int max=0;
        vector<int> pos;
        if (req->exec(reqString))
        {
            while(req->next())
            {

//                if (req->value("effmax").toInt()==req->value("effectif").toInt())
//                {
//                    pos.push_back(i);
//                }
                if (req->value("effectif").toInt()>=max)
                {
                    if (req->value("effectif").toInt()>max)
                    {
                        pos.clear();
                        max=req->value("effectif").toInt();
                        pos.push_back(i);
                    }
                    else
                    {
                        max=req->value("effectif").toInt();
                        pos.push_back(i);
                    }

                }
                res->append(new QPieSlice(req->value("indicatif").toString()+" (effectif: "+req->value("effectif").toString()+", ",req->value("effectif").toInt()));
    //            res->slices().at(i)->setLabel(QString::number(res->slices().at(i)->percentage())+"%");
                res->slices().at(i)->setLabelVisible();
    //            res->slices().at(i)->setLabelPosition(QPieSlice::LabelPosition::LabelInsideNormal);

                i++;
            }



            if (!res->slices().isEmpty())
            {
                int j=0;

                while (j<(int)pos.size())
                {
                    QPieSlice *slice = res->slices().at(j);
                    slice->setExploded();
                    slice->setLabelVisible();
                    slice->setPen(QPen(Qt::yellow, 2));
                    slice->setBrush(Qt::green);
                    req->finish();
                    j++;
                }

            }

            i=0;

            while(i<res->slices().length())
            {
                res->slices().at(i)->setLabel(res->slices().at(i)->label()+"Taux: "+QString::number(qRound(res->slices().at(i)->percentage()*100))+"%)");
                i++;
            }

//            req->exec("DROP VIEW V_tmp");
            return  res;
        }
        else
        {
           req->finish();
           abort("Erreur lors de getstat");
           return res;
        }
    }

}

QStringList DBManager::getAllValues()
{
    QStringList res;

    if (req->exec("SELECT * FROM valeur"))
    {
        while (req->next())
        {
            res.append(req->value("contenu").toString());
        }
        req->finish();
        return res;
    }
    else
    {
        abort("erreur lors de la récupération des valeurs");
        return res;
    }
}

bool DBManager::getWithMainDb() const
{
    return m_withMainDb;
}

void DBManager::setWithMainDb(bool withMainDb)
{
    m_withMainDb = withMainDb;
}

void DBManager::seConnecter()
{
    reload();

    //    if (th->isRunning())
    //    {
    //        qDebug("stooooooop");
    //        this->stopThread();
//    }

    if (getWithMainDb())
    {
        instatiateConnectionThread();
    }


    if (DB->isOpen()) DB->close();


    bool err=DB->open();
    if (!err)
    {
        setConnection(false);
        emit errBrute(DB->lastError().type());
    }
    else
    {

        emit connected();//connecté à la base de donné
        setConnection(true);
        qDebug() <<"connected to db" << *m_hote+ " " + *m_userName+" "+ *m_password+" "+ *m_DBName+" "+QString::number(*m_port);

    }
}

void DBManager::stopThread()
{
    if (getWithMainDb())
    {
        if (this->th!=nullptr)
        {
            if (th->isRunning())
            {
                if (observator!=nullptr)
                {
                    observator->setStop(true);

                    QThread::sleep(3);
                }
            }
        }
    }

}

void DBManager::abort(QString message)
{
    qDebug() <<  message;
    emit errBrute(req->lastError().type());
    req->finish();
}

void DBManager::instatiateConnectionThread()
{
    if (getWithMainDb())
    {
        stopThread();
        th= new QThread(this);

        observator= new connexionObservator();
        observator->moveToThread(th);
        connect(th, &QThread::finished,th,&QThread::deleteLater);
        connect(th, &QThread::started, observator, &connexionObservator::verifierConnection);
        connect(observator, &connexionObservator::fini,th,&QThread::quit);
        connect(observator, &connexionObservator::fini, observator, &QObject::deleteLater);


        connect(observator, &connexionObservator::connexionStateChanged, this, &DBManager::connectionStateChangedAction);

        th->start();

    //    emit goVerif();
    }

}

bool DBManager::userConnectionApp(const QString &un, const QString &pass)
{
    if (isConnected())
    {

        QString r="SELECT id_personne FROM login WHERE user_name=:un AND pass=:pass";
        req->prepare(r);
        req->bindValue(":un",un);
        req->bindValue(":pass",pass);

        if (req->exec())//à améliorer pour plus de
        {
            if (req->first())
            {
                int idu=req->value("id_personne").toInt();

                req->finish();
                emit userConnection(true,idu);
                xmlManager->addUserConnected(QString::number(idu));

                return true;

            }
            else
            {
                emit userConnection(false,-1);
                req->finish();
                return false;
            }
        }
        else
        {
            emit errBrute(req->lastError().type());
            req->finish();
            return  false;
        }

    }
    else
    {
       emit errBrute(DB->lastError().type());
        return false;
    }
}

bool DBManager::isConnected()
{
    return connection;
}

bool DBManager::setAppId(QString id)
{
    if (isConnected())
    {
        QString rt="INSERT INTO application values(:i)";
        req->prepare(rt);
        req->bindValue(":i",id);

        if (req->exec())
        {
            return true;
        }

        return false;
    }
    else
    {
        return false;
    }
}

bool DBManager::insertLog(QString action, QString idApp, QString idPers, QString idAdmission)
{
    if (isConnected())
    {
        QString rt="INSERT INTO log (action,date_log,id_application,id_personnel,id_admission) VALUES(:act,:idA,:idP,:idAdm)";
        req->prepare(rt);
        req->bindValue(":act",action);
        req->bindValue(":idA",idApp);
        req->bindValue("idP",idPers);
        req->bindValue(":idAdm",idAdmission);

        if (req->exec())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

PERSONNE* DBManager::getPersonne(QString id)
{
    req->prepare("SELECT * FROM personne WHERE id_personne=:i");
    req->bindValue(":i",id);
    if (req->exec())
    {
        if (req->first())
        {
            int idp= req->value("id_personne").toInt();
            QString nom= req->value("nom").toString();
            QString pn= req->value("prenom").toString();
            QString age= req->value("age").toString();
            sexe sex;

            if (req->value("genre").toString()=="1")
            {
                sex= HOMME;
            }
            else
            {
                sex= FEMME;
            }
            QString id_nationalite= req->value("id_nationalite").toString();
            QString id_adresse= req->value("id_adresse").toString();

            req->finish();

            QList<CONTACT*> contacts= getContacts(QString::number(idp));
            NATIONALITE *nat= getNationalite(id_nationalite);
            ADRESSE *adresse= getAdresse(id_adresse);

            return new PERSONNE(idp,nom,pn,sex,nat,adresse,contacts,getNaissance(QString::number(idp)),age);
        }
        else
        {
            req->finish();
            return new PERSONNE();
        }

    }
    else
    {
        qDebug("erreur de requete getPersonne erreur");
        emit errBrute(req->lastError().type());
        req->finish();
        return new PERSONNE();
    }
}

VILLE *DBManager::getVille(QString id)
{
    req->prepare("SELECT * FROM ville WHERE num_ville=:i");
    req->bindValue(":i",id);
    if (req->exec())
    {
        if (req->first())
        {
            VILLE *v= new VILLE(req->value("num_ville").toInt(),req->value("nom_ville").toString());
            req->finish();
            return v;
        }
        else
        {
            req->finish();
            return new VILLE();
        }

    }
    else
    {
        qDebug("erreur de requete getVille erreur");
        emit errBrute(req->lastError().type());
        req->finish();
        return new  VILLE();
    }
}

QUARTIER* DBManager::getQuatier(QString id)
{
    req->prepare("SELECT * FROM quartier WHERE id_quartier=:i");
    req->bindValue(":i",id);
    if (req->exec())
    {
        if (req->first())
        {
            QString idv=req->value("num_ville").toString();
            int idq=req->value("id_quartier").toInt();
            QString nom=req->value("nom_quartier").toString();
            req->finish();

            QUARTIER *q= new QUARTIER(idq,nom,new VILLE(*getVille(idv)));

            return q;
        }
        else
        {
            req->finish();
            return new QUARTIER();
        }
    }
    else
    {
        qDebug("erreur de requete getQuatier erreur");
        emit errBrute(req->lastError().type());
        req->finish();
        return new QUARTIER();
    }
}

ADRESSE* DBManager::getAdresse(QString id)
{
    req->prepare("SELECT * FROM adresse WHERE id_adresse=:i");
    req->bindValue(":i",id);
    if (req->exec())
    {
        if (req->first())
        {

            QString idq=req->value("id_quartier").toString();
            ADRESSE *a= new ADRESSE(req->value("id_adresse").toInt(),req->value("lot").toString(),new QUARTIER());
            req->finish();

            a->setQuartier(new QUARTIER(*getQuatier(idq)));

            return a;
        }
        else
        {
            req->finish();
            return new ADRESSE();
        }
    }
    else
    {
        qDebug("erreur de raquete dans getAdresse");
        emit errBrute(req->lastError().type());
        req->finish();
        return new ADRESSE();
    }
}

QList<CONTACT*> DBManager::getContacts(QString id_personne)
{
    QList<CONTACT*> res;

    req->prepare("SELECT * FROM contact WHERE id_personne=:i");
    req->bindValue(":i",id_personne);
    if (req->exec())
    {
        while(req->next())
        {
            res.append(new CONTACT(req->value("id_contact").toInt(),req->value("tel").toString()));
        }
        req->finish();

        return res;
    }
    else
    {
        qDebug("erreur requete getContact");
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

PATIENT *DBManager::getPatient(QString id)
{
    req->prepare("SELECT * FROM patient WHERE id_personne=:i");
    req->bindValue(":i",id);

    if(req->exec())
    {
        if(req->first())
        {
            int idp=req->value("id_personne").toInt();
            QString profession= req->value("proffession").toString();
            STATUT *statut= getStatut(req->value("id_statut").toString());
            req->finish();
            PERSONNE *per= getPersonne(QString::number(idp));


            return new  PATIENT(idp,per->nom(),per->prenom(),profession,per->genre(),per->nationalite(),statut,per->adresse(),per->contacts(),per->naissance(),per->age());
        }
        else
        {
            req->finish();
            return new PATIENT();
        }
    }
    else
    {
        qDebug("erreur de requete getPatient erreur");
        emit errBrute(req->lastError().type());
        req->finish();
        return new PATIENT();
    }

}

PERSONEL* DBManager::getPersonel(QString id)
{
    req->prepare("SELECT * FROM personnel  WHERE id_personne=:i");
    req->bindValue(":i",id);

    if(req->exec())
    {
        if(req->first())
        {
            QString idsta=req->value("id_statut_mili").toString();
            req->finish();
            SM *statut= getStatutMilitaire(idsta);
            PERSONNE* per= getPersonne(id);
            return new PERSONEL(id.toInt(),per->nom(),per->prenom(),per->genre(),per->nationalite(),statut,per->adresse(),per->contacts(),per->naissance(),per->age());
        }
        else
        {
            req->finish();
            return new PERSONEL();
        }
    }
    else
    {
        emit errBrute(req->lastError().type());
        req->finish();
        return new PERSONEL();
    }
}

SANTE* DBManager::getSante(QString id)
{
    req->prepare("SELECT * FROM santee WHERE id_personne=:i");
    req->bindValue(":i",id);

    if(req->exec())
    {
        if(req->first())
        {
            int ids=req->value("id_personne").toInt();

            req->finish();
            PERSONEL *per= new PERSONEL(*getPersonel(QString::number(ids)));

            return new SANTE(ids,per->nom(),per->prenom(),per->genre(),per->nationalite(),per->getStatut(),per->adresse(),per->contacts(),per->naissance(),per->age());
        }
        else
        {
            req->finish();
            return new SANTE();
        }
    }
    else
    {
        qDebug("erreur de requete getSante erreur");
        emit errBrute(req->lastError().type());
        req->finish();
        return new SANTE();
    }
}

MEDECIN * DBManager::getMedecin(QString id)
{
    req->prepare("SELECT * FROM medecin WHERE id_personne=:i");
    req->bindValue(":i",id);

    if(req->exec())
    {
        if(req->first())
        {
            int ids=req->value("id_personne").toInt();

            req->finish();
            SANTE *per= getSante(QString::number(ids));

            return new MEDECIN(ids,per->nom(),per->prenom(),per->genre(),per->nationalite(),per->getStatut(),per->adresse(),per->contacts(),per->naissance(),per->age());
        }
        else
        {
            req->finish();
            return new MEDECIN();
        }
    }
    else
    {
        qDebug("erreur de requete getMedecin erreur");
        emit errBrute(req->lastError().type());
        req->finish();
        return new MEDECIN();
    }
}

INFIRMIER *DBManager::getInfirmier(QString id)
{
    req->prepare("SELECT * FROM infirmier WHERE id_personne=:i");
    req->bindValue(":i",id);

    if(req->exec())
    {
        if(req->first())
        {
            int ids=req->value("id_personne").toInt();

            req->finish();
            SANTE *per= getSante(QString::number(ids));

            return new INFIRMIER(ids,per->nom(),per->prenom(),per->genre(),per->nationalite(),per->getStatut(),per->adresse(),per->contacts(),per->naissance(),per->age());
        }
        else
        {
            req->finish();
            return new INFIRMIER();
        }
    }
    else
    {
        qDebug("erreur de requete getInfirmier erreur");
        emit errBrute(req->lastError().type());
        req->finish();
        return new INFIRMIER();
    }
}

NATIONALITE * DBManager::getNationalite(QString id)
{
    req->prepare("SELECT * FROM nationalite WHERE id_nationalite=:i");
    req->bindValue(":i",id);

    if(req->exec())
    {
        if(req->first())
        {
            NATIONALITE *res = new NATIONALITE(req->value("id_nationalite").toInt(),req->value("label_nationalite").toString());
            req->finish();
            return res;
        }
        else
        {
            req->finish();
            return new NATIONALITE();
        }
    }
    else
    {
        qDebug("erreur de requete getNationalite erreur");
        emit errBrute(req->lastError().type());
        req->finish();
        return new NATIONALITE();
    }
}

QDate DBManager::getNaissance(QString id_personne)
{
    req->prepare("SELECT * FROM nee WHERE id_personne=:i");
    req->bindValue(":i",id_personne);

    if (req->exec())
    {
        if (req->first())
        {
            QString id_date=req->value("id_date_naissance").toString();
            req->finish();
            if (req->exec("SELECT * FROM date_naissance WHERE id_date_naissance='"+id_date+"'"))
            {
               if (req->first())
               {
                   QString res=req->value("naissance").toString();
                   req->finish();
                   return QDate::fromString(res);

               }
               else
               {
                   qDebug("Erreur SQL avec la table date_naissance et personne");
                   req->finish();
                   return QDate();
               }
            }
            else
            {    
                emit errBrute(req->lastError().type());
                req->finish();
                return QDate();
            }


        }
        else
        {

            req->finish();
            return QDate();
        }
    }
    else
    {
        qDebug("erreur de requete getNaissance erreur");
        req->finish();
        emit errBrute(req->lastError().type());
        return QDate();
    }
}

SM *DBManager::getStatutMilitaire(QString id)
{

    req->prepare("SELECT * FROM statut_mili WHERE id_statut_mili=:i");
    req->bindValue(":i",id);
    if(req->exec())
    {
        if(req->first())
        {
            int ids= req->value("id_statut_mili").toInt();
            QString l= req->value("label_statut_mili").toString();

            req->finish();
            return new SM(ids,l);
        }
        else
        {
            req->finish();
            return new SM();
        }
    }
    else
    {
        qDebug("erreur de requete getSM erreur");
        emit errBrute(req->lastError().type());
        req->finish();
        return new SM();
    }
}

STATUT *DBManager::getStatut(QString id)
{
    req->prepare("SELECT * FROM statut WHERE id_statut=:i");
    req->bindValue(":i",id);
    if(req->exec())
    {
        if(req->first())
        {
            int ids= req->value("id_statut").toInt();
            QString l= req->value("label_statut").toString();

            req->finish();
            return new STATUT(ids,l);
        }
        else
        {
            req->finish();
            return new STATUT();
        }
    }
    else
    {
        qDebug("erreur de requete getStatut erreur");
        emit errBrute(req->lastError().type());
        req->finish();
        return new STATUT();
    }
}

SORTIE* DBManager::getSortie(PATIENT *pat)
{
    QString last= getIdLastAdmission(pat);

    if (last.isEmpty())
    {
        qWarning("Invalid patient on getSortie");
        return new SORTIE();
    }

    req->prepare("SELECT * FROM sortie INNER JOIN mode_sortie ON sortie.id_mode_sortie=mode_sortie.id_mode_sortie WHERE sortie.id_admission=:i");
    req->bindValue(":i",last);

    if (req->exec())
    {
        if(req->first())
        {
            QString idr=req->value("id_sortie").toString();
            QDateTime ds=QDateTime::fromString(req->value("date_sortie").toString());
            QString ids=req->value("id_service").toString();
            QString idms= req->value("id_mode_sortie").toString();
            QString ms= req->value("label").toString();
            req->finish();

            SERVICE *ser= getService(ids);

            return new SORTIE(idr,ds,new MODESORTIE(idms,ms),ser);
        }
        else
        {
            qDebug() << "pas de sortie pour" +pat->nom()+" "+pat->prenom();
            req->finish();
            return new SORTIE();
        }
    }
    else
    {
        qDebug("erreur de requete getSortie erreur");
        emit errBrute(req->lastError().type());
        req->finish();
        return new SORTIE();
    }
}

SORTIE *DBManager::getSortie(QString id_admission)
{
//    if (req->exec("SELECT * FROM sortie INNER JOIN admission"))
}

QString DBManager::getSituation(PATIENT *pat)
{
    if (pat->isNull())
    {
        qWarning ("patient invalide");
    }

    SORTIE *s= getSortie(pat);

    if (!s->isNull())
    {
        return s->mode()->getLabel();
    }
    else
    {
        return getModeSoin(getIdLastAdmission(pat))->getLabel();
    }

}

MODESOIN *DBManager::getModeSoin(QString id_admission)
{
    if (req->exec("SELECT mode_soin.id_mode_soin AS id, mode_soin.label as l FROM mode_soin INNER JOIN admission ON mode_soin.id_mode_soin=admission.id_mode_soin WHERE admission.id_admission="+id_admission+""))
    {
        if (req->first())
        {

            MODESOIN *res= new MODESOIN(req->value("id").toString(),req->value("l").toString());
            req->finish();

            return  res;
        }
        else
        {
            qWarning() << "id admission "+id_admission+" non valide dans getModeSoin";
        }
    }
    else
    {
        qDebug("erreur de requete getModesoin");
        emit errBrute(req->lastError().type());
        req->finish();
        return new MODESOIN();
    }
}

QStringList DBManager::getIdAdmission(PATIENT *pat)
{
    QStringList res;
    if (pat->isNull())
    {
        qWarning ("patient invalide");
    }

    if (req->exec("SELECT id_admission as id FROM admission WHERE id_patient='"+QString::number(pat->getId())+"'"))
    {
        while(req->next())
        {
            qDebug() << req->value("id").toString();

            res.append(req->value("id").toString());
        }
        req->finish();
        return res;
    }
    else
    {
        qDebug("Error frorm getIdamission");
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

QString DBManager::isClassExist(QString id_super,QString description)
{
    QString res;
    if (id_super=="")
    {
        id_super="0";
    }

    if (req->exec("SELECT * FROM classe WHERE descriptif='"+description+"'"))
    {
        while (req->next())
        {
            if (req->value("id_classe_supra_ordonne").toString()==id_super)
            {
                res= req->value("id_classe").toString();
                req->finish();
                return res;
            }
        }
        req->finish();

        return res;
    }
    else
    {
        qDebug("error from isExistClass");
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

QString DBManager::isValueExist(QString content)
{
    QString res;
    if (req->exec("SELECT id_valeur FROM valeur WHERE contenu='"+content+"'"))
    {
        if (req->first())
        {
            res= req->value("id_valeur").toString();
            req->finish();
            return res;
        }

        req->finish();

        return res;
    }
    else
    {
        qDebug("error from isValueExist");
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

QString DBManager::getIdLastAdmission(PATIENT *pat)
{
    if (pat->isNull())
    {
        qWarning("Patient null dans getIdAdmission");
    }

    QStringList res=getIdAdmission(pat);
    if (res.isEmpty())
    {
        qDebug("getIdLastAdmission erreur");
        return QString();
    }
    else
    {
        return res.last();
    }
}

QList<PATIENT *> DBManager::getAllPatients()
{
    QList<PATIENT *> res;
    if (req->exec("SELECT id_personne FROM patient"))
    {
        QStringList ids;

        while(req->next())
        {
            ids.append(req->value("id_personne").toString());
        }

        req->finish();

        int i=0;

        while (i< ids.length())
        {
            res.append(getPatient(ids.at(i)));
            i++;
        }

        return res;
    }
    else
    {
        qDebug("erreur de requete getAllPatients erreur");
        req->finish();
        emit errBrute(req->lastError().type());
        return res;
    }
}

SERVICE *DBManager::getService(QString id, bool withPers)
{
    req->prepare("SELECT * FROM service INNER JOIN service_de ON service.id_service=service_de.id_service INNER JOIN hopital ON hopital.id_hopital=service_de.id_hopital WHERE service.id_service=:i");
    req->bindValue(":i",id);

    if (req->exec())
    {
        if (req->first())
        {
            SERVICE *res= new SERVICE(req->value("id_service").toInt(),req->value("nom_service").toString(),req->value("nom_hopital").toString());
            QString id= req->value("id_service").toString();
            QString idm= req->value("id_major").toString();
            QString idc= req->value("id_chef").toString();

            req->finish();

            res->setMajor(getMedecin(idm));
            res->setChef(getMedecin(idc));

            if (withPers)
            {
                res->setMedecins(getAllMedecinsFromService(id));
                res->setInfirmiers(getAllInfirmiersFromService(id));
            }

            return res;

        }
        else
        {

            req->finish();
            return new SERVICE();
        }
    }
    else
    {
        qDebug("erreur de requete getService erreur");
        req->finish();
        emit errBrute(req->lastError().type());
        return new SERVICE();
    }
}

QList<SERVICE *> DBManager::getServices(QString id_hopital, bool withPers)
{
    QList<SERVICE *> res;

    req->prepare("SELECT sd.id_service as ids FROM service_de as sd INNER JOIN hopital as h ON sd.id_hopital=h.id_hopital WHERE h.id_hopital=:i");
    req->bindValue(":i",id_hopital);

    if (req->exec())
    {
        QStringList ids;
        while (req->next())
        {

            ids.append(req->value("ids").toString());

        }
        req->finish();
        int i=0;
        while (i < ids.length())
        {
            res.append(getService(ids.at(i),withPers));
            i++;
        }
        return res;
    }
    else
    {
        qDebug("erreur de requete getServices erreur");
        req->finish();
        emit errBrute(req->lastError().type());
        return res;
    }
}

QList<MEDECIN *> DBManager::getAllMedecinsFromService(QString id_service)
{
    QList<MEDECIN *> res;
    //m pour médecin p pour personne
    req->prepare("SELECT med.id_personne as id FROM personnel as p INNER JOIN service as s ON p.id_service=s.id_service INNER JOIN medecin AS med ON p.id_personne=med.id_personne WHERE s.id_service=:i");
    req->bindValue(":i",id_service);

    if (req->exec())
    {
        QStringList ids;
        while (req->next())
        {
            ids.append(req->value("id").toString());
        }

        req->finish();

        int i=0;

        while (i < ids.length())
        {
            res.append(getMedecin(ids.at(i)));
            i++;
        }

        return res;
    }
    else
    {
        qDebug("error from getAllMed");
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

QList<INFIRMIER *> DBManager::getAllInfirmiersFromService(QString id_service)
{
    QList<INFIRMIER *> res;
    //m pour médecin p pour personne
    req->prepare("SELECT inf.id_personne as id FROM personnel as p INNER JOIN service as s ON p.id_service=s.id_servicen INNER JOIN infirmier AS inf ON p.id_personne=inf.id_personne WHERE s.id_service=:i");
    req->bindValue(":i",id_service);

    if (req->exec())
    {
        QStringList ids;
        while (req->next())
        {
            ids.append(req->value("id").toString());
        }

        req->finish();

        int i=0;

        while (i < ids.length())
        {
            res.append(getInfirmier(ids.at(i)));
            i++;
        }

        return res;
    }
    else
    {
        qDebug("error from getAllMed");
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

QList<MODEADMISSION *> DBManager::getAllModeAdmission()
{
    QList<MODEADMISSION *> res;

    req->prepare("SELECT * FROM mode_admission");


    if (req->exec())
    {
        while (req->next())
        {
            res.append(new MODEADMISSION(req->value("id_mode_admission").toString(),req->value("label").toString()));
        }

        req->finish();
        return res;
    }
    else
    {
        qDebug("error from getAllModeAdmission");
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

QList<MODESORTIE *> DBManager::getAllModeSortie()
{
    QList<MODESORTIE *> res;

    req->prepare("SELECT * FROM mode_sortie");


    if (req->exec())
    {
        while (req->next())
        {
            res.append(new MODESORTIE(req->value("id_mode_sortie").toString(),req->value("label").toString()));
        }

        req->finish();
        return res;
    }
    else
    {
        qDebug("error from getAllModeSortie");
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

QList<MODESOIN *> DBManager::getAllModeSoin()
{
    QList<MODESOIN *> res;

    req->prepare("SELECT * FROM mode_soin");


    if (req->exec())
    {
        while (req->next())
        {
            res.append(new MODESOIN(req->value("id_mode_soin").toString(),req->value("label").toString()));
        }

        req->finish();
        return res;
    }
    else
    {
        qDebug("error from getAllModeSoin");
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

QList<QUARTIER *> DBManager::getAllQuatierOfVille(QString id_ville)
{
    QList<QUARTIER *> res;

    req->prepare("SELECT * FROM quartier LEFT JOIN ville ON quartier.num_ville=ville.num_ville WHERE ville.num_ville="+id_ville);


    if (req->exec())
    {
        while (req->next())
        {

            res.append(new QUARTIER(req->value("id_quartier").toInt(),req->value("nom_quartier").toString(),new VILLE()));
        }

        req->finish();
        return res;
    }
    else
    {
        qDebug("error from getAllQuartierOfVille");
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

QList<VILLE *> DBManager::getAllVille()
{
    QList<VILLE *> res;

    req->prepare("SELECT * FROM ville ORDER BY nom_ville");


    if (req->exec())
    {
        while (req->next())
        {
            if (req->value("nom_ville").toString()!="AUCUN")
            {
                res.append(new VILLE(req->value("num_ville").toInt(),req->value("nom_ville").toString()));
            }
        }

        req->finish();
        return res;
    }
    else
    {
        qDebug("error from getAllVille");
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

QList<STATUT *> DBManager::getAllStatut()
{
    QList<STATUT *> res;

    req->prepare("SELECT * FROM statut ORDER BY label_statut");


    if (req->exec())
    {
        while (req->next())
        {
            res.append(new STATUT(req->value("id_statut").toInt(),req->value("label_statut").toString()));
        }

        req->finish();
        return res;
    }
    else
    {
        qDebug("error from getAllStatut");
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

QList<NATIONALITE *> DBManager::getAllNationalites()
{
    QList<NATIONALITE *> res;

    req->prepare("SELECT * FROM nationalite ORDER BY label_nationalite");


    if (req->exec())
    {
        while (req->next())
        {
            if (req->value("label_nationalite").toString().toUpper()=="AUCUN")
            {
                continue;
            }
            res.append(new NATIONALITE(req->value("id_nationalite").toInt(),req->value("label_nationalite").toString()));
        }

        req->finish();
        return res;
    }
    else
    {
        qDebug("error from getAllNationalite");
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

QList<ADRESSE *> DBManager::getAllAdresses(QString id_quartier)
{
    QList<ADRESSE *> res;

    req->prepare("SELECT * FROM adresse AS a INNER JOIN quartier AS q ON a.id_quartier=q.id_quartier LEFT JOIN ville AS v ON q.num_ville=v.num_ville WHERE q.id_quartier="+id_quartier);


    if (req->exec())
    {
        while (req->next())
        {
            res.append(new ADRESSE(req->value("a.id_adresse").toInt(),req->value("a.lot").toString(),new QUARTIER(id_quartier.toInt(),req->value("nom_ville").toString(),new VILLE(req->value("num_ville").toInt(),req->value("nom_ville").toString()))));
        }

        req->finish();
        return res;
    }
    else
    {
        qDebug("error from getAllStatut");
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

QList<HOPITAL *> DBManager::getAllHopital()
{
    QList<HOPITAL *> res;
    QString ids;
    if (req->exec("SELECT * FROM hopital ORDER BY nom_hopital"))
    {
        while(req->next())
        {
            ids.append(req->value("id_hopital").toString());
            res.append(new HOPITAL(req->value("id_hopital").toString(),req->value("nom_hopital").toString(),QList<SERVICE *>()));
        }

        req->finish();

        int i=0;

        while(i < ids.length())
        {
            res.at(i)->setServices(getServices(ids.at(i)));
            i++;
        }
        return res;
    }
    else
    {
        qDebug("error from getAllHopital");
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

bool DBManager::addFiche(class FICHE *fiche)
{
    qDebug("Début de l'ajout");
    QString idServiceProvenance,idServiceDestinaire,idSortie,contact;
    QString idResponsable=QString::number(fiche->getResponsble()->getId());
    QString genre= fiche->proprietire()->genre()==HOMME? "1":"0";
    QString idNationalite= QString::number(fiche->proprietire()->nationalite()->getId());

    if (idNationalite==-1)
    {
        qDebug()<< "Nationalité "+ fiche->proprietire()->nationalite()->getLabel()+ "ne inéistante ejout";
        if (req->exec("INSERT INTO nationalite (label_nationalite) VALUES("+fiche->proprietire()->nationalite()->getLabel()))
        {
            fiche->proprietire()->nationalite()->setId(req->lastInsertId().toInt());
            req->finish();
        }
        else
        {
            req->finish();
            abort("erreur ajout new nationalité");
        }

    }


    if (fiche->getProvenance()->getId()!=-1)//Vérication du mode d'admission si c'est un transfert
    {
        if (fiche->getProvenance()->nomHopital().toUpper()!="CENHOSOA")//vérifier si le patient viens des services local à CENHOSOA
        {
            idServiceProvenance=QString::number(fiche->getProvenance()->getId());

            SERVICE *s= getService(idServiceDestinaire,true);

            if (s->isNull())
            {
                if (req->exec("SELECT id_hopital FROM hopital WHERE nom_hopital='"+fiche->getProvenance()->nomHopital()+"'"))
                {
                    QString idh;
                    if (req->first())
                    {
                        idh=req->value("id_hopital").toString();
                        req->finish();
                        if (!req->exec("INSERT INTO service (nom_service) VALUES("+fiche->getProvenance()->nom()+"'"))
                        {
                            abort("service non inséré");
                            return false;
                        }
                        idServiceProvenance=req->lastInsertId().toString();

                        if (!req->exec("INSERT INTO service_de (id_hopital,id_service) VALUES("+idh+","+req->lastInsertId().toString()+")"))
                        {
                            abort("service non inséré");
                            return false;
                        }
                        req->finish();

                    }
                    else
                    {
                        req->finish();
                        if (!req->exec("INSERT INTO hopital (nom_hopital) VALUES("+fiche->getProvenance()->nomHopital()+"'"))
                        {
                              abort("hopital non inséré");
                              return false;
                        }
                        idh=req->lastInsertId().toString();
                        req->finish();
                        req->exec("INSERT INTO service (nom_service) VALUES("+fiche->getProvenance()->nom()+"'");
                        req->finish();

                        idServiceProvenance=req->lastInsertId().toString();
                        req->exec("INSERT INTO service_de (id_hopital,id_service) VALUES("+idh+","+req->lastInsertId().toString()+")");
                        req->finish();
                        idServiceProvenance=req->lastInsertId().toString();
                    }


               }

                else
                {
                    req->finish();

                    return false;
                }

            }
            else
            {
                idServiceProvenance=QString::number(s->getId());
            }
            idResponsable="NULL";//toujours null car c'est surement un médecin d'un autre sercvice

        }
        else
        {
            idServiceProvenance=QString::number(fiche->getProvenance()->getId());
        }

    }
    else
    {
        idServiceProvenance="NULL";
    }
    idServiceDestinaire= fiche->sortie()->getDestination()->getId()==-1? "NULL":QString::number(fiche->sortie()->getDestination()->getId());

    QStringList re;//Liste des requêtes à executer par rapport à la situation d'hospitalisation et adresse du patient
    if (fiche->proprietire()->getId()==-1)//Vérification de l'existence
    {
        qDebug("Niveau 1");
        if (fiche->proprietire()->adresse()->getId()==-1)
        {
            qDebug("Niveau 2");
            if (fiche->proprietire()->adresse()->quartier()->getId()==-1)
            {
                qDebug("Niveau 3");
                if (fiche->proprietire()->adresse()->quartier()->ville()->getId()==-1)
                {

                    qDebug("Niveau 4");

                    re.append("INSERT INTO ville (nom_ville) VALUES('"+fiche->proprietire()->adresse()->quartier()->ville()->nom()+"')");

                    re.append("INSERT INTO quartier (nom_quartier,num_ville) VALUES('"+fiche->proprietire()->adresse()->quartier()->nom()+"' , LAST_INSERT_ID())");

                    re.append("INSERT INTO adresse (lot,id_quartier) VALUES('"+fiche->proprietire()->adresse()->lot()+"',LAST_INSERT_ID())");


                    re.append("INSERT INTO personne (nom,prenom,age,genre,id_nationalite,id_adresse) VALUES('"+fiche->proprietire()->nom()+"','"+fiche->proprietire()->prenom()+"','"+fiche->proprietire()->age()+"',"+genre+","+QString::number(fiche->proprietire()->nationalite()->getId())+",LAST_INSERT_ID())");
                    re.append("INSERT INTO patient (id_personne,proffession,id_statut) VALUES(LAST_INSERT_ID(),'"+fiche->proprietire()->getProffession()+"',"+QString::number(fiche->proprietire()->getStatut()->getId())+")");
                    re.append("INSERT INTO admission (date_admission,id_service,id_patient,id_medecin,id_mode_admission,id_mode_soin) VALUES('"+fiche->getDate_admission().toString("yyyy-MM-dd hh:mm:ss")+"',"+idServiceProvenance+",LAST_INSERT_ID(),"+idResponsable+","+QString::number(fiche->modeadmission()->getId())+","+QString::number(fiche->modesoin()->getId())+")");
                }
                else
                {
                    qDebug("Niveau 4'");

                    re.append("INSERT INTO quartier (nom_quartier,num_ville) VALUES('"+fiche->proprietire()->adresse()->quartier()->nom()+"','"+QString::number(fiche->proprietire()->adresse()->quartier()->ville()->getId())+"')");
                    re.append("INSERT INTO adresse (lot,id_quartier) VALUES('"+fiche->proprietire()->adresse()->lot()+"', LAST_INSERT_ID())");
                    re.append("INSERT INTO personne (nom,prenom,age,genre,id_nationalite,id_adresse) VALUES('"+fiche->proprietire()->nom()+"','"+fiche->proprietire()->prenom()+"','"+fiche->proprietire()->age()+"',"+genre+","+QString::number(fiche->proprietire()->nationalite()->getId())+",LAST_INSERT_ID())");
                    re.append("INSERT INTO patient (id_personne,proffession,id_statut) VALUES(LAST_INSERT_ID(),'"+fiche->proprietire()->getProffession()+"',"+QString::number(fiche->proprietire()->getStatut()->getId())+")");
                    re.append("INSERT INTO admission (date_admission,id_service,id_patient,id_medecin,id_mode_admission,id_mode_soin) VALUES('"+fiche->getDate_admission().toString("yyyy-MM-dd hh:mm:ss")+"',"+idServiceProvenance+",LAST_INSERT_ID(),"+idResponsable+","+QString::number(fiche->modeadmission()->getId())+","+QString::number(fiche->modesoin()->getId())+")");
                }

            }
            else
            {

                if (fiche->proprietire()->adresse()->quartier()->ville()->getId()==-1)
                {

                    qDebug("Niveau 4");

                    re.append("INSERT INTO ville (nom_ville) VALUES('"+fiche->proprietire()->adresse()->quartier()->ville()->nom()+"')");

                    re.append("INSERT INTO quartier (nom_quartier,num_ville) VALUES('"+fiche->proprietire()->adresse()->quartier()->nom()+"' , LAST_INSERT_ID())");

                    re.append("INSERT INTO adresse (lot,id_quartier) VALUES('"+fiche->proprietire()->adresse()->lot()+"',LAST_INSERT_ID())");


                    re.append("INSERT INTO personne (nom,prenom,age,genre,id_nationalite,id_adresse) VALUES('"+fiche->proprietire()->nom()+"','"+fiche->proprietire()->prenom()+"','"+fiche->proprietire()->age()+"',"+genre+","+QString::number(fiche->proprietire()->nationalite()->getId())+",LAST_INSERT_ID())");
                    re.append("INSERT INTO patient (id_personne,proffession,id_statut) VALUES(LAST_INSERT_ID(),'"+fiche->proprietire()->getProffession()+"',"+QString::number(fiche->proprietire()->getStatut()->getId())+")");
                    re.append("INSERT INTO admission (date_admission,id_service,id_patient,id_medecin,id_mode_admission,id_mode_soin) VALUES('"+fiche->getDate_admission().toString("yyyy-MM-dd hh:mm:ss")+"',"+idServiceProvenance+",LAST_INSERT_ID(),"+idResponsable+","+QString::number(fiche->modeadmission()->getId())+","+QString::number(fiche->modesoin()->getId())+")");
                }
                else
                {
                    qDebug("Niveau 4'");

                    re.append("INSERT INTO quartier (nom_quartier,num_ville) VALUES('"+fiche->proprietire()->adresse()->quartier()->nom()+"','"+QString::number(fiche->proprietire()->adresse()->quartier()->ville()->getId())+"')");
                    re.append("INSERT INTO adresse (lot,id_quartier) VALUES('"+fiche->proprietire()->adresse()->lot()+"', LAST_INSERT_ID())");
                    re.append("INSERT INTO personne (nom,prenom,age,genre,id_nationalite,id_adresse) VALUES('"+fiche->proprietire()->nom()+"','"+fiche->proprietire()->prenom()+"','"+fiche->proprietire()->age()+"',"+genre+","+QString::number(fiche->proprietire()->nationalite()->getId())+",LAST_INSERT_ID())");
                    re.append("INSERT INTO patient (id_personne,proffession,id_statut) VALUES(LAST_INSERT_ID(),'"+fiche->proprietire()->getProffession()+"',"+QString::number(fiche->proprietire()->getStatut()->getId())+")");
                    re.append("INSERT INTO admission (date_admission,id_service,id_patient,id_medecin,id_mode_admission,id_mode_soin) VALUES('"+fiche->getDate_admission().toString("yyyy-MM-dd hh:mm:ss")+"',"+idServiceProvenance+",LAST_INSERT_ID(),"+idResponsable+","+QString::number(fiche->modeadmission()->getId())+","+QString::number(fiche->modesoin()->getId())+")");
                }
//                qDebug("Niveau 3'");
//                re.append("INSERT INTO adresse (lot,id_quartier) VALUES('"+fiche->proprietire()->adresse()->lot()+"',"+QString::number(fiche->proprietire()->adresse()->quartier()->getId())+")");
//                re.append("INSERT INTO personne (nom,prenom,age,genre,id_nationalite,id_adresse) VALUES('"+fiche->proprietire()->nom()+"','"+fiche->proprietire()->prenom()+"','"+fiche->proprietire()->age()+"',"+genre+","+QString::number(fiche->proprietire()->nationalite()->getId())+",LAST_INSERT_ID())");
//                re.append("INSERT INTO patient (id_personne,proffession,id_statut) VALUES(LAST_INSERT_ID(),'"+fiche->proprietire()->getProffession()+"',"+QString::number(fiche->proprietire()->getStatut()->getId())+")");
//                re.append("INSERT INTO admission (date_admission,id_service,id_patient,id_medecin,id_mode_admission,id_mode_soin) VALUES('"+fiche->getDate_admission().toString("yyyy-MM-dd hh:mm:ss")+"',"+idServiceProvenance+",LAST_INSERT_ID(),"+idResponsable+","+QString::number(fiche->modeadmission()->getId())+","+QString::number(fiche->modesoin()->getId())+")");
            }

        }
        else
        {

            qDebug("Niveau 2'");
            re.append("INSERT INTO personne (nom,prenom,age,genre,id_nationalite,id_adresse) VALUES('"+fiche->proprietire()->nom()+"','"+fiche->proprietire()->prenom()+"','"+fiche->proprietire()->age()+"',"+genre+","+QString::number(fiche->proprietire()->nationalite()->getId())+",LAST_INSERT_ID())");
            re.append("INSERT INTO patient (id_personne,proffession,id_statut) VALUES(LAST_INSERT_ID(),'"+fiche->proprietire()->getProffession()+"',"+QString::number(fiche->proprietire()->getStatut()->getId())+")");
            re.append("INSERT INTO admission (date_admission,id_service,id_patient,id_medecin,id_mode_admission,id_mode_soin) VALUES('"+fiche->getDate_admission().toString("yyyy-MM-dd hh:mm:ss")+"',"+idServiceProvenance+",LAST_INSERT_ID(),"+idResponsable+","+QString::number(fiche->modeadmission()->getId())+","+QString::number(fiche->modesoin()->getId())+")");
        }

    }
    else
    {
        qDebug("Niveau 1'");
        re.append("INSERT INTO admission (date_admission,id_service,id_patient,id_medecin,id_mode_admission,id_mode_soin) VALUES('"+fiche->getDate_admission().toString("yyyy-MM-dd hh:mm:ss")+"',"
        +idServiceProvenance+","+QString::number(fiche->proprietire()->getId())+","+idResponsable+","+QString::number(fiche->modeadmission()->getId())+","+QString::number(fiche->modesoin()->getId())+")");
    }

   DB->transaction();
    int j=0;

    while(j<re.length())
    {
        if(!req->exec(re.at(j)))
        {

            DB->rollback();
            abort(re.at(j)+ " erreur");
            return false;
        }
        j++;
    }

    j=0;
    QString idAdmission=req->lastInsertId().toString();

    QString idLastExam;

    while (j<fiche->getExamens().length())//Ajout des informations médicales
    {
        QString de= fiche->getExamens().at(j)->date().toString("yyyy-MM-dd hh:mm:ss");
        QString mr= QString::number(fiche->getExamens().at(j)->examinateur()->getId());
        if (mr=="-1")
        {
            mr="NULL";
        }
        if (de.isEmpty())
        {
            de="NULL";
        }
        if (!req->exec("INSERT INTO examen (date_exam,id_medecin,id_admission) VALUES("+de+","+mr+","+idAdmission+")"))
        {
            qDebug() << req->lastQuery();
            abort("Exam n "+QString::number(j) +" non inséré");
            return false;
        }

        idLastExam= req->lastInsertId().toString();
        req->finish();

        int k=0;

        while (k<fiche->getExamens().at(j)->propos().length())
        {
            QString idMereSelect;
           if (fiche->getExamens().at(j)->propos().at(k)->id_mere().isEmpty())
           {
              idMereSelect = " IS NULL ";
           }
           else
           {
               idMereSelect ="="+fiche->getExamens().at(j)->propos().at(k)->id_mere()+" ";
           }

            if (req->exec("SELECT id_classe FROM classe  WHERE id_classe_supra_ordonne"+idMereSelect+
                          "AND descriptif='"+autoFormat(fiche->getExamens().at(j)->propos().at(k)->descriptif())+"'"))
            {

                if (req->first())
                {
                    QString idc= req->value("id_classe").toString();
                    req->finish();

                    int m=0;

                    while (m<fiche->getExamens().at(j)->propos().at(k)->values().length())
                    {
                        if (req->exec("SELECT id_valeur FROM valeur WHERE contenu='"+autoFormat(fiche->getExamens().at(j)->propos().at(k)->values().at(m))+"'"))
                        {
                            if (req->first())
                            {
                                QString idv=req->value("id_valeur").toString();
                                req->finish();
                                if (req->exec("INSERT INTO propos (id_valeur,id_classe,id_examen) VALUES("+idv+","+idc+","+idLastExam+")"))//une erreur à corriger
                                {
                                    qDebug() << fiche->getExamens().at(j)->propos().at(k)->descriptif()+" "+fiche->getExamens().at(j)->propos().at(k)->values().at(m) +" valeur ajouté";
                                }
                                else
                                {
                                    abort("erreur lors de l'ajout de valeur dans le propos");
                                    return false;
                                }
                            }
                            else
                            {
                                req->finish();
                                req->exec("INSERT INTO valeur (contenu) VALUES('"+autoFormat(fiche->getExamens().at(j)->propos().at(k)->values().at(m))+"')");
                                req->exec("INSERT INTO propos (id_valeur,id_classe,id_examen) VALUES(LAST_INSERT_ID(),"+idc+","+idLastExam+")");
                            }
                        }
                        else
                        {
                            abort("erreur dans la recherche des valeurs 0");
                            return false;
                        }

                         m++;
                    }

                }
                else
                {
                    req->finish();

                    if (req->exec("SELECT id_classe FROM classe WHERE id_classe="+fiche->getExamens().at(j)->propos().at(k)->id_mere()))
                    {
                        if (req->first())
                        {
                            req->finish();
                            req->exec("INSERT INTO classe (descriptif,id_classe) VALUES('"+autoFormat(fiche->getExamens().at(j)->propos().at(k)->descriptif())+"',"+fiche->getExamens().at(j)->propos().at(k)->id_mere()+")");
                            QString idc=req->lastInsertId().toString();
                            int m=0;

                            while (m<fiche->getExamens().at(j)->propos().at(k)->values().length())
                            {
                                if (req->exec("SELECT id_valeur FROM valeur WHERE contenu='"+autoFormat(fiche->getExamens().at(j)->propos().at(k)->values().at(m))+"'"))
                                {
                                    if (req->first())
                                    {
                                        QString idv=req->value("id_valeur").toString();
                                        req->finish();
                                        if (req->exec("INSERT INTO propos (id_valeur,id_classe,id_examen) VALUES("+idv+","+idc+","+idLastExam+")"))
                                        {
                                            qDebug() << fiche->getExamens().at(j)->propos().at(k)->descriptif()+" "+fiche->getExamens().at(j)->propos().at(k)->values().at(m) +" valeur ajouté";
                                        }
                                        else
                                        {
                                            abort("erreur lors de l'ajout de valeur dans le propos");
                                            return false;
                                        }
                                    }
                                    else
                                    {
                                        req->finish();
                                        req->exec("INSERT INTO valeur (contenu) VALUES('"+autoFormat(fiche->getExamens().at(j)->propos().at(k)->values().at(m))+"')");
                                        req->exec("INSERT INTO propos (id_valeur,id_classe,id_examen) VALUES(LAST_INSERT_ID(),"+idc+","+idLastExam+")");
                                    }
                                }
                                else
                                {
                                    abort("erreur dans la recherche des valeurs 1");
                                    return false;
                                }

                                 m++;
                            }
                        }
                        else
                        {
                            req->finish();
                            emit error("Une valeur dans la fiche est pas valide ");
                            return false;
                        }
                    }

                }
            }
            else
            {

                qDebug() << req->lastQuery();
                abort("Erreur ajout des classes 1");
                DB->rollback();
                return false;
            }
            k++;
        }
        j++;
    }

    return true;
}

bool DBManager::addExam(QString id_admission, QList<EXAMEN *> examens)
{
    QString idlastExam;
    int j=0;

    while (j<examens.length())
    {
        if (!req->exec("INSERT INTO examen (date_exam,id_medecin,id_admission) VALUES('"+examens.at(j)->date().toString("yyyy-MM-dd hh:mm:ss")+"',"+QString::number(examens.at(j)->examinateur()->getId())+","+id_admission+")"))
        {
            qDebug() << req->lastQuery();
            abort("Exam pour "+id_admission +" non inséré");

            return false;
        }


        idlastExam= req->lastInsertId().toString();
        req->finish();

        int k=0;

        while (k<examens.at(j)->propos().length())
        {
             QString idMereSelect;
            if (examens.at(j)->propos().at(k)->id_mere().isEmpty())
            {
               idMereSelect = " IS NULL ";
            }
            else
            {
                idMereSelect =" ="+examens.at(j)->propos().at(k)->id_mere()+" ";
            }
            if (req->exec("SELECT id_classe FROM classe  WHERE id_classe_supra_ordonne"+idMereSelect+" AND descriptif='"+autoFormat(examens.at(j)->propos().at(k)->descriptif())+"'"))
            {

                if (req->first())// si la classe existe
                {
                    qDebug() << "la classe "+examens.at(j)->propos().at(k)->descriptif()+" existe";
                    QString idc= req->value("id_classe").toString();
                    req->finish();

                    int m=0;

                    while (m<examens.at(j)->propos().at(k)->values().length())//parcours de tout les valeurs
                    {
                        if (req->exec("SELECT id_valeur FROM valeur WHERE contenu='"+autoFormat(examens.at(j)->propos().at(k)->values().at(m))+"'"))
                        {

                            if (req->first())
                            {
                                qDebug("Cas ajout de propos 1");
                                QString idv=req->value("id_valeur").toString();
                                req->finish();
                                if (req->exec("INSERT INTO propos (id_valeur,id_classe,id_examen) VALUES("+idv+","+idc+","+idlastExam+")"))
                                {
                                    qDebug() << examens.at(j)->propos().at(k)->descriptif()+" "+examens.at(j)->propos().at(k)->values().at(m) +" valeur ajouté";

                                }
                                else
                                {
                                    abort("erreur lors de l'ajout de valeur dans le propos");
                                    return false;
                                }
                            }
                            else
                            {
                                qDebug("Cas ajout de propos 2");
                                req->finish();
                                req->exec("INSERT INTO valeur (contenu) VALUES('"+autoFormat(examens.at(j)->propos().at(k)->values().at(m))+"')");
                                req->exec("INSERT INTO propos (id_valeur,id_classe,id_examen) VALUES(LAST_INSERT_ID(),"+idc+","+idlastExam+")");
                                //il faut ajouter l'id de l'exam
                                req->finish();

                            }
                        }
                        else
                        {
                            abort("erreur dans la recherche des valeurs 3");
                            return false;
                        }

                         m++;
                    }

                }
                else//ce cas ci se présente que lors d'un import, donc c'est en ecore à vérifier
                {
                    qDebug() << "La classe "+ examens.at(j)->propos().at(k)->descriptif()+" n'existe pas";
                    req->finish();

//                    if (req->exec("SELECT id_classe FROM classe WHERE id_classe="+examens.at(j)->propos().at(k)->id_mere()))
//                    {
//                        if (req->first())
//                        {
//                            req->finish();
                    req->exec("INSERT INTO classe (descriptif,id_classe) VALUES('"+autoFormat(examens.at(j)->propos().at(k)->descriptif())+"',"+examens.at(j)->propos().at(k)->id_mere()+")");
                    QString idc=req->lastInsertId().toString();
                    int m=0;

                    while (m<examens.at(j)->propos().at(k)->values().length())
                    {
                        if (req->exec("SELECT id_valeur FROM valeur WHERE contenu='"+autoFormat(examens.at(j)->propos().at(k)->values().at(m))+"'"))
                        {
                            if (req->first())
                            {
                                qDebug() << "Cas 3";
                                QString idv=req->value("id_valeur").toString();
                                req->finish();
                                if (req->exec("INSERT INTO propos (id_valeur,id_classe,id_examen) VALUES("+idv+","+idc+","+idlastExam+")"))
                                {
                                    qDebug() << examens.at(j)->propos().at(k)->descriptif()+" "+examens.at(j)->propos().at(k)->values().at(m) +" valeur ajouté";
                                }
                                else
                                {
                                    abort("erreur lors de l'ajout de valeur dans le propos");
                                    return false;
                                }
                            }
                            else
                            {
                                qDebug("cas 4 d'ajout caleurs");
                                req->finish();
                                req->exec("INSERT INTO valeur (contenu) VALUES('"+autoFormat(examens.at(j)->propos().at(k)->values().at(m))+"')");
                                req->exec("INSERT INTO propos (id_valeur,id_classe,id_examen) VALUES(LAST_INSERT_ID(),"+idc+","+idlastExam+")");
                            }
                        }
                        else
                        {
                            abort("erreur dans la recherche des valeurs 3");
                            return false;
                        }

                         m++;
                    }
//                        }
//                        else
//                        {
//                            req->finish();
//                            emit error("Une valeur dans la fiche est pas valide ");
//                            return false;
//                        }
//                    }

                }
            }
            else
            {
                qDebug() << req->lastQuery();
                abort("Erreur ajout des classes");
                return false;
            }
            k++;
        }
        j++;
    }
    return  true;
}

bool DBManager::addSortie(QString id_admission, SORTIE *s)
{
    QString id_hopital="";
    QString id_service="";
    if (s->getDestination()->getId()==-1)
    {
        if(req->exec("SELECT id_hopital FROM hopital WHERE nom_hopital='"+s->getDestination()->nomHopital()+"'"))
        {
            if (req->first())
            {
                id_hopital= req->value("id_hopital").toString();
                req->finish();
            }
            else
            {
                req->finish();

                if (req->exec("INSERT INTO hopital (nom_hopital) VALUES('"+s->getDestination()->nomHopital()+"')"))
                {
                    id_hopital= req->lastInsertId().toString();
                }
                else
                {
                    abort("erreur lors de l'ajout de l'information Hopital");
                    return  false;
                }
            }
        }
        else
        {
            qDebug() << req->lastQuery();
            abort("Erreur lors de la vérification de l'existence de l'hopital");
            return false;
        }

        if (req->exec("INSERT INTO service (nom_service) VALUES('"+s->getDestination()->nom()+"')"))
        {
            id_service= req->lastInsertId().toString();
        }
        else
        {
            abort("erreur lors de l'ajout de la nouvelle service");
            return false;
        }

        if (!req->exec("INSERT INTO service_de (id_service,id_hopital) VALUES("+id_service+","+id_hopital+")"))
        {
            abort("erreur lors de la liaison du nouveau service et hopital");
            return false;
        }
    }
    else
    {
        id_service= QString::number(s->getDestination()->getId());
    }

    if (!req->exec("INSERT INTO sortie(date_sortie,id_service,id_mode_sortie,id_admission) VALUES('"+s->getDate().toString("yyyy-MM-dd hh:mm:ss")+"',"+id_service+","+QString::number(s->mode()->getId())+","+id_admission+")"))
    {
            qDebug() << req->lastQuery();

            abort("erreur lors de la lisaison de sortie");

            return false;
    }

    return true;
}

int DBManager::getHospisalisationNumber(PATIENT *pat)
{
    if (pat->isNull())
    {
        qWarning("patient null dans getHospitalisationNumber");
        req->finish();
        return -1;
    }

    if (req->exec("SELECT COUNT(*) AS nb_hosp FROM admission WHERE id_patient='"+QString::number(pat->getId())+"'"))
    {
        req->first();

        int val= req->value("nb_hosp").toInt();
        req->finish();
        return val;
    }
    else
    {
        emit errBrute(req->lastError().type());
        req->finish();
        return -1;
    }

}

bool DBManager::setModelFromDB()
{
    //Récupération des entêtes
    if (req->exec("SELECT * FROM classe"))
    {
        QStringList id,desc,ids;
        QString idPA,descPA,idsPA;
        int i=0;
        QList<bool> modifiableValue;
        QList<bool> parametrable;


        QString supra;
        while (req->next())
        {


            supra=req->value("id_classe_supra_ordonne").toString();
            if (supra=="0")
            {
//                modifiableValue.append(false);
//                parametrable.append(false);
                supra="";
            }
//            else
//            {
//                modifiableValue.append(true);
//                parametrable.append(true);
//            }

            if (req->value("id_classe").toString()=="29")
            {
                id.insert(1,req->value("id_classe").toString());
                desc.insert(1,req->value("descriptif").toString());
                ids.insert(1,supra);
                if (supra=="")
                {
                    modifiableValue.insert(1,false);
                    parametrable.insert(1,false);
                }
                else
                {
                    modifiableValue.insert(1,true);
                    parametrable.insert(1,true);
                }
            }
            else if(req->value("id_classe").toString()=="30")
            {
                id.insert(5,req->value("id_classe").toString());
                desc.insert(5,req->value("descriptif").toString());
                ids.insert(5,supra);
                if (supra=="")
                {
                    modifiableValue.insert(5,false);
                    parametrable.insert(5,false);
                }
                else
                {
                    modifiableValue.insert(5,true);
                    parametrable.insert(5,true);
                }

            }
            else if (req->value("id_classe").toString()=="31")
            {
                id.insert(2,req->value("id_classe").toString());
                desc.insert(2,req->value("descriptif").toString());
                ids.insert(2,supra);
                if (supra=="")
                {
                    modifiableValue.insert(2,false);
                    parametrable.insert(2,false);
                }
                else
                {
                    modifiableValue.insert(2,true);
                    parametrable.insert(2,true);
                }
            }

            else
            {
                if (supra=="")
                {
                    modifiableValue.append(false);
                    parametrable.append(false);
                }
                else
                {
                    modifiableValue.append(true);
                    parametrable.append(true);
                }

                id.append(req->value("id_classe").toString());
                desc.append(req->value("descriptif").toString());
                ids.append(supra);
            }


            i++;

        }

        i=0;
        //Insertion des des menus dans le fichier xml
        while (i<id.length())
        {
            qDebug() << desc.at(i);

            xmlManager->addMenu(id.at(i),desc.at(i),"1",QList<QDomElement>(),ids.at(i),QString(),modifiableValue.at(i),parametrable.at(i),true);
            xmlManager->addValue(id.at(i),"oui",true,true);
            xmlManager->addValue(id.at(i),"non",true,false);
            i++;
        }

        req->finish();

        return true;

    }
    else
    {
        emit errBrute(req->lastError().type());
        req->finish();
        return false;
    }
}

bool DBManager::addValue(QString valeur)
{
    if (!getIdClass(valeur).isEmpty())
    {
        qDebug() << "Valeur "+valeur+"existante";
        return true;
    }

    req->prepare("INSERT INTO valeur(contenu) VALUES(:c)");
    req->bindValue(":c",valeur);
    if(!req->exec())
    {
        emit errBrute(req->lastError().type());
        req->finish();
        return  false;
    }

    return true;

}

QString DBManager::addClass(QString desc, QString id_supra)
{
    QString id;
    QString r;

    if(id_supra.isEmpty())
    {
        r="INSERT INTO classe (descriptif) VALUES(:d)";
        req->prepare(r);
        req->bindValue(":d",desc);
    }
    else
    {
        r="INSERT INTO classe (descriptif,id_classe_supra_ordonne) VALUES(:d,:i)";
        req->prepare(r);
        req->bindValue(":d",desc);
        req->bindValue(":i",id_supra);
    }

    if (req->exec())
    {
        id= req->lastInsertId().toString();
        if (id_supra.isEmpty())
        {
//            if (req->exec("INSERT INTO super_classe ('"+req->lastInsertId().toString()+"')"))
//            {
//                req->finish();
//                return true;
//            }
//            else
//            {
//                emit errBrute(req->lastError().type());
//                req->finish();
//                return false;
//            }
        }
        else
        {
            req->finish();
            return id;
        }
    }
    else
    {

        qDebug("Error from add classe");
        qDebug() << req->lastQuery();
        emit errBrute(req->lastError().type());
        req->finish();
        return QString();
    }

}

QString DBManager::getIdValue(QString content)
{
    req->prepare("SELECT id_valeur FROM valeur WHERE contenu=:v");

    req->bindValue(":v",content);

    if(req->exec())
    {
        if (req->first())
        {
            req->finish();
            qDebug() << "valeur "+content+ " exist";
            return req->value("id_valeur").toString();
        }
        else
        {
            req->finish();

            return QString();
        }
    }
    else
    {
        emit errBrute(req->lastError().type());
        req->finish();
        return QString();
    }
}

QString DBManager::getIdClass(QString content)
{
    req->prepare("SELECT id_classe FROM valeur WHERE descriptif=:v");

    req->bindValue(":v",content);

    if(req->exec())
    {
        if (req->first())
        {
            req->finish();
            qDebug() << "valeur "+content+ " exist";
            return req->value("id_classe").toString();
        }
        else
        {
            req->finish();

            return QString();
        }
    }
    else
    {
        emit errBrute(req->lastError().type());
        req->finish();
        return QString();
    }
}

QList<CLASSE *> DBManager::getClasses(QString id_mere)
{
    QList<CLASSE *> res;
    QString las;
    QStringList desc;
    QStringList id;

    if (id_mere.isEmpty())
    {
        las= "IS NULL";
    }
    else
    {
        las= " = "+ id_mere;
    }

    if (req->exec("SELECT * FROM classe WHERE id_classe_supra_ordonne "+las))
    {
        while (req->next())
        {
            id.append(req->value("id_classe").toString());
            res.append(new CLASSE(req->value("id_classe").toString(),req->value("descriptif").toString(),QStringList(),req->value("id_classe_supra_ordonne").toString(),QList<CLASSE *>()));
        }

        req->finish();

        //récupération des  sous classes
       int i=0;

       while (i< res.length())
       {
           res.at(i)->setSubClasses(getClasses(QString::number(res.at(i)->getId())));
           i++;
       }

            return res;
    }
    else
    {
        req->finish();
        abort("erreur dans get classes");
        res;
    }
}

QList<CLASSE *> DBManager::findSubclassesOf(CLASSE *mere, QList<CLASSE *> others)
{
    QList<CLASSE *> res;
    QList<CLASSE *> others2;
    int i=0;
    while (i<others.length())
    {
        if (others.at(i)->id_mere().toInt() ==mere->getId())
        {
            res.append(others.at(i));
        }
        else
        {
            others2.append(others.at(i));
        }

        i++;
    }

    i=0;
    while(i<res.length())
    {
        res.at(i)->setSubClasses(findSubclassesOf(res.at(i),others2));
        i++;
    }

    return res;
}

QList<EXAMEN *> DBManager::getExamensOf(QString id_admission)
{
    qDebug() << id_admission;
    QList<EXAMEN *> res;
    QStringList idExaminateurs;
//    QStringList ide;
    req->prepare("SELECT e.id_examen AS ide, e.date_exam AS de, e.id_medecin as idme FROM examen as e INNER JOIN admission as a ON e.id_admission=a.id_admission  WHERE a.id_admission=:i ORDER BY de");
    req->bindValue(":i",id_admission);
    if (req->exec())
    {
        while (req->next())
        {
            idExaminateurs.append(req->value("idme").toString());
//            ide.append(req->value("ide").toString());
            res.append(new EXAMEN(req->value("ide").toString(),req->value("de").toDateTime(),new MEDECIN(),QList<CLASSE *>()));
        }
        req->finish();

        //récupération des responsable d'un examen

        int i=0;
        while (i<res.length())
        {
            res.at(i)->setExaminateur(getMedecin(idExaminateurs.at(i)));
            i++;
        }

        i=0;

        //récupération des classes

        while(i<res.length())
        {
            QList<CLASSE *> classes;
             if (req->exec("SELECT c.id_classe as id,c.descriptif as des , c.id_classe_supra_ordonne as ids FROM propos AS p INNER JOIN examen AS e ON p.id_examen=e.id_examen INNER JOIN classe as c ON C.id_classe=p.id_classe WHERE e.id_examen="+QString::number(res.at(i)->getId())))
             {
                  while (req->next())
                  {
                     classes.append(new CLASSE(req->value("id").toString(),req->value("des").toString(),QStringList (),req->value("ids").toString(),QList<CLASSE *>()));
                  }
                  req->finish();

                  //récupération des valeurs de chaque classes
                  int j=0;
                  while (j<classes.length())
                  {
                      QStringList val;
                      val.clear();
                      if (req->exec("SELECT  v.contenu AS cont FROM valeur as v INNER JOIN propos as p ON p.id_valeur=v.id_valeur WHERE p.id_classe="+QString::number(classes.at(j)->getId())+" AND p.id_examen="+QString::number(res.at(i)->getId())))//récupération des valeurs d'une classe sur l'examen actuel
                      {

                          while (req->next())
                          {
                              val.append(req->value("cont").toString());
                          }
                          req->finish();

                      }
                      else
                      {
                          abort("Erreur lors de la récupération des valeurs de classe");
                      }

                      classes.at(j)->setValues(val);
                      j++;
                  }


             }
             else
             {
                 abort("erreur de récupération des classes dan getExamenOf");
             }

             res.at(i)->setPropos(classes);
            i++;
        }



        //récupération des mères

        i=0;

        QList<CLASSE *> meres;
        QList<CLASSE *> others;

        //récupération des mere, c'est le propos de res 0 car tout les admission ont une première exam vide contenant tout
        //les classes mères
        int j=0;
        while (j<res.at(0)->propos().length())

        {
            if (res.at(i)->propos().at(j)->id_mere()=="0")
            {
                qDebug() << "mere "+res.at(i)->propos().at(j)->descriptif()+" récup";
                meres.append(res.at(i)->propos().at(j));
            }
//            else
//            {
//                others.append(res.at(i)->propos().at(j));
//            }
            j++;
        }

        while (i<res.length())//organisation des classes hiérachiquement
        {

            int k=0;

            while (k<meres.length())//récupération des sous classes
            {
                meres.at(k)->setSubClasses(findSubclassesOf(meres.at(k),res.at(i)->propos()));
//                int l=0;
//                QList<CLASSE *> subClasses;//conteneur des sous classes de la mère actuelle
//                QList<CLASSE *> others2;
//                while(l<others.length())
//                {
//                    if (others.at(l)->id_mere().toInt()==meres.at(k)->getId())
//                    {
//                        subClasses.append(others.at(l));
//                    }
//                    else
//                    {
//                        others2.append(others.at(l));
//                    }
//                    l++;
//                }

//                meres.at(k)->setSubClasses(subClasses);

//                //parmi les fils des mères faire la meme recherche jusqu'à ce qu'un fils qui deviens une mère n'a plus de fils
//                int m=0;

//                while (m<meres.at(k)->subClasses().length())
//                {
//                   meres.at(k)->subClasses().at(m)->setSubClasses(findSubclassesOf(meres.at(k)->subClasses().at(m),others2));
//                    m++;
//                }

                k++;
            }

            res.at(i)->setPropos(meres);
            i++;
        }


        i=0;
        bool allEmpty;
        QList<EXAMEN *> swap;
        QList<CLASSE *> swapPropos;
        EXAMEN *tmp;
        while (i<res.length())//filtrage des classes sans valeurs
        {
            allEmpty=true;
            swapPropos= QList<CLASSE *>();
            int k=0;
            while (k<res.at(i)->propos().length())
            {
                if (!res.at(i)->propos().at(k)->subClasses().isEmpty())
                {
                    qDebug() << "Non vide"+ res.at(i)->propos().at(k)->descriptif();
                    if (allEmpty)
                        allEmpty=false;
                    swapPropos.append(res.at(i)->propos().at(k));

                }

                k++;
            }

            if (!allEmpty)
            {
                tmp=new EXAMEN(QString::number(res.at(i)->getId()),res.at(i)->date(),res.at(i)->examinateur(),swapPropos);
                swap.append(tmp);
            }

            i++;
        }

        return  swap;

    }
    else
    {
        abort("erreur dans getExamensOf");
        return res;
    }
}

QList< class FICHE *> DBManager::getFicheOf(QString id_patient)
{
    PATIENT *proprietaire= getPatient(id_patient);
    QString reqText="SELECT dest.id_service AS iddes,mos.id_mode_sortie AS idmos,mos.label AS mosl, sor.id_sortie AS idsor, sor.date_sortie as dsor,ser.id_service AS idser, a.id_admission AS id ,a.date_admission AS da ,m.id_personne AS mid, ma.id_mode_admission AS idma, ma.label AS mal, ms.id_mode_soin AS idms, ms.label AS msl  FROM admission AS a INNER JOIN patient AS p ON a.id_patient=p.id_personne INNER JOIN mode_soin AS ms ON a.id_mode_soin=ms.id_mode_soin INNER JOIN mode_admission AS ma ON a.id_mode_admission=ma.id_mode_admission LEFT JOIN medecin AS m ON a.id_medecin=m.id_personne LEFT JOIN service as ser ON ser.id_service=a.id_service LEFT JOIN sortie as sor ON sor.id_admission=a.id_admission LEFT JOIN  mode_sortie AS mos ON mos.id_mode_sortie=sor.id_mode_sortie LEFT JOIN service AS dest ON dest.id_service=sor.id_service  WHERE p.id_personne="+id_patient;
    if (req->exec(reqText))
    {
        QList< class FICHE *> res;
        QStringList idm, idSers,idSor,dest;
        QStringList ida;

        while(req->next())
        {
            ida.append(req->value("id").toString());
            dest.append(req->value("iddes").toString());
            idSor.append(req->value("idsor").toString());
            idSers.append(req->value("idser").toString());
            idm.append(req->value("mid").toString());

            QString iddes= req->value("iddes").toString();
            QString idmos= req->value("idmos").toString();
            QString idsor= req->value("idsor").toString();

            if (iddes=="0")
                iddes="-1";
            if(idmos=="0")
                idmos="-1";
            if (idsor=="0")
                idsor="-1";
            res.append(new class FICHE(req->value("id").toString(),proprietaire,req->value("da").toDateTime(),new MODEADMISSION(req->value("idma").toString(),req->value("mal").toString()),new MODESOIN(req->value("idms").toString(),req->value("msl").toString()),new MEDECIN(),QList<EXAMEN *>(),new SERVICE(),new SORTIE(idsor,req->value("dsor").toDateTime(),new MODESORTIE(idmos,req->value("mosl").toString()),new SERVICE())));
        }

        req->finish();
        int i=0;
        while(i < idm.length())
        {
            res.at(i)->sortie()->setDestination(getService(dest.at(i)));
            res.at(i)->setProvenance(getService(idSers.at(i)));
            res.at(i)->setResponsble(getMedecin(idm.at(i)));
            res.at(i)->setExamens(getExamensOf(QString::number(res.at(i)->getId())));
            i++;
        }
        return  res;
    }
    else
    {
        qDebug("error form getFicheOf");
        emit errBrute(req->lastError().type());
        req->finish();
        return QList< class FICHE *>();
    }

}

class FICHE *DBManager::getFicheOf(QString id_patient, QString num)
{
    PATIENT *proprietaire= getPatient(id_patient);
    class FICHE* res = new class FICHE();

    QString reqText="SELECT dest.id_service AS iddes,mos.id_mode_sortie AS idmos,mos.label AS mosl, sor.id_sortie AS idsor, sor.date_sortie as dsor,ser.id_service AS idser, a.id_admission AS id ,a.date_admission AS da ,m.id_personne AS mid, ma.id_mode_admission AS idma, ma.label AS mal, ms.id_mode_soin AS idms, ms.label AS msl  FROM admission AS a INNER JOIN patient AS p ON a.id_patient=p.id_personne INNER JOIN mode_soin AS ms ON a.id_mode_soin=ms.id_mode_soin INNER JOIN mode_admission AS ma ON a.id_mode_admission=ma.id_mode_admission LEFT JOIN medecin AS m ON a.id_medecin=m.id_personne LEFT JOIN service as ser ON ser.id_service=a.id_service LEFT JOIN sortie as sor ON sor.id_admission=a.id_admission LEFT JOIN  mode_sortie AS mos ON mos.id_mode_sortie=sor.id_mode_sortie LEFT JOIN service AS dest ON dest.id_service=sor.id_service  WHERE p.id_personne="+id_patient+ " AND a.id_admission="+num;

    if (req->exec(reqText))
    {
        QString idm,idSers,idSor, dest;
        if (req->first())
        {
            dest=req->value("iddes").toString();
            idSor=req->value("idsor").toString();
            idSers=req->value("idser").toString();
            idm=req->value("mid").toString();

            QString iddes= req->value("iddes").toString();
            QString idmos= req->value("idmos").toString();
            QString idsor= req->value("idsor").toString();


            if (iddes=="0")
                iddes="-1";
            if(idmos=="0")
                idmos="-1";
            if (idsor=="0")
                idsor="-1";

            res =new class FICHE(req->value("id").toString(),proprietaire,req->value("da").toDateTime(),new MODEADMISSION(req->value("idma").toString(),req->value("mal").toString()),new MODESOIN(req->value("idms").toString(),req->value("msl").toString()),new MEDECIN(),QList<EXAMEN *>(),new SERVICE(),new SORTIE(idsor,QDateTime::fromString(req->value("dsor").toString()),new MODESORTIE(idmos,req->value("mosl").toString()),new SERVICE()));
        }

        req->finish();

        if (!dest.isEmpty() || !idSers.isEmpty() || !idm.isEmpty())
        {
            res->sortie()->setDestination(getService(dest));
            res->setProvenance(getService(idSers));
            res->setResponsble(getMedecin(idm));
        }
        return  res;
    }
    else
    {
        emit errBrute(req->lastError().type());
        req->finish();
        return res;
    }
}

void DBManager::showError(QSqlError::ErrorType text)
{
    QString content;

    switch (text)
    {
        case QSqlError::ConnectionError: content ="Erreur de connexion,assurez vous que les paramètres du serveur de donnée sont correct et que le server est en marche!\n Accedez aux paramètres:Outils > Paramètres > Serveur de donnée";
        break;
    case QSqlError::StatementError: content="Requête invalide,veuillez vérifier les saisies ou informer les responsables techniques";
        break;
    case QSqlError::TransactionError: content="Requete non aboutit jusqu'à la fin veillez réessayer";
        break;
    default: content="Une erreur inconnue s'est produite! si l'erreur perciste veuillez en informer les responsables techniques";
        break;

    }

    content+=" \n Détails: "+DB->lastError().text();

    emit error(content);//signale que les fenêtres reçoivent

    qDebug ()<< "Une erreur s'est produite "+ content;
}

void DBManager::xmlError(QString err)
{
    emit error(err);
}

void DBManager::connectionStateChangedAction(bool state)
{
    this->setConnection(state);
    emit connectionStateChanged(state);//signal pour renseigner mainwindow que l'état da la connexion a changé
    if(state)
    {
        DB->close();
        qDebug() <<" reconnection username:" << DB->userName();
        DB->open();//se reconnecter automatiquement si la connection est rétablie
    }
}
