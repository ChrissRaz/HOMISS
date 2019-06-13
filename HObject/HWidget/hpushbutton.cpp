#include "hpushbutton.h"

HPushButton::HPushButton(bool state, QWidget *parent): QPushButton (parent)
{
    this->setState(state);
    instantiate();
    connect(this,&QPushButton::clicked,this,&HPushButton::on_click);
}

HPushButton::HPushButton(QString text, bool state, QWidget *parent): QPushButton (text,parent)
{
    this->setState(state);
    instantiate();
}


void HPushButton::instantiate()
{
    this->setFlat(true);
    this->setMaximumWidth(250);
    this->setMaximumHeight(25);
}

bool HPushButton::state() const
{
    return m_state;
}

void HPushButton::setState(bool state)
{
    m_state = state;
}

void HPushButton::on_click(bool checked)
{
    bool s;
    if (state())
    {
        s=false;
    }
    else
    {
        s=true;
    }

    setState(s);
//    emit stateChanged(s);
}


