#include "service_ui.h"
#include "ui_service_ui.h"

SERVICE_UI::SERVICE_UI(QWidget *parent) : QDialog(parent), ui(new Ui::SERVICE_UI)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);

    setCurrentId("0");
    xmlManager= new HXmlManager(this);
    QObject::connect(ui->confiramtion,&QDialogButtonBox::accepted,this,&SERVICE_UI::save);
    QObject::connect(ui->serviceValue,SIGNAL(currentIndexChanged(int)),this,SLOT(changeCurrentId(int)));
}

SERVICE_UI::~SERVICE_UI()
{
    delete ui;
}

void SERVICE_UI::setList(QList<SERVICE *> list)
{
    int i=0;
    QStringList vals;

    while (i<list.length())
    {
        id_services.append(QString::number(list.at(i)->getId()));
        vals.append(list.at(i)->nom());
        i++;
    }
    setCurrentId(id_services.first());
    ui->serviceValue->addItems(vals);
}

QString SERVICE_UI::getCurrentId() const
{
    return currentId;
}

void SERVICE_UI::setCurrentId(const QString &value)
{
    currentId = value;
}

void SERVICE_UI::changeCurrentId(int pos)
{
    setCurrentId(id_services.at(pos));
}

void SERVICE_UI::save()
{
    xmlManager->setStartParameters(startParamaterType::SER,false);
    xmlManager->setService(getCurrentId(),ui->serviceValue->currentText());
    this->close();
}
