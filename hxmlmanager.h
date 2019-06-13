#ifndef HXMLMANAGER_H
#define HXMLMANAGER_H

#include <QtXml>
#include <QFile>
#include <QtSql>
#include <QRandomGenerator>
#include <QDateTime>
#include "HObject/service.h"
#include <QtWidgets>
#include "HObject/HWidget/hwidgets.h"

enum startParamaterType
{
    ID,
    GLOBAL,
    FICHE,
    SER
};

enum DBInfoType
{
    HOSTNAME,
    USERNAME,
    PASSWORD,
    DBNAME,
    PORT
};


enum LINE
{
    PARAM,AJOUT
};

class HXmlManager: public QObject
{
    Q_OBJECT
public:
    HXmlManager(QObject *parent=nullptr);
    ~HXmlManager();

    QSqlDatabase getDataBase();//+
    QString getDatabaseParameter(DBInfoType type);//+
    bool getStartParameter(startParamaterType type);//+
    QString getAppId();//+
    SERVICE* getService();





    //Manipulation du modèle
    QDomElement addValue(QString id_menu, QString value, bool used=true, bool defaultVal=false);
    QDomElement addMenu(QString id, QString desc, QString mode, QList<QDomElement > values, QString id_supra=QString(), QString alias=QString(), bool ValueModifiable=true, bool parametrable=true, bool valueInverted=false, QDomNode ref=QDomNode(), bool insertAfter=true);
    QDomElement getMenu(QString id, QDomElement container);
    QDomElement updateMenu(QString id, QString desc, QString mode, bool usage, QString alias, QString nbVal, bool valueInverted);
    QDomElement updateValue(QString id_menu, QString lastV, QString value, bool used, bool defaultVal);
    QList<QDomElement> addValues(QString id_menu,QList<QDomElement> values);
    QList<QDomElement> getSuperMenus();
    QList<QDomElement> getMenuChilds(QString id);
    QList<QDomElement> getValuesOfMenu(QString id, bool withNull=false);
    QString getDefaultValueOfMenu(QString id);

    QDomElement getValueOfMenu(QString id_menu, QString id_value, bool alsoNull=false);
    QDomElement setUsageValue(QString id_menu, QString content, bool usage);
    QDomElement setUsageMenu(QString id,bool usage);//les valeurs sont hérités

    //
    QDomDocument *getConfigurationDom() const;
    QDomElement findElement(QDomElement elem, const QString &tagName);//+

private:

    bool modifAttr(QDomNode &mere, QString const &tagname, QString const &attrName, QString const& newValue);//+
    bool updateFile(QFile &file, const QString &content); //+
    bool updateConfigFIle();//appelé à chaque enregistrement +
    bool reloadDom(QFile &source, QDomDocument &target);//+
    bool reloadConfigDom();//+

    QDomElement getConnectedUser();//+
    QDomElement getUser(QString id_user);

public slots:
    bool setDatabaseParameters(QString host, QString un, QString pass, QString DBName, QString port);//+
    bool setStartParameters(startParamaterType type, bool newVal);//+
    bool setAppId(QString const& value);//+
    bool setService(QString id,QString nom);

    void addUserConnected(QString id);//+
    void deconnectUser(QString id);//+

    QString getIdConnectedUser();//+
    QString getDateConnection(QString id_user);
    QString getUserConnectedConnexionDate();
    bool isUserConnected();

    void showError(QString msg);

signals:

    void fileNotOpened(QString message);
    void menuExistante();
protected:
    void error();//déclencheur de signal d'erreur
    QFile *configurationFile;
    QDomDocument *configurationDom;
};


#endif // HXMLMANAGER_H
