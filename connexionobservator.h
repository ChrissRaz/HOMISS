#ifndef CONNEXIONOBSERVATOR_H
#define CONNEXIONOBSERVATOR_H

#include <QObject>
#include <QtSql>
#include <QThread>
#include <QDebug>
#include "hxmlmanager.h"


class connexionObservator: public QObject
{
    Q_OBJECT

public:
    connexionObservator();
    ~connexionObservator();

    void setReq(QSqlQuery *value);

    bool getStop() const;
    void setStop(bool value);

public slots:
    void verifierConnection();
    void xmlError(QString err);
    void stopDB();
signals:
    void connexionStateChanged(bool state);
    void databaseInstanceError(QString message);//emited when an error occured
    void fini();


protected:
    bool connection;
    QSqlDatabase *db;
    HXmlManager *xmlManager;
    bool stop;
};

#endif // CONNEXIONOBSERVATOR_H
