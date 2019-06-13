#-------------------------------------------------
#
# Project created by QtCreator 2018-11-18T11:30:04
#
#-------------------------------------------------

QT       += core gui sql xml printsupport  charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(QtAwesome/QtAwesome.pri)

TARGET = HOMISS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        dbmanager.cpp \
        parametre.cpp \
        service_ui.cpp \
        HObject/adresse.cpp \
        HObject/contact.cpp \
        HObject/hobject.cpp \
        HObject/infirmier.cpp \
        HObject/labeled.cpp \
        HObject/medecin.cpp \
        HObject/nationalite.cpp \
        HObject/patient.cpp \
        HObject/personel.cpp \
        HObject/personne.cpp \
        HObject/quartier.cpp \
        HObject/sante.cpp \
        HObject/service.cpp \
        HObject/sm.cpp \
        HObject/statut.cpp \
        HObject/ville.cpp \
        connexionobservator.cpp \
        hxmlmanager.cpp \
    HObject/HWidget/hline.cpp \
    HObject/sortie.cpp \
    HObject/modesortie.cpp \
    HObject/modeadmission.cpp \
    HObject/modesoin.cpp \
    HObject/HWidget/examligne.cpp \
    HObject/HWidget/addclassdialog.cpp \
    HObject/classe.cpp \
    HObject/examen.cpp \
    HObject/fiche.cpp \
    HObject/HWidget/patientview.cpp \
    HObject/HWidget/informationclass.cpp \
    HObject/HWidget/informationexamen.cpp \
    HObject/hopital.cpp \
    HObject/HWidget/hchartview.cpp \
    HObject/HWidget/transferer.cpp


HEADERS += \
        mainwindow.h \
        DBManager.h \
        parametre.h \
        service_ui.h\
        HObject/adresse.h \
        HObject/contact.h \
        HObject/hobject.h \
        HObject/hobjects.h \
        HObject/infirmier.h \
        HObject/labeled.h \
        HObject/medecin.h \
        HObject/nationalite.h \
        HObject/patient.h \
        HObject/personel.h \
        HObject/personne.h \
        HObject/quartier.h \
        HObject/sante.h \
        HObject/service.h \
        HObject/sm.h \
        HObject/statut.h \
        HObject/ville.h \
        connexionobservator.h \
        hxmlmanager.h \
    HObject/HWidget/hline.h \
    HObject/sortie.h \
    HObject/modesortie.h \
    HObject/modeadmission.h \
    HObject/modesoin.h \
    HObject/HWidget/examligne.h \
    HObject/HWidget/addclassdialog.h \
    HObject/classe.h \
    HObject/examen.h \
    HObject/fiche.h \
    HObject/HWidget/patientview.h \
    HObject/HWidget/informationclass.h \
    HObject/HWidget/informationexamen.h \
    HObject/hopital.h \
    HObject/HWidget/hchartview.h \
    HObject/HWidget/transferer.h

FORMS += \
        mainwindow.ui \
    parametre.ui \
    service_ui.ui \
    HObject/HWidget/hline.ui \
    HObject/HWidget/examligne.ui \
    HObject/HWidget/addclassdialog.ui \
    HObject/HWidget/patientview.ui \
    HObject/HWidget/informationclass.ui \
    HObject/HWidget/informationexamen.ui \
    HObject/HWidget/hchartview.ui \
    HObject/HWidget/transferer.ui

RESOURCES += \
    hom_ressources.qrc



DISTFILES +=


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Apps/Mysql/mysql-5.7.25-win32/mysql-5.7.25-win32/lib/ -llibmysql
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Apps/Mysql/mysql-5.7.25-win32/mysql-5.7.25-win32/lib/ -llibmysqld
else:unix: LIBS += -L$$PWD/../../../Apps/Mysql/mysql-5.7.25-win32/mysql-5.7.25-win32/lib/ -llibmysql

INCLUDEPATH += $$PWD/../../../Apps/Mysql/mysql-5.7.25-win32/mysql-5.7.25-win32/include
DEPENDPATH += $$PWD/../../../Apps/Mysql/mysql-5.7.25-win32/mysql-5.7.25-win32/include
