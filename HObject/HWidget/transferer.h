#ifndef TRANSFERER_H
#define TRANSFERER_H
#include "examligne.h"

#include <QWidget>

#include "DBManager.h"
#include "hxmlmanager.h"

namespace Ui {class TRANSFERER;}

class TRANSFERER : public QDialog
{
    Q_OBJECT

public:
    explicit TRANSFERER(class FICHE *f, QWidget *parent = nullptr);
    ~TRANSFERER();


    QString getId_admission() const;
    void setId_admission(const QString &value);

    QList<MODESORTIE *> getSorties() const;
    void setSorties(const QList<MODESORTIE *> &value);

    QList<HOPITAL *> getHopitals() const;
    void setHopitals(const QList<HOPITAL *> &value);

    QList<SERVICE *> getServices() const;
    void setServices(const QList<SERVICE *> &value);

    class FICHE *getF() const;
    void setF(class FICHE *f);

public slots:
    void recept(QString id, QString id_mere, QString descri, QString value);
    void remove(QString id, QString id_mere, QString desc);
private slots:
    void showDBError(QString message);
    void activate();

    void on_Local_toggled(bool checked);

    void on_hopitalDestinaire_currentIndexChanged(const QString &arg1);

    void on_hopitalDestinaire_currentTextChanged(const QString &arg1);

    void on_modeSortie_currentIndexChanged(int index);

    void on_serviceDestionaire_currentTextChanged(const QString &arg1);

    void on_serviceDestionaire_currentIndexChanged(int index);

    void on_dateSortie_dateTimeChanged(const QDateTime &dateTime);

    void on_buttonBox_clicked(QAbstractButton *button);


signals:
    void save(QString chemin);


private:

    void updateCompleterService(QList<SERVICE *> se);
    void updateCompleterHopital(QList<HOPITAL *> ho);
    Ui::TRANSFERER *ui;
    DBManager *dbm;
    HXmlManager *xmlManager;
    QString id_admission;
    QList<MODESORTIE *> sorties;
    QList<HOPITAL *> hopitals;
    QList<SERVICE *> services;
    SORTIE *sortie;
    class FICHE *m_f;
    EXAMLIGNE *parametre;
    EXAMEN *newExamen;
};

#endif // TRANSFERER_H
