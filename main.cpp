#include "mainwindow.h"
#include <QApplication>
#include "parametre.h"
#include "QDebug"
#include "HObject/adresse.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

//    QFile f(":/file/style");//Load of the stylesheet
//    f.open(QIODevice::ReadOnly);
//    QString style= f.readAll();
//    a.setStyleSheet(style)


    QObject::connect(&w,&MainWindow::quitter,&a,&QApplication::quit);
    return a.exec();
}

