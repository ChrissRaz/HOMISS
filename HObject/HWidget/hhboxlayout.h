#ifndef HHBOXLAYOUT_H
#define HHBOXLAYOUT_H

#include <QHBoxLayout>


class HHBoxLayout: public QHBoxLayout
{
public:
    HHBoxLayout(QString id,QWidget *parent=nullptr);

    QString getId() const;
    void setId(const QString &value);

protected:
    QString id;
};

#endif // HHBOXLAYOUT_H
