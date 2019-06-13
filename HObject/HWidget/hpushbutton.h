#ifndef HPUSHBUTTON_H
#define HPUSHBUTTON_H
#include <QPushButton>

namespace H {
class HPushButton;
}
class HPushButton: public QPushButton
{
public:
    HPushButton(bool state=false,QWidget *parent=nullptr);
    HPushButton(QString text,bool state=false,QWidget *parent=nullptr);

    bool state() const;
    void setState(bool state);


private slots:
    void on_click(bool checked);


signals:
    void stateChanged(bool state);

protected:
    void instantiate();
    bool m_state;
};

#endif // HPUSHBUTTON_H
