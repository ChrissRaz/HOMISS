#include "examligne.h"
#include "ui_examligne.h"


EXAMLIGNE::EXAMLIGNE(QString id, QString desc, int nbVal, QStringList values, QString alias, QString typeRempl, QStringList idSub, bool modifiable, bool inverted, QString idMere, QWidget *parent):
    QWidget(parent),ui(new Ui::EXAMLIGNE)
{
    ui->setupUi(this);

    ui->personalise->setVisible(false);
    ui->valueBinary->setVisible(false);
    ui->groupBox_2->setVisible(false);
    ui->valueListe->setVisible(false);
    ui->other->setEnabled(false);
    ui->sub->setVisible(false);
    ui->timeEdit->setVisible(false);
    ui->dateEdit->setVisible(false);
    ui->dateTimeEdit->setVisible(false);


    this->setObjectName(desc);
    m_id=new QString(id);
    m_id_mere= new QString(idMere);

    this->setDescri(new QString(desc));
    radios= new QList<QRadioButton *>();
    checks= new QList<QCheckBox *>();


    xmlManager= new HXmlManager();

    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    if (!modifiable)
    {
        emit set(id,idMere, desc,xmlManager->getDefaultValueOfMenu(id));
        ui->other->setVisible(false);
    }
    else
    {
         if(typeRempl=="1")
         {
             int i=0;

             if (nbVal==2)
             {

                 if(inverted)
                 {
                     ui->valueBinary->setVisible(true);
                     ui->valueBinary->setText(xmlManager->getDefaultValueOfMenu(id));

                     QCheckBox *n=new QCheckBox(this);

                     if (ui->valueBinary->text()==values.at(0))
                     {
                         n->setText(values.at(1));
                         checks->append(n);
                     }
                     else
                     {
                         n->setText(values.at(0));
                         checks->append(n);

                     }
                     n->setVisible(false);
                 }
                 else
                 {
                     ui->valueListe->setVisible(true);
                     ui->valueListe->addItem(values.at(0));
                     ui->valueListe->addItem(values.at(1));
                 }
             }
             else if (nbVal<10)
             {
                 ui->other->setEnabled(true);
                 ui->groupBox_2->setVisible(true);
                 QRadioButton *r;

                 while (i< nbVal)
                 {
                     r= new QRadioButton(values.at(i),this);
                     if(i<5)
                     {
                         ui->choixCntainer->addWidget(r,0,i);
                     }
                     else
                     {
                          ui->choixCntainer->addWidget(r,1,i-5);
                     }

                     r->show();
                     connect(r,&QRadioButton::toggled,this,&EXAMLIGNE::radio_toggled);
                     radios->append(r);
                     i++;
                 }
             }
             else
             {
                 ui->valueListe->setVisible(true);
                 while (i<nbVal)
                 {
                     ui->valueListe->addItem(values.at(i));
                     i++;
                 }
             }

         }
         else if(typeRempl=="0")
         {
             ui->other->setEnabled(true);
             ui->groupBox_2->setVisible(true);
             int i=0;
             QCheckBox *c;
             int l=0;
             int coll=0;
             while (i< nbVal)
             {
                 c= new QCheckBox(values.at(i),this);
                 if(i%7==0)
                 {
                     l++;
                     coll=0;

                 }
                 ui->choixCntainer->addWidget(c,l,coll);
                 c->show();
                 connect(c,&QCheckBox::toggled,this,&EXAMLIGNE::checkBox_checked);
                 checks->append(c);
                 coll++;
                 i++;
             }
         }
         else if (typeRempl=="2")
         {
             ui->personalise->setVisible(true);
             ui->other->setVisible(false);
         }
         else if (typeRempl=="3")
         {
             ui->other->setVisible(false);
             ui->timeEdit->setVisible(true);
         }
         else if (typeRempl=="4")
         {
             ui->other->setVisible(false);
             ui->dateEdit->setVisible(true);
         }
         else if (typeRempl=="5")
         {
             ui->other->setVisible(false);
             ui->dateTimeEdit->setVisible(true);
         }
         else
         {
             ui->descriptif->setText("Invalide Valeur, celà pourrais lever une incohérence!");
         }

     }

     if (alias.isEmpty())
     {
         ui->descriptif->setText(desc);
     }
     else
     {
         ui->descriptif->setText(alias);
     }

     int j=0;
     EXAMLIGNE *ne;

    while (j<idSub.length())//génération des sousMenu del'instance
    {
      QDomElement el=xmlManager->getMenu(idSub.at(j),xmlManager->findElement(xmlManager->getConfigurationDom()->documentElement(),"model"));
      if (el.attribute("usage")=="true")
      {
          QList<QDomElement > vals= xmlManager->getValuesOfMenu(el.attribute("id"));
          QStringList v;
          QStringList s;
          int k=0;
          while (k<vals.length())
          {
              v.append(vals.at(k).attribute("content"));
              k++;
          }
          k=0;
          vals= xmlManager->getMenuChilds(idSub.at(j));

          while (k<vals.length())
          {
              s.append(vals.at(k).attribute("id"));
              k++;
          }

          ne=new EXAMLIGNE(el.attribute("id"),el.attribute("descriptif"),el.attribute("nbVal").toInt(),v,el.attribute("Alias"), el.attribute("remplissage"),s,el.attribute("valueModifiable")=="true"?true:false, el.attribute("valueInverted")=="true"? true:false,*getId(),this);
          connect(ne,&EXAMLIGNE::set,this,&EXAMLIGNE::cascade);
          connect(ne,&EXAMLIGNE::clear,this,&EXAMLIGNE::cascadeClear);
          ui->subLayout->addWidget(ne);

      }
       j++;
    }
}

EXAMLIGNE::~EXAMLIGNE()
{
    delete ui;
    delete xmlManager;

    delete descri;
    delete radios;
    delete checks;

    delete getId();
    delete getId_mere();
}

void EXAMLIGNE::on_other_clicked()
{
    if (ui->personalise->isVisible())
    {
        ui->personalise->setVisible(false);
    }
    else
    {
        ui->personalise->setVisible(true);
    }

}

void EXAMLIGNE::radio_toggled(bool state)
{
    emit clear(*getId(),*getId_mere(),*descri);
    if (!ui->personalise->isVisible())
    {
        int i=0;
        while(i<radios->length())
        {
            if (radios->at(i)->isChecked())
            {

                emit set(*getId(),*getId_mere(),*descri,radios->at(i)->text());
                break;

            }
            i++;
        }
    }

}

void EXAMLIGNE::checkBox_checked(bool state)
{
    emit clear(*getId(),*getId_mere(),*descri);

    int i=0;
    while(i<checks->length())
    {
        if (checks->at(i)->isChecked())
        {
            emit set(*getId(),*getId_mere(),*descri,checks->at(i)->text());

        }
        i++;
    }

}

QString *EXAMLIGNE::getDescri() const
{
    return descri;
}

void EXAMLIGNE::setDescri(QString *value)
{
    descri = value;
}

HXmlManager *EXAMLIGNE::getXmlManager() const
{
    return xmlManager;
}

void EXAMLIGNE::on_valueBinary_toggled(bool checked)
{
    emit clear(*getId(),*getId_mere(),*descri);
    if (!ui->personalise->isVisible())
    {
        if (checked)
        {
            emit set(*getId(),*getId_mere(),ui->descriptif->text(),ui->valueBinary->text());

        }
        else
        {
            emit set(*getId(),*getId_mere(),ui->descriptif->text(),checks->at(0)->text());
        }
    }
}

void EXAMLIGNE::on_personalise_editingFinished()
{
    if (!radios->isEmpty())
    {
        emit clear(*getId(),*getId_mere(),*descri);
    }

    if (!ui->personalise->text().isEmpty())
    {
        emit set(*getId(),*getId_mere(), ui->descriptif->text(),ui->personalise->text());
    }

}

void EXAMLIGNE::cascade(QString id, QString id_mere, QString description, QString value)
{
    emit set (id,id_mere,description,value);
}

void EXAMLIGNE::cascadeClear(QString id, QString id_mere, QString description)
{
    emit clear(id,id_mere,description);
}

void EXAMLIGNE::on_arrow_clicked()
{
    ui->sub->setVisible(!ui->sub->isVisible());
    if (ui->sub->isVisible())
    {
        ui->arrow->setIcon(QIcon(QPixmap(":/images/down")));
    }
    else
    {
        ui->arrow->setIcon(QIcon(QPixmap(":/images/up")));
    }

}

void EXAMLIGNE::on_descriptif_clicked()
{
    ui->sub->setVisible(!ui->sub->isVisible());
    if (ui->sub->isVisible())
    {
        ui->arrow->setIcon(QIcon(QPixmap(":/images/down")));
    }
    else
    {
        ui->arrow->setIcon(QIcon(QPixmap(":/images/up")));
    }
}

void EXAMLIGNE::on_timeEdit_timeChanged(const QTime &time)
{
    if (ui->timeEdit->isVisible())
    {
        emit clear(*getId(),*getId_mere(),*descri);
        emit set(*getId(),*getId_mere(),*descri,time.toString("hh:mm:ss"));
    }
}

void EXAMLIGNE::on_dateEdit_dateChanged(const QDate &date)
{
    if (ui->dateTimeEdit->isVisible())
    {
        emit clear(*getId(),*getId_mere(),*descri);
        emit set(*getId(),*getId_mere(),*descri,date.toString("dd/MM/yyyy"));
    }

}

void EXAMLIGNE::on_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    if (ui->dateTimeEdit->isVisible())
    {
        emit clear(*getId(),*getId_mere(),*descri);
        emit set(*getId(),*getId_mere(),*descri,dateTime.toString("dd/MM/yyyy hh:mm:ss"));
    }

}

void EXAMLIGNE::getDefault()// pour que tout les fiches contiennent les classe mere le plus haut dans le niveau
{
    if (getId_mere()->isEmpty())
    {
        emit set(*getId(),*getId_mere(),*getDescri(),xmlManager->getDefaultValueOfMenu(*getId()));
    }
}

void EXAMLIGNE::derouler()
{
    ui->arrow->click();
    ui->arrow->setIcon(QIcon(QPixmap(":/images/down")));
}

QString *EXAMLIGNE::getId_mere() const
{
    return m_id_mere;
}

void EXAMLIGNE::setId_mere(QString *id_mere)
{
    m_id_mere = id_mere;
}

QString *EXAMLIGNE::getId() const
{
    return m_id;
}

void EXAMLIGNE::setId(QString *id)
{
    m_id = id;
}
