#ifndef PARAMETRE_H
#define PARAMETRE_H

#include <QObject>
#include <QtWidgets>
#include "hxmlmanager.h"
#include "HObject/HWidget/hline.h"

namespace Ui
{
class PARAMETRE;
}

class PARAMETRE : public QDialog
{
    Q_OBJECT

public:
    explicit PARAMETRE(QWidget *parent = nullptr, int index=0);
    ~PARAMETRE();

    QString getHost() const;
    void setHost(const QString &value);

    QString getDbname() const;
    void setDbname(const QString &value);

    QString getPass() const;
    void setPass(const QString &value);

    QString getPort() const;
    void setPort(const QString &value);

    QString getUsername() const;
    void setUsername(const QString &value);

    void saveDBParams();


    bool isValid() const;


    void instantiate();
    bool getViewFicheSet() const;
    void setViewFicheSet(bool value);

public slots:
    void actionClick(QAbstractButton *clickedButton);
    void exec(bool first=false);



private slots:


    void verifChamp(QString text);

    bool getEgal() const;
    void setEgal(bool value);

    void on_DBButton_clicked();

    void on_ModeleBtn_clicked();


    void hideMessage();

    void showMessage(QString msg);

    void setViewfiche();


signals:
    void applied();

protected:
    void setDatabaseParams();
    void save();//inci se trouve tout les sauvegarde nécessaire, que ça soit sur l'objet ou sur le fichier de sauvegarde
    void setValidity(bool value);


private:
    Ui::PARAMETRE *ui;
    HXmlManager *xmlManager;
    QString host,dbname,pass,port,username;
    bool validity;
    bool egal;
    bool m_first;
    bool applique;
    bool viewFicheSet;
};


#endif // PARAMETRE_H
