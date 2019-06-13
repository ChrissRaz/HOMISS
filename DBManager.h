#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QString>
#include <QList>
#include <QThread>
#include "connexionobservator.h"
#include <QTimer>
#include "HObject/hobjects.h"
#include <QtCharts>

using namespace std;

enum OPERATOR {OR,AND};
enum TYPEVALEUR {CHOIX,PERSONALISE};
enum SITUATION {DECEDE,TRASFERE,FT,TI,TE};//Fin Traitement, Traitement Interne, Traitement externe
enum TRAITEMENT {HOSP,EXT};
enum TYPESTAT {NORMALE,INTERVALLE};


class DBManager: public QObject
{
    Q_OBJECT

public:
    DBManager(QString hote, QString userName, QString password, QString DBName, int port, QObject *parent=nullptr);
    DBManager(QSqlDatabase &database,QObject *parent=nullptr);
    ~DBManager();

    void setHote(QString *hote);

    void setUserName(QString *userName);

    void setPassword(QString *password);

    void setDBName(QString *DBName);

    void setPort(int *value);


    QSqlDatabase *getDB() const;
    void setDB(QSqlDatabase *value);
    bool isConnected();
    bool setAppId(QString id);
    bool insertLog(QString action, QString idApp, QString idPers, QString idAdmission);

    PERSONNE *getPersonne(QString id);
    VILLE *getVille(QString id);
    QUARTIER *getQuatier(QString id);
    ADRESSE *getAdresse(QString id);
    QList<CONTACT *> getContacts(QString id_personne);
    PATIENT *getPatient(QString id);
    PERSONEL* getPersonel(QString id);
    SANTE *getSante(QString id);
    MEDECIN *getMedecin(QString id);
    INFIRMIER *getInfirmier(QString id);
    NATIONALITE *getNationalite(QString id);
    QDate getNaissance(QString id_personne);
    SM *getStatutMilitaire(QString id);
    STATUT *getStatut(QString id);

    SORTIE* getSortie(PATIENT *pat);//this one get the last sortie
    SORTIE* getSortie(QString id_admission);
    QString getIdLastAdmission(PATIENT *pat);

    int getHospisalisationNumber(PATIENT *pat);//
    QString getSituation(PATIENT *pat);
    MODESOIN *getModeSoin(QString id_admission);

    QList<PATIENT *> getAllPatients();
    SERVICE *getService(QString id,bool withPers=false);

    QList<SERVICE*> getServices(QString id_hopital="1",bool withPers=false);



    QList<MEDECIN *> getAllMedecinsFromService(QString id_service);
    QList<INFIRMIER *> getAllInfirmiersFromService(QString id_service);

    QList<MODEADMISSION *> getAllModeAdmission();
    QList<MODESORTIE *> getAllModeSortie();
    QList<MODESOIN *>getAllModeSoin();
    QList<QUARTIER *> getAllQuatierOfVille(QString id_ville);
    QList<VILLE *> getAllVille();
    QList<STATUT *> getAllStatut();
    QList<NATIONALITE *> getAllNationalites();
    QList<ADRESSE *> getAllAdresses(QString id_quartier);//séléctionner tout les adresse des patients dans ce quatier
    QList <HOPITAL *> getAllHopital();



    bool addFiche(class FICHE *fiche);
    bool addExam(QString id_admission, QList<EXAMEN *> examens);
    bool addSortie(QString id_admission,SORTIE *s);

    QStringList getIdAdmission(PATIENT *pat);

    QString isClassExist(QString id_super, QString description);
    QString isValueExist(QString content);

    //model de fiche
    bool setModelFromDB();
    bool addValue(QString valeur);

    QString addClass(QString desc,QString id_supra=QString());
    QString getIdValue(QString content);//pourra servir à vérifier l'éxistence d'un valeur...
    QString getIdClass(QString content);

    QList<CLASSE *> getClasses(QString id_mere=QString());
    QList<CLASSE *> findSubclassesOf (CLASSE *mere,QList<CLASSE *> others);

    QList<EXAMEN *> getExamensOf(QString id_admission);
    QList <class FICHE*> getFicheOf(QString id_patient);
    class FICHE* getFicheOf(QString id_patient,QString num);


    QSqlQuery *getReq() const;

    QPieSeries* getStat(QString reqString, TYPESTAT type=NORMALE, int interv=0);

    QStringList getAllValues();

private:
    bool getWithMainDb() const;
    void setWithMainDb(bool withMainDb);
    void configureDB(QString hote, QString userName, QString password, QString DBName, int port);
    void instantiate();
    void setConnection(bool value);
    void stopThread();

    void abort(QString message);



public slots:

    bool userConnectionApp(QString const&un,QString const &pass);//connection de l'utilisateur

    void seConnecter();//connection à la base de donnée

signals:
    void goVerif();//connextion observator thread launcher
    void errBrute(QSqlError::ErrorType text);
    void connected();
    void error(QString text);
    void userConnection(bool result,int idUser);//result of connection 
    void connectionStateChanged(bool state);

private slots:
    void showError(QSqlError::ErrorType text);
    void xmlError(QString err);
    void instatiateConnectionThread();
    void connectionStateChangedAction(bool state);
    void reload();


protected:

    QString autoFormat(QString entre);
    QSqlDatabase *DB;
    QSqlQuery *req;
    QString *m_hote,*m_userName, *m_password,*m_DBName;
    int *m_port;
    bool connection=false;
    QThread *th;
    connexionObservator *observator;
    HXmlManager  *xmlManager;

    bool m_withMainDb;


};

#endif // DBMANAGER_H
