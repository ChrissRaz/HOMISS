#ifndef SERVICE_UI_H
#define SERVICE_UI_H

#include <QDialog>
#include "hxmlmanager.h"
#include "HObject/service.h"

namespace Ui {
class SERVICE_UI;
}

class SERVICE_UI : public QDialog
{
    Q_OBJECT

public:
    explicit SERVICE_UI(QWidget *parent = nullptr);
    ~SERVICE_UI();

    void setList(QList<SERVICE *> list);

    QString getCurrentId() const;
    void setCurrentId(const QString &value);

protected slots:
    void changeCurrentId(int pos);
    void save();

private:
    Ui::SERVICE_UI *ui;
    HXmlManager *xmlManager;
    QStringList id_services;
    QString currentId;
};

#endif // SERVICE_UI_H
