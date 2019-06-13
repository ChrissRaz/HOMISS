#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "DBManager.h"
#include <QtXml>
#include <QFile>
#include "parametre.h"
#include "service_ui.h"
#include <QtAwesome.h>
#include <QRandomGenerator>
#include "HObject/hobjects.h"
#include "HObject/HWidget/patientview.h"
#include "HObject/HWidget/examligne.h"
#include <QPrinter>
#include "HObject/HWidget/transferer.h"
#include "HObject/HWidget/hchartview.h"
#include "QSaveFile"

#define NB_COLONNES_PATIENT_LIST 6

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    PARAMETRE *getParametreWindow() const;


    PERSONEL *getConnectedUser() const;
    void setConnectedUser(PERSONEL *value);

    void updateListePatient(QList<PATIENT*>patients);





    QList<MODEADMISSION *> *getMode_admissions() const;
    void setMode_admissions(QList<MODEADMISSION *> *mode_admissions);

    QList<MODESORTIE *> *getMode_sortie() const;
    void setMode_sortie(QList<MODESORTIE *> *mode_sortie);

    QList<MODESOIN *> *getMode_soins() const;
    void setMode_soins(QList<MODESOIN *> *mode_soins);

    QList<MEDECIN *> *getMedecins() const;
    void setMedecins(QList<MEDECIN *> *medecins);

    QList<VILLE *> *getVilles() const;
    void setVilles(QList<VILLE *> *villes);

    QList<QUARTIER *> *getQuartier() const;
    void setQuartier(QList<QUARTIER *> *quartier);

    QList<NATIONALITE *> *getNationalites() const;
    void setNationalites(QList<NATIONALITE *> *nationalites);

    QList<ADRESSE *> *getAdresses() const;
    void setAdresses(QList<ADRESSE *> *adresses);

    QList<STATUT *> *getStatuts() const;
    void setStatuts(QList<STATUT *> *statuts);

    QList<HOPITAL *> *getHopitals() const;
    void setHopitals(QList<HOPITAL *> *hopitals);

    QList<PATIENT *> *getPatients() const;
    void setPatients(QList<PATIENT *> *patients);

    class FICHE *getNewPatient() const;
    void setNewPatient(class FICHE *value);

    EXAMEN *getNewExamen() const;
    void setNewExamen(EXAMEN *value);

    bool getAddMode() const;
    void setAddMode(bool value);

    class FICHE *getCurrentPatient() const;
    void setCurrentPatient(class FICHE *currentPatient);

public slots:
    void menuAction(QAction *act);
    void showDatabaseError(QString err);
    void changeIndicatorConnection(bool state);

signals:
    void quitter();
    void connecter(QString un,QString pass);
    void fatalError();






private slots:
    void on_connexionbtn_clicked();
    void verifyChamp(QString champValue);
    void connectionResult(bool isConnected,int idUser);//connexion result handler
    void idicatorChageState(bool state);
    void clignoter(QWidget &wid);
    void enableShowError();
    void clignoterIndicator();
    void fatalErrorAction(QString error);
    void instantiateDB();//instatiation de la base de donnée avec le fichier de configuration"


    //récupération des infos de saisis dans les classe
    void addExamToFiche(EXAMEN *exam);

    void recept(QString id,QString id_mere,QString descri,QString value);
    void remove(QString id, QString id_mere,QString desc);



    void on_newPatientBtn_clicked();

    void on_ConfirmationPatientBtn_clicked(QAbstractButton *button);

    void on_listePatientView_clicked(const QModelIndex &index);

    void updateInfoMedicaleViewOnAdd(class FICHE *f);

    void configureAddPatientView(class FICHE *f);

    void on_ville_currentIndexChanged(int index);

    void on_quartier_currentIndexChanged(int index);

    void on_quartier_currentTextChanged(const QString &arg1);

    void on_ville_currentTextChanged(const QString &arg1);

    void on_hopitalCombo_currentIndexChanged(int index);

    void on_nom_editingFinished();

    void on_prenom_editingFinished();



    void on_age_editingFinished();

    void on_sexe_2_currentIndexChanged(int index);

    void on_nationalite_currentTextChanged(const QString &arg1);
     void on_nationalite_currentIndexChanged(int index);


    void on_proffession_currentTextChanged(const QString &arg1);

    void on_contact_editingFinished();


    void on_lot_editingFinished();

     void on_dateAdmission_dateTimeChanged(const QDateTime &dateTime);
    void on_modeAdmission_currentIndexChanged(int index);

    void on_modeSoin_currentIndexChanged(int index);


    void on_medecinResponsable_currentIndexChanged(int index);

    void on_service_currentTextChanged(const QString &arg1);

    void on_service_currentIndexChanged(int index);

    void on_addExamenBtn_clicked();

    void on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_responsablePrincipalCheck_toggled(bool checked);

    void on_examinateur_currentIndexChanged(int index);

    void on_statut_currentIndexChanged(int index);


    void addExamFor(class FICHE *f);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_filtre_currentIndexChanged(int index);



    void on_pushButton_2_clicked(bool checked);


    void on_hopitalCombo_currentTextChanged(const QString &arg1);



    void on_endStat_clicked();

    void startStat();
    void finishStat();

protected:
    NATIONALITE *isNationaliteExiste(NATIONALITE *nat);
    VILLE *isVilleExist(VILLE *v);
    QUARTIER *isQuartierExistInVille(VILLE *v,QUARTIER *q);
    ADRESSE *isSameAdresse(ADRESSE *);

    PATIENT *isPatientLastHospitalised(PATIENT *pat);

    bool rewritePatient(class FICHE *f);
    void closeAdd();

    void changeBtnState(bool state);


    void setToolbar();
     void start();
     QString generateId();
     void showParametre(int index=0);
     void setButtonState(bool state);


     void updateView(QList< class FICHE *> newFiche);
     void updateCompleterLot(QUARTIER *quartier);
     void updateCompleterQuartier(VILLE *ville);
     void updateCompleterService(HOPITAL *hopital);


     void deleteInfoMedicaleViewOnAdd();
     void instantiateAllDataForAdd();
     void deleteAllDataForAdd();

     bool isSavable();


     bool exportToPdf(QWidget *w,QString dir);
     void paintEvent(QPaintEvent *);



private:
     bool verifId();
//     bool verifDB();
     bool verifFiche();


    Ui::MainWindow *ui;
    DBManager *dbm;
    PARAMETRE *parametreWindow;
    HXmlManager *xmlManager;
    QtAwesome *awesome;
    QTimer *timerIndicator;
    bool firstLauch;
    PERSONEL *connectedUser;
    bool first=false;
    SERVICE_UI *s;
    bool canShowError;
    bool allLoaded;//variable pour vérifier que les vues sont tous chargés

    QList< class FICHE *> current;


    //données d'ajout, tout ces donnée devrons être initialisée à chaque ouverture du fenêtre de l'ajout
    class FICHE *newPatient;
    EXAMEN *newExamen;

    QList<MODEADMISSION *> *m_mode_admissions;
    QList<MODESORTIE *> *m_mode_sortie;
    QList<MODESOIN *> *m_mode_soins;
    QList<MEDECIN *> *m_medecins;
    QList<VILLE *> *m_villes;
    QList<QUARTIER *> *m_quartier;
    QList<NATIONALITE *> *m_nationalites;
    QList<ADRESSE *> *m_adresses;
    QList<STATUT *> *m_statuts;
    QList<HOPITAL *> *m_hopitals;
    QList<PATIENT *> *m_patients;

    class FICHE *m_currentPatient;
    QGroupBox *container;

    bool isExamEdited;
    bool onModif=false;
    bool addMode;

    QSortFilterProxyModel *sortFilter;
    QPdfWriter *writer;
    TRANSFERER *transfertView;
    HCHARTVIEW *cv;


    QSaveFile saver;

};


#endif // MAINWINDOW_H
