#include "connexionobservator.h"


connexionObservator::connexionObservator()
{
    this->setStop(false);
    connection=false;
    QSqlDatabase::removeDatabase("ObservatorDB");
    db= new QSqlDatabase();
    xmlManager= new HXmlManager(this);
}

connexionObservator::~connexionObservator()
{
}


void connexionObservator::verifierConnection()
{
    QSqlDatabase::removeDatabase("ObservatorDB");

    db=new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL","ObservatorDB"));

    db->setHostName(xmlManager->getDatabaseParameter(DBInfoType::HOSTNAME));
    db->setUserName(xmlManager->getDatabaseParameter(DBInfoType::USERNAME));
    db->setPassword(xmlManager->getDatabaseParameter(DBInfoType::PASSWORD));
    db->setDatabaseName(xmlManager->getDatabaseParameter(DBInfoType::DBNAME));
    db->setPort(xmlManager->getDatabaseParameter(DBInfoType::PORT).toInt());


    if (db->isValid())
    {
        connection=db->open();
    }
    else
    {
        emit databaseInstanceError("Le paramètre de la base de donnée est invalide!\n celà est due à une mal configuration ou une modification du fichier de configuration");
    }

    emit connexionStateChanged(connection);
    connect(xmlManager,SIGNAL(fileNotOpened(QString)), this, SLOT(xmlError(QString)));
    connect(this,&connexionObservator::destroyed,this,&connexionObservator::stopDB);

    while (!getStop())
    {

        bool lastValue=connection;
        connection = db->open();

        if (lastValue!=connection)
        {
            emit connexionStateChanged(connection);
            qDebug () << "Changed " << connection;
        }

        QThread::msleep(100);
        db->close();
    }

    if (db->isOpen())
        db->close();
    QSqlDatabase::removeDatabase("ObservatorDB");
    emit fini();
    qDebug("stoped");
}

void connexionObservator::xmlError(QString err)
{
    setStop(true);
    emit databaseInstanceError(err);
}

void connexionObservator::stopDB()
{
    db->close();
}

bool connexionObservator::getStop() const
{
    return stop;
}

void connexionObservator::setStop(bool value)
{
    stop = value;
}

