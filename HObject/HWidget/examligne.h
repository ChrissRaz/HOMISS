#ifndef EXAMLIGNE_H
#define EXAMLIGNE_H

#include <QWidget>
#include "hxmlmanager.h"

namespace Ui {
class EXAMLIGNE;
}

class EXAMLIGNE : public QWidget
{
    Q_OBJECT

public:
    explicit EXAMLIGNE(QString id, QString desc, int nbVal, QStringList values, QString alias, QString typeRempl, QStringList idSub, bool parametrable, bool inverted,QString idMere=QString(), QWidget *parent = nullptr);
    ~EXAMLIGNE();

    HXmlManager *getXmlManager() const;


    QString *getDescri() const;
    void setDescri(QString *value);

    QString *getId() const;
    void setId(QString *id);

    QString *getId_mere() const;
    void setId_mere(QString *id_mere);

    void getDefault();
    void derouler();
private slots:
    void on_other_clicked();
    void radio_toggled(bool state);
    void checkBox_checked(bool state);
    void on_valueBinary_toggled(bool checked);

    void on_personalise_editingFinished();

    void cascade(QString id, QString id_mere,QString description,QString value);

    void cascadeClear(QString id, QString id_mere,QString description);

    void on_arrow_clicked();

    void on_descriptif_clicked();

    void on_timeEdit_timeChanged(const QTime &time);

    void on_dateEdit_dateChanged(const QDate &date);

    void on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime);



signals:
    void set(QString id, QString id_mere,QString description,QString value);
    void clear(QString id, QString id_mere,QString description);
//    void del(QString description,QString value);
private:
    Ui::EXAMLIGNE *ui;
    HXmlManager *xmlManager;
    QString *descri;
    QList<QRadioButton *> *radios;
    QList<QCheckBox *> *checks;
    QString *m_id;
    QString *m_id_mere;
};

#endif // EXAMLIGNE_H
