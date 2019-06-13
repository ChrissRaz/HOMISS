#include "hline.h"
#include "ui_hline.h"

//HLine::HLine(QWidget *parent) : QWidget(parent),ui(new Ui::HLine)
//{
//    ui->setupUi(this);

//    this->setId("");
//    this->setDescriptif("");
//    this->setAlias("");
//    this->setUsage(false);
//    this->setValues(QStringList());
//    this->setIdSubMenus(QStringList());
//    this->ui->groupBox->setTitle("");
//    this->setTypeRemplissage("0");

//    instantiate();
//}

HLine::HLine(QString id, QString desc, QStringList values, QString alias, QString typeRempl, bool usage, QStringList idSub, bool parametrable, bool inverted, QWidget *parent): QWidget(parent),ui(new Ui::HLine)
{
    ui->setupUi(this);
    this->setId(id);
    xmlManager= new HXmlManager(this);
    onModif=false;

    //pramamétra ges affichage par défaut

    this->ui->groupBox->setTitle(desc);


    //privation de modification
    ui->remplissage->setEnabled(parametrable);
    ui->addValueOpenBtn->setEnabled(parametrable);

    ui->descriptif->setText(desc);

//    qDebug() << inverted;



    if (values.length()==2)
    {
        ui->inverted->setVisible(true);
        ui->def->setVisible(true);
    }
    else
    {

        ui->inverted->setVisible(false);
        ui->def->setVisible(false);
    }



    if (!alias.isEmpty())
    {
        setAlias(alias);
    }

    ui->listValues->setModel(new QStringListModel(values));

    int j=0;
    while (j < values.length())
    {
        if (isDefault(values.at(j)))
        {
            ui->def->setText(values.at(j));
            break;
        }
        j++;
    }

    ui->inverted->setChecked(inverted);
    if (!inverted)
    {
        ui->inverted->setChecked(inverted);
    }

    ui->remplissage->setCurrentIndex(typeRempl.toInt());//ne jamais déplace avant l'instantiation de xmlManager
    ui->groupBox->setChecked(usage);

    connect(ui->groupBox,&QGroupBox::toggled,this,&HLine::setUsage);

    this->setAddValueVisibility(false);
    this->setSubmenuVisibility(false);
    this->on_remplissage_currentIndexChanged(typeRempl.toInt());



    int i=0;

    HLine *ne;

    while(i < idSub.length())//ajout des sous menus
    {
        QDomElement sub= xmlManager->getMenu(idSub.at(i),xmlManager->findElement(xmlManager->getConfigurationDom()->documentElement(),"model"));
        if (!sub.isNull())
        {
            QStringList values;
            QStringList ids;

            QList<QDomElement > valE= xmlManager->getValuesOfMenu(sub.attribute("id"));
            QList<QDomElement > idsE= xmlManager->getMenuChilds(sub.attribute("id"));

            int k=0;

            while (k < valE.length())//récupération des id des values
            {
                values.append(valE.at(k).attribute("content"));
                k++;
            }

            k=0;

            while (k < idsE.length())// récupération des id des sous menu
            {
                ids.append(idsE.at(k).attribute("id"));
                k++;
            }

            //Détermination du remplissage

            if (sub.attribute("remplissage")!="0" && sub.attribute("remplissage")!="1" && sub.attribute("remplissage")!="2")
            {
                qWarning("remplissage invalide dans l'instantiation des lignes super dan PARAMETRE::instantiate");
            }

            bool usage=false;

            if (sub.attribute("usage")=="true")
            {
                usage=true;
            }
            else if(sub.attribute("usage")=="false")
            {
                usage=false;
            }
            else
            {
                qWarning("usage invalide dans l'instantiation des lignes super dan PARAMETRE::instantiate");
            }

            ne= new HLine(sub.attribute("id"),sub.attribute("descriptif"),values,sub.attribute("alias"),sub.attribute("remplissage"),usage,ids,sub.attribute("parametrable")=="true"? true:false,sub.attribute("valueInverted")=="true"? true:false,this);
            ui->subMenuLayout->addWidget(ne);
        }
        i++;
    }


}

HLine::~HLine()
{
    delete ui;
    delete xmlManager;
//    delete add;
}

//void HLine::update()
//{
//    xmlManager->updateMenu(this->id(),this->descriptif(),this->typeRemplissage(),this->usage(),this->alias(),QString::number(this->values().length()));
//}



//QStringList HLine::values() const
//{
//    return values_;
//}

//void HLine::setValues(const QStringList &values)
//{
//    values_ = values;
//}

//bool HLine::usage() const
//{
//    return usage_;
//}

void HLine::setUsage(bool usage)
{
    if (!usage)
    {

        if (addValueVisibility())
        {
            setAddValueVisibility(false);
        }
        if (submenuVisibility())
        {
            setSubmenuVisibility(false);
        }
    }

    ui->container->setEnabled(usage);
    updateMenu();
}

void HLine::setComplementState(bool state)
{
    ui->complement->setEnabled(state);

    if(addValueVisibility())//on emplie si disabled
    {
        setAddValueVisibility(false);
    }
}

void HLine::setAddValueVisibility(bool state)
{
    if (!state)
        this->setOnModif(false);
    ui->addValueGB->setVisible(state);
    ui->updateCurrentValueBtn->setEnabled(false);
    ui->deleteValueBtn->setEnabled(false);
}

void HLine::setSubmenuVisibility(bool state)
{
    ui->subMenuGb->setVisible(state);
    ui->addSubMenuBtn->setVisible(state);
}

bool HLine::complementState()
{
    return ui->complement->isEnabled();
}

bool HLine::addValueVisibility()
{
    return  ui->addValueGB->isVisible();
}

bool HLine::submenuVisibility()
{
    return  ui->subMenuGb->isVisible();
}

QString HLine::id() const
{
    return id_;
}

void HLine::setId(const QString &id)
{
    id_ = id;
}

QString HLine::alias() const
{
    QString res= ui->descriptif->text().section("(",1,1);
    res= res.section(")",0,0);
    return res;
}

void HLine::setAlias(const QString &alias)
{
    if (!alias.isEmpty())
    {
        QString res= ui->descriptif->text().section("(",1,1)+"("+alias+")";
        ui->descriptif->setText(descriptif()+" ("+alias+")");
    }
    else
    {
        ui->descriptif->setText(descriptif());
    }


}

QString HLine::descriptif() const
{
    return ui->groupBox->title();
}

//void HLine::setDescriptif(const QString &descriptif)
//{
//    descriptif_ = descriptif;
//}

void HLine::on_remplissage_currentIndexChanged(int index)
{
    updateMenu();

    if (index==2||index==3||index==4||index==5)
    {
        if (addValueVisibility())
        {
            setAddValueVisibility(false);
        }

        this->setComplementState(false);

    }
    else
    {
        this->setComplementState(true);

    }
}

void HLine::on_descriptif_clicked()//à changer après
{
    if (addValueVisibility())
    {
        setAddValueVisibility(false);
    }

    if (submenuVisibility())
    {
        setSubmenuVisibility(false);
        ui->arrow->setIcon(QIcon(QPixmap(":/images/up")));
        emit arroWStateChanged(false);
    }
    else
    {
        setSubmenuVisibility(true);
        ui->arrow->setIcon(QIcon(QPixmap(":/images/down")));
        emit arroWStateChanged(true);
    }
}

void HLine::on_arrow_clicked()
{
    if (addValueVisibility())
    {
        setAddValueVisibility(false);
    }

    if (submenuVisibility())
    {
        setSubmenuVisibility(false);
        ui->arrow->setIcon(QIcon(QPixmap(":/images/up")));
        emit arroWStateChanged(false);
    }
    else
    {
        setSubmenuVisibility(true);
        ui->arrow->setIcon(QIcon(QPixmap(":/images/down")));
        emit arroWStateChanged(true);
    }
}

void HLine::on_addValueOpenBtn_clicked()
{
    if (addValueVisibility())
    {
        setAddValueVisibility(false);
    }
    else
    {
        setAddValueVisibility(true);
    }
}

//QString HLine::typeRemplissage() const
//{
//    return typeRemplissage_;
//}

//void HLine::setTypeRemplissage(const QString &typeRemplissage)
//{
//    typeRemplissage_ = typeRemplissage;
//}

void HLine::on_newValue_textChanged(const QString &arg1)
{
    if (arg1=="")
    {
        ui->addV->setEnabled(false);
    }
    else
    {
        ui->addV->setEnabled(true);
    }
}

void HLine::on_addV_clicked()
{
    ui->addV->setEnabled(false);
    QStringList val;
    int k=0;

    while (k < ui->listValues->model()->rowCount())
    {
        val.append(ui->listValues->model()->index(k,0).data().toString());
        k++;
    }
    ui->updateCurrentValueBtn->setEnabled(false);
    ui->deleteValueBtn->setEnabled(false);


    if (this->getOnModif())
    {
        val.replace(ui->listValues->currentIndex().row(),ui->newValue->text());
        xmlManager->updateValue(id(),ui->listValues->currentIndex().data().toString(), ui->newValue->text(),true,isDefault(ui->listValues->currentIndex().data().toString()));
        this->setOnModif(false);
    }
    else
    {
        if (!val.contains(ui->newValue->text()))
        {
            QDomElement v= xmlManager->updateValue(this->id(),ui->newValue->text(),ui->newValue->text(),true,false);
            if(v.isNull())
            {
                xmlManager->addValue(this->id(),ui->newValue->text());
                val.append(ui->newValue->text());
            }
           else
            {
                val.append(v.attribute("content"));
            }

            if(val.length()==2)
            {
                ui->def->clear();
                ui->inverted->setChecked(false);
                ui->def->setVisible(true);
                ui->inverted->setVisible(true);
            }
            else
            {
                ui->def->clear();
                ui->inverted->setChecked(false);
                ui->def->setVisible(false);
                ui->inverted->setVisible(false);
            }
        }

    }


    ui->listValues->setModel(new QStringListModel(val));
    updateMenu();
    ui->newValue->clear();
}

void HLine::on_finishBtn_clicked()
{
    setAddValueVisibility(false);
}

void HLine::on_listValues_clicked(const QModelIndex &index)
{
    ui->updateCurrentValueBtn->setEnabled(true);
    ui->deleteValueBtn->setEnabled(true);
//    if (onModif)
//    {
//        if (currentRow!=index.row())
//        {
//            setOnModif(false);
//            ui->newValue->clear();
//        }
//    }
}

bool HLine::getOnModif() const
{
    return onModif;
}

void HLine::setOnModif(bool value)
{
    onModif = value;
}

void HLine::on_updateCurrentValueBtn_clicked()
{
    this->setOnModif(true);
    ui->newValue->setFocus();
    ui->newValue->setText(ui->listValues->currentIndex().data().toString());
}

void HLine::on_deleteValueBtn_clicked()
{
    QStringList l;
    int k=0;

    while (k < ui->listValues->model()->rowCount())
    {
        l.append(ui->listValues->model()->index(k,0).data().toString());
        k++;
    }


    l.removeAt(ui->listValues->currentIndex().row());

    if (isDefault(ui->listValues->currentIndex().data().toString()))
    {
        ui->def->setText("");
    }
    else
    {
        if(l.length()==1)
        {
             xmlManager->updateValue(this->id(),l.at(0),l.at(0),true,false);
        }
    }

    if(l.length()==2)
    {
        ui->inverted->setVisible(true);
        ui->def->setVisible(true);
    }
    else
    {
        if (ui->inverted->isChecked())
        {
            ui->inverted->setChecked(false);
        }
        ui->def->setText("");
        ui->inverted->setVisible(false);
        ui->def->setVisible(false);
    }

    xmlManager->updateValue(this->id(),ui->listValues->currentIndex().data().toString(),ui->listValues->currentIndex().data().toString(),false,false);
    ui->listValues->setModel(new QStringListModel (l));
    ui->updateCurrentValueBtn->setEnabled(false);
    ui->deleteValueBtn->setEnabled(false);

    updateMenu();
}

void HLine::on_groupBox_toggled(bool arg1)
{
    updateMenu();
}

void HLine::on_addAliasBtn_clicked()
{
    QString al= QInputDialog::getText(this,"Alias","Alias ",QLineEdit::Normal,this->alias());
    this->setAlias(al);
    updateMenu();
}

void HLine::on_addSubMenuBtn_clicked()
{

    ADDCLASSDIALOG *add= new ADDCLASSDIALOG(this->id(),this->descriptif(),this);
    connect(add,&ADDCLASSDIALOG::classAdded,this,&HLine::machNewClasse);
    add->exec();
}

void HLine::updateMenu()
{
    xmlManager->updateMenu(id(),descriptif(),QString::number(ui->remplissage->currentIndex()),ui->groupBox->isChecked(),alias(),QString::number(ui->listValues->model()->rowCount()),ui->inverted->isChecked());
}

bool HLine::isDefault(QString value)
{
    return xmlManager->getValueOfMenu(id(),value).attribute("default")=="true";
}

void HLine::on_inverted_toggled(bool checked)
{
    if (checked)
    {
         ui->def->setText(ui->listValues->indexAt(QPoint(0,0)).data(0).toString());
         xmlManager->updateValue(this->id(),ui->listValues->indexAt(QPoint(0,0)).data(0).toString(),ui->listValues->indexAt(QPoint(0,0)).data(0).toString(),true,true);
    }
    else
    {
        ui->def->setText("");
        xmlManager->updateValue(this->id(),ui->listValues->indexAt(QPoint(0,0)).data(0).toString(),ui->listValues->indexAt(QPoint(0,0)).data(0).toString(),true,false);
    }

    if (this->getOnModif())
    {
        this->setOnModif(false);
        ui->newValue->clear();
    }
    updateMenu();
}

void HLine::machNewClasse(int pos, QDomElement ne)
{
    QStringList vals;
    vals.append("oui");
    vals.append("non");

    ui->subMenuLayout->insertWidget(pos,new HLine(ne.attribute("id"),ne.attribute("descriptif"),vals,ne.attribute("alias"),ne.attribute("remplissage"),true,QStringList(),ne.attribute("parametrable")=="true"? true:false,ne.attribute("valueInverted")=="true"? true:false,this));
}
