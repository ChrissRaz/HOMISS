#ifndef HLINE_H
#define HLINE_H

#include <QWidget>
#include "hxmlmanager.h"
#include "addclassdialog.h"

namespace Ui
{
class HLine;
}

class HLine : public QWidget
{
    Q_OBJECT

public:
//    explicit HLine(QWidget *parent = nullptr);
    HLine(QString id, QString desc, QStringList values, QString alias, QString typeRempl, bool usage, QStringList idSub, bool parametrable, bool inverted, QWidget *parent=nullptr);
    ~HLine();
//    QStringList values() const;
//    void setValues(const QStringList &values);

    QString id() const;
    void setId(const QString &id);

    QString alias() const;
    void setAlias(const QString &alias);

    QString descriptif() const;
//    void setDescriptif(const QString &descriptif);

//    bool usage() const;




    bool hasChild();

//    QString typeRemplissage() const;
//    void setTypeRemplissage(const QString &typeRemplissage);

    bool getOnModif() const;
    void setOnModif(bool value);


public slots:
    void setUsage(bool usage);

    void setComplementState(bool state);
    void setAddValueVisibility(bool state);
    void setSubmenuVisibility(bool state);
    bool complementState();
    bool addValueVisibility();
    bool submenuVisibility();

//protected:
//    void update();

signals:
    void arroWStateChanged(bool state);
    void usageStateChanged(bool usage);

private slots:
    void on_remplissage_currentIndexChanged(int index);

    void on_descriptif_clicked();

    void on_arrow_clicked();

    void on_addValueOpenBtn_clicked();


    void on_newValue_textChanged(const QString &arg1);

    void on_addV_clicked();

    void on_finishBtn_clicked();

    void on_listValues_clicked(const QModelIndex &index);


    void on_updateCurrentValueBtn_clicked();

    void on_deleteValueBtn_clicked();

    void on_groupBox_toggled(bool arg1);

    void on_addAliasBtn_clicked();

    void on_addSubMenuBtn_clicked();

    void on_inverted_toggled(bool checked);


    void machNewClasse(int pos, QDomElement ne);
private:
    void updateMenu();
    bool isDefault(QString value);
//    int currentRow=0;
    bool onModif;

    Ui::HLine *ui;

//    QStringList values_;
    QString id_;

//    QString alias_;
//    QString descriptif_;
//    bool usage_;
//    QString typeRemplissage_;
//    QStringList idSubMenus_;
    HXmlManager *xmlManager;
//    ADDCLASSDIALOG *add;
};

#endif // HLINE_H
