#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui
{
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);


    ~Form();

private:
    Ui::Form *ui;
    QStringList  values_;
    bool rowState_;
    bool addValuesState_;
    QString id_;
    QString alias_;
    QString descriptif_;
};

#endif // FORM_H
