#ifndef HLINEEDIT_H
#define HLINEEDIT_H

#include <QLineEdit>
class HLineEdit: public QLineEdit
{
public:
    HLineEdit(QString id, QWidget *parent=nullptr);

    QString getId() const;
    void setId(const QString &value);

protected:
    QString id;

};

#endif // HLINEEDIT_H
