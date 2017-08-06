#-------------------------------------------------
#
# Project created by QtCreator 2015-01-28T12:13:28
#
#-------------------------------------------------

QT       += core gui\
            network \
            xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServeurDrawIt2
TEMPLATE = app


SOURCES += main.cpp\
    serveur.cpp \
    minuteur.cpp \
    joueur.cpp \
    gestionjoueur.cpp \
    lecturexmlfichier.cpp \
    threadlecture.cpp

HEADERS  += \
    serveur.h \
    minuteur.h \
    joueur.h \
    gestionjoueur.h \
    lecturexmlfichier.h \
    threadlecture.h

FORMS    +=
