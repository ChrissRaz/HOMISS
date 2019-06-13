#ifndef HVBLAYOUT_H
#define HVBLAYOUT_H

#include <QVBoxLayout>

class HVBLayout: public QVBoxLayout
{
public:
    HVBLayout(QString id,QWidget *parent=nullptr);

    QString getId() const;
    void setId(const QString &value);

protected:
    QString id;
};

#endif // HVBLAYOUT_H
