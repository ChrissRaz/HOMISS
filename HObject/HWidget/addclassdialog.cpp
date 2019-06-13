#include "addclassdialog.h"
#include "ui_addclassdialog.h"

//ADDCLASSDIALOG::ADDCLASSDIALOG(QWidget *parent) : QDialog(parent),ui(new Ui::ADDCLASSDIALOG)
//{
//    ui->setupUi(this);
//    this->setAttribute(Qt::WA_DeleteOnClose);
//    ui->setupUi(this);
//    this->setIdparent("");
//    this->setId_others(QStringList());

//    this->instantiate();
//}

ADDCLASSDIALOG::ADDCLASSDIALOG(QString id_parent, QString parentDescription, QWidget *parent):QDialog(parent),ui(new Ui::ADDCLASSDIALOG)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setIdparent(id_parent);
    ui->title->setText("Ajouter une nouvelle sous classe de "+parentDescription);
    this->instantiate();
}

ADDCLASSDIALOG::~ADDCLASSDIALOG()
{
    delete ui;
}

void ADDCLASSDIALOG::save()
{
    QString id;

    if (getChilds().at(ui->othersMenu->currentIndex()).isNull())
    {
        QMessageBox::warning(nullptr,"error","ref null");
    }

    id= dbm->addClass(ui->newDesc->text().simplified(),idparent());
    if (id.isEmpty())
    {
        QMessageBox::warning(this,"Erreur","Une erreur s'es produit, veillez réessayer");
        return;
    }

    QDomElement neu;

   if (getChilds().isEmpty())
   {
       neu=xmlManager->addMenu(id,ui->newDesc->text().simplified(),"1",QList<QDomElement>(),idparent(),ui->newAlias->text().simplified(),true,true,true);
   }
   else
   {
       if (ui->first->isChecked())
       {
          neu= xmlManager->addMenu(id,ui->newDesc->text().simplified(),"1",QList<QDomElement>(),idparent(),ui->newAlias->text().simplified(),true,true,true,getChilds().at(ui->othersMenu->currentIndex()),false);

       }
       else
       {
           neu=xmlManager->addMenu(id,ui->newDesc->text().simplified(),"1",QList<QDomElement>(),idparent(),ui->newAlias->text().simplified(),true,true,true,getChilds().at(ui->othersMenu->currentIndex()));

       }
   }
   xmlManager->addValue(id,"oui",true,true);
   xmlManager->addValue(id,"non",true,false);

   if (!neu.isNull())
   {
       emit classAdded(ui->othersMenu->currentIndex(),neu);
       this->close();
   }
   else
   {
       QMessageBox::warning(this,"Erreur","Une erreur s'es produit, veillez réessayer");
       return;
   }
}

QString ADDCLASSDIALOG::idparent() const
{
    return m_idparent;
}

void ADDCLASSDIALOG::setIdparent(const QString &idparent)
{
    m_idparent = idparent;
}

bool ADDCLASSDIALOG::isValid() const
{
    return m_isValid;
}

void ADDCLASSDIALOG::setIsValid(bool isValid)
{
    m_isValid = isValid;
}

QStringList ADDCLASSDIALOG::id_others() const
{
    return m_id_others;
}

void ADDCLASSDIALOG::setId_others(const QStringList &id_others)
{
    m_id_others = id_others;
}

void ADDCLASSDIALOG::instantiate()
{
    this->setIsValid(false);
    xmlManager= new HXmlManager(this);
    db= new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL","ADDCLASSDB"));
    db->setHostName(xmlManager->getDatabaseParameter(DBInfoType::HOSTNAME));
    db->setUserName(xmlManager->getDatabaseParameter(DBInfoType::USERNAME));
    db->setPassword(xmlManager->getDatabaseParameter(DBInfoType::PASSWORD));
    db->setDatabaseName(xmlManager->getDatabaseParameter(DBInfoType::DBNAME));
    db->setPort(xmlManager->getDatabaseParameter(DBInfoType::PORT).toInt());

    if (!db->open())
    {
        QMessageBox::warning(this,"Non connecté","Impossible d'acceder à la base de donnée.\nVeuillez réessayer!)");
        this->close();
    }

    dbm= new DBManager(*db,this);
    dbm->seConnecter();

// qDebug( ) << dbm->isClassExist("","MOTIF DE CONSULTATION")+"hahahaha";

    QList<QDomElement > el= xmlManager->getMenuChilds(this->idparent());

//     QList<QDomElement > el;

    QDomElement me= xmlManager->getMenu(idparent(),xmlManager->findElement(xmlManager->getConfigurationDom()->documentElement(),"model"));


    setChilds(me.childNodes());

//    int i=0;

//    while (i<getChilds().length())
//    {
//        el.append(getChilds().at(i).toElement());
//        i++;
//    }

//    QDomNode no= me.firstChild();
//    QDomElement current;
//    while(!no.isNull())
//    {
//        if (no.isElement() && no.isText())
//        {
//            childs.append(no);
//            current= no.toElement();
//            el.append(current);
//        }

//        no=no.nextSibling();
//    }

    int i=0;
    QStringList o;
    bool nu=true;
    while(i<el.length())
    {
        if (nu) nu=false;
        ui->othersMenu->addItem(el.at(i).attribute("descriptif"));
        o.append(el.at(i).attribute("id_supra"));
        i++;
    }

    if (nu)
    {
        ui->first->setChecked(true);
        ui->first->setEnabled(false);
    }
    else
    {
        ui->othersMenu->setCurrentIndex(0);//décleucheur pour la valeur par défaut de ref
    }

}

void ADDCLASSDIALOG::on_newDesc_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
    {
        ui->confirmationBtn->setEnabled(false);
    }
    else
    {
        ui->confirmationBtn->setEnabled(true);
        if (ui->othersMenu->findText(arg1.simplified(),static_cast<Qt::MatchFlag>(Qt::CaseInsensitive | Qt::MatchExactly))!=-1)
        {
            ui->newDesc->setStyleSheet("color: red;");
            ui->confirmationBtn->setEnabled(false);
        }
        else
        {
            ui->confirmationBtn->setEnabled(true);
            ui->newDesc->setStyleSheet("color: black;");
        }
    }
}

void ADDCLASSDIALOG::on_newDesc_editingFinished()
{
    if (ui->newDesc->text().isEmpty())
    {
        ui->confirmationBtn->setEnabled(false);
    }
    else
    {
        ui->confirmationBtn->setEnabled(true);
        if (ui->othersMenu->findText(ui->newDesc->text().simplified(),static_cast<Qt::MatchFlag>(Qt::CaseInsensitive | Qt::MatchExactly))!=-1)
        {
            ui->newDesc->setStyleSheet("color: red;");
            ui->confirmationBtn->setEnabled(false);
        }
        else
        {
            ui->confirmationBtn->setEnabled(true);
            ui->newDesc->setStyleSheet("color: black;");
        }
    }
}


void ADDCLASSDIALOG::on_confirmationBtn_accepted()
{
    this->save();
    this->close();
}

void ADDCLASSDIALOG::on_confirmationBtn_rejected()
{
    if (ui->newDesc->text().isEmpty())
    {
        this->close();
    }
    else
    {
        int res= QMessageBox::question(this,"Abandonner","Voulez vous ababdonner l'ajout?");

        if (res==QMessageBox::Ok)
        {
            this->close();
        }
    }
}



void ADDCLASSDIALOG::on_first_toggled(bool checked)
{
    if (checked)
    {
        ui->othersMenu->setEnabled(false);
        ui->othersMenu->setCurrentIndex(0);

    }
    else
    {
        ui->othersMenu->setEnabled(true);
    }

}

QDomNodeList ADDCLASSDIALOG::getChilds() const
{
    return childs;
}

void ADDCLASSDIALOG::setChilds(const QDomNodeList &value)
{
    childs = value;
}


