#ifndef HCHARTVIEW_H
#define HCHARTVIEW_H

#include <QChart>

#include <QWidget>
#include "DBManager.h"
#include "hxmlmanager.h"


namespace Ui
{
class HCHARTVIEW;
}

class HCHARTVIEW : public QWidget
{
    Q_OBJECT

public:
    explicit HCHARTVIEW(QWidget *parent = nullptr);
    ~HCHARTVIEW();

    QChartView *getView() const;

    void fomerReq();
    void interpretParam();//interprétation des critères
    void refresh();
    void rechoisir();
    void launch();

    void appendQueryCombo();
    void deleteLastQueryCombo();


    QChart *getChart() const;
    void setChart(QChart *value);

    int getNbCritere() const;
    void setNbCritere(int value);

    QString getCriterePrincipale() const;

    bool getPrincipaleEstChoisi() const;
    void setPrincipaleEstChoisi(bool value);

    QString getReqString() const;
    void setReqString(const QString &value);

    QList<QPieSlice *> getResults() const;
    void setResults(const QList<QPieSlice *> &value);

    QPieSeries *getSeries() const;
    void setSeries(QPieSeries *value);

    void eventCheck(bool buttonstate, QCheckBox *box);

    QString getCurrentReq() const;
    void setCurrentReq(const QString &value);

    QList<QComboBox *> getListQueryCombo() const;
    void setListQueryCombo(const QList<QComboBox *> &value);

    QComboBox *getCurrentQueryCombo() const;
    void setCurrentQueryCombo(QComboBox *value);

    QList<CLASSE *> getCurrentNiveau() const;
    void setCurrentNiveau(const QList<CLASSE *> &value);

    QList<CLASSE *> getNiveauBase() const;
    void setNiveauBase(const QList<CLASSE *> &value);

private slots:

    void showError(QString text);

    void on_sexeF_toggled(bool checked);

    void on_villeF_toggled(bool checked);

    void on_dateAdmissionF_toggled(bool checked);

    void on_ageF_toggled(bool checked);

    void on_modeSoinF_toggled(bool checked);

    void on_intervalleAgeF_toggled(bool checked);

    void on_nationalieF_toggled(bool checked);

    void on_professionF_toggled(bool checked);

    void on_modeSortieF_toggled(bool checked);


    void on_informationMedicalF_toggled(bool checked);

    void on_nationaliteF_toggled(bool checked);

    void on_IntervalleAdmissionF_toggled(bool checked);

    void on_rechoisir_clicked();

    void on_refresh_clicked();

    void on_pushButton_clicked();


    void on_villeQuartierF_toggled(bool checked);

    void on_lauch_clicked();

    void on_sexe_currentIndexChanged(int index);

    void on_age_valueChanged(int arg1);

    void on_age2_valueChanged(const QString &arg1);

    void on_ville_currentIndexChanged(int index);


    void on_quartier_currentIndexChanged(int index);

    void on_nationalite_currentIndexChanged(int index);

    void on_dateAdmission_dateChanged(const QDate &date);

    void on_dateAdmission2_dateChanged(const QDate &date);

    void on_modeSoin_currentIndexChanged(int index);

    void on_modeSortie_currentIndexChanged(int index);

//    void on_queryBase_currentIndexChanged(int index);

    void on_values_currentTextChanged(const QString &arg1);

    void on_values_currentIndexChanged(const QString &arg1);

    void on_addQuery_clicked();

    void on_age_valueChanged(const QString &arg1);

    void on_endPram_clicked();

    void currentQueryIndexChanged(int index);

    void on_delQuery_clicked();


private:
    Ui::HCHARTVIEW *ui;
    QChartView *view;
    QChart *chart;
    QPieSeries *series;

    int nbCritere;
    QString criterePrincipale;
    bool principaleEstChoisi;
    QString reqString;
    QString currentReq;
    QStringList history;
    QList<QPieSlice *> results;


    HXmlManager *xmlManager;
    DBManager *dbm;


    //
    QList<MODESOIN *> soins;
    QList<MODESORTIE *> sorties;
    QList<VILLE *> villes;
    QList<QUARTIER *> quartiers;
    QList<NATIONALITE *> natio;
    QStringList prof;
    QStringList idMeres;
    QList<CLASSE *> currentNiveau;
    QList<CLASSE *> niveauBase;
    QStringList values;

    QList<QComboBox *> listQueryCombo;
    QComboBox *currentQueryCombo;



    //valeurs filtres

    QString sexe, age1,age2,ville,quatier,natinalite,profession,dateAdmission1,dateAdmission2,modeSoin,modeSortie,classe,valeur;

};

#endif // HCHARTVIEW_H
