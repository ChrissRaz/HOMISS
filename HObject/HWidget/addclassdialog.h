#ifndef ADDCLASSDIALOG_H
#define ADDCLASSDIALOG_H

#include <QDialog>
#include "DBManager.h"

namespace Ui {
class ADDCLASSDIALOG;
}

class ADDCLASSDIALOG : public QDialog
{
    Q_OBJECT

public:
//    explicit ADDCLASSDIALOG(QWidget *parent = nullptr);
    ADDCLASSDIALOG(QString id_parent, QString parentDescription,QWidget *parent=nullptr);
    ~ADDCLASSDIALOG();

    void save();

    QString idparent() const;
    void setIdparent(const QString &idparent);

    bool isValid() const;
    void setIsValid(bool isValid);

    QStringList id_others() const;
    void setId_others(const QStringList &id_others);

    void instantiate();




    QDomNodeList getChilds() const;
    void setChilds(const QDomNodeList &value);

signals:
    void classAdded(int pos, QDomElement newEl);

private slots:
    void on_newDesc_textChanged(const QString &arg1);
    void on_newDesc_editingFinished();

    void on_confirmationBtn_accepted();

    void on_confirmationBtn_rejected();




    void on_first_toggled(bool checked);


private:
    Ui::ADDCLASSDIALOG *ui;
    QString m_idparent;
    QSqlDatabase *db;
    HXmlManager *xmlManager;
    bool m_isValid;
    DBManager *dbm;
    QStringList m_id_others;
    QDomNodeList childs;
};

#endif // ADDCLASSDIALOG_H
